#' Phase-Type distribution (Canonical)
#'
#' Density, distribution function, and quantile function for
#' canonical phase-type distribution.
#'
#' The PH distribution has the density distribution function
#' F(x) = alpha exp(T x) tau where alpha is an initial probability vector,
#' T is an infinitesimal generator matrix and tau is an exit rate vector.
#' 
#' The canonical form 1 (CF1) is a special case of PH distribution, in which
#' The infinitesimal generator consists of only diagonal and superdiagonal entries, i.e.,
#' The phase moves to only the next stage and the last state is the absorbing state of
#' underlying continuous-time Markov chain. In CF1, the parameters are dominated only
#' by the initial probability vector and the transtion rates to the next stage. In addition,
#' transition rates of CF1 increases with the stage processes.
#'
#' @name cf1
#' @param x,q A numeric vector of quantiles.
#' @param p A numeric vector of probabilities.
#' @param alpha A numeric vector for initial probabilities.
#' @param rate A numeric vector for transition rates to the next stage
#' (diagonal entries of infinitesimal generator matrix).
#' @param lower.tail A logical; if TRUE, the probability is P[X <= x], otherwise, P[X > x].
#' @param log,log.p A logical; if TRUE, the probability p is given as log(p).
#' @return 'dcf1' gives the desity, 'pcf1' gives the distribution, and
#' 'qcf1' gives the quantile function.
NULL

#' @rdname cf1
#' @export
dcf1 <- function(x, alpha = 1, rate = 1, log = FALSE, eps = 1.0e-8, unif.factor = 1.01) {
  s <- order(x)
  dx <- diff(c(0, x[s]))
  res <- C_cf1pdf(dx, alpha, rate, eps, unif.factor, log)
  res[order((1:length(x))[s])]
}

#' @rdname cf1
#' @export
pcf1 <- function(q, alpha = 1, rate = 1, lower.tail = TRUE, log.p = FALSE,
                 eps = 1.0e-8, unif.factor = 1.01) {
  s <- order(q)
  dx <- diff(c(0, q[s]))
  res <- C_cf1cdf(dx, alpha, rate, eps, unif.factor, lower.tail, log.p)
  res[order((1:length(q))[s])]
}

#' @rdname cf1
#' @export
qcf1 <- function(p, alpha = 1, rate = 1, lower.tail = TRUE, log.p = FALSE) {
  NULL
}

#' @rdname cf1
#' @export
rcf1 <- function(n, alpha = 1, rate = 1, scramble = TRUE) {
  res <- C_cf1sample(n, alpha, rate)
  if (scramble) {
    sample(res)
  } else {
    res
  }
}

#' Simple parameter estimation for PH
#'
#' Determine the parameters of CF1 from data.
#' The rate increases expoentially.
#'
#' @param n An integer for the number of phases
#' @param scale A numeric vector of scale. This corresponds to the mean of CF1.
#' @param shape A numeric vector of shape. This is a parameter to indicate the
#' difference between the maximum/minimum rates.
#' @return List of CF1 parameters.

cf1.params.power <- function(n, scale, shape) {
  rate <- rep(0, n)
  p <- exp(1.0/(n-1) * log(shape))
  total <- 1.0
  tmp <- 1.0
  for (i in 1:(n-1)) {
    tmp <- tmp * (i+1) / (i * p)
    total <- total + tmp
  }
  base <- total / (n * scale);
  tmp <- base
  for (i in 1:n) {
    rate[i] <- tmp
    tmp <- tmp * p
  }
  list(alpha=rep(1/n, n), rate=rate)
}

#' Simple parameter estimation for PH
#'
#' Determine the parameters of CF1 from data.
#' The rate increases linearly.
#'
#' @param n An integer for the number of phases
#' @param scale A numeric vector of scale. This corresponds to the mean of CF1.
#' @param shape A numeric vector of shape. This is a parameter to indicate the
#' difference between the maximum/minimum rates.
#' @return List of CF1 parameters.

cf1.params.linear <- function(n, scale, shape) {
  rate <- rep(0, n)
  al <- (shape-1)/(n-1)
  total <- 1.0
  for (i in 1:n) {
    total <- total + (i+1) / (al * i + 1)
  }
  base <- total / (n * scale);
  for (i in 1:n) {
    rate[i] <- base * (al + i + 1)
  }
  list(alpha=rep(1/n, n), rate=rate)
}

#' Simple parameter estimation for PH
#'
#' Determine the parameters of CF1 from data.
#' The rate increases linearly.
#'
#' @param n An integer for the number of phases
#' @param scale A numeric vector of scale. This corresponds to the mean of CF1.
#' @param shape A numeric vector of shape. This is a parameter to indicate the
#' difference between the maximum/minimum rates.
#' @return List of CF1 parameters.

cf1.params.linear <- function(n, scale, shape) {
  rate <- rep(0, n)
  al <- (shape-1)/(n-1)
  total <- 1.0
  for (i in 1:n) {
    total <- total + (i+1) / (al * i + 1)
  }
  base <- total / (n * scale);
  for (i in 1:n) {
    rate[i] <- base * (al + i + 1)
  }
  list(alpha=rep(1/n, n), rate=rate)
}

#' Simple parameter estimation for PH
#'
#' Determine the parameters of CF1 from data.
#' Try all candidates with several EM steps and select the best one.
#'
#' @param n An integer for the number of phases
#' @param data A data.frame for fault data. It should be an instance of Rsrat.faultdata
#' @param diff.init A numeric vector for shape
#' @param scale.init A numeric vector for scale
#' @param maxiter.init An integer of emsteps
#' @param verbose A logic to display the results of trials
#' @return List of CF1 parameters.

cf1.params.init <- function(n, data,
                            shape.init = c(1, 4, 16, 64, 256, 1024),
                            scale.init = c(0.5, 1.0, 2.0),
                            maxiter.init = 5, verbose = FALSE) {
  if (verbose)
    cat("Initializing CF1 ...\n")
  m <- data$mean
  maxllf <- -Inf
  maxph <- NULL
  for (fn in c(cf1.params.power, cf1.params.linear)) {
    for (x in scale.init) {
      for (s in shape.init) {
        res <- list(param=c(list(omega=data$total), fn(n, scale=m*x, shape=s)))
        phres <- try({
          for (k in 1:maxiter.init) {
            res <- em_cf1_emstep(res$param, data)
          }
          res
        })
        if (class(phres) != "try-error") {
          if (is.finite(phres$llf)) {
            if (maxllf < phres$llf) {
              maxllf <- phres$llf
              maxph <- phres$param
              if (verbose) cat("o")
            } else {
              if (verbose) cat("x")
            }
          } else {
            if (verbose) cat("-")
          }
        } else {
          if (verbose) cat("-")
        }
      }
    }
    if (verbose) cat("\n")
  }
  maxph
}

#' Moments of CF1
#'
#' Compute the moments of CF1
#'
#' @param n An integer of the degree of moment
#' @param alpha A numeric vector for initial probabilities.
#' @param rate A numeric vector for transition rates.
#' @return A value of moment

cf1moment <- function(n, alpha, rate) {
  tmp <- alpha
  tmp2 <- 1.0
  res <- numeric(0)
  # for (i in 1:k) {
  #             tmp <- msolve(alpha=1.0, A=-as.matrix(ph@Q), x=tmp, transpose=TRUE)
  #             tmp2 <- tmp2 * i
  #             res <- c(res, tmp2 * sum(tmp))
  #           }
  #           res
  #         }
}

