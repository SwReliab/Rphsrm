#' Class for phase-type NHPP-based software reliability model (canonical form 1)
#'
#' @docType class
#' @return Object of \code{\link{R6Class}} with methods for NHPP-based software reliability model.
#' @format \code{\link{R6Class}} object.
CPHSRM <- R6::R6Class("CPHSRM",
  inherit = Rsrat::NHPP,
  private = list(
    n = NA,
    Ft = function(t, lower.tail = TRUE) {
      pcf1(t, alpha=self$alpha(), rate=self$rate(), lower.tail=lower.tail)
    },
    invFt = function(p) { qcf1(p, alpha=self$alpha(), rate=self$rate()) },
    ft = function(t) { dcf1(t, alpha=self$alpha(), rate=self$rate()) },
    rf = function(n) { rcf1(n, alpha=self$alpha(), rate=self$rate(), scramble=FALSE) }
  ),
  public = list(
    #' @description
    #' The number of total faults.
    #' @param n An integer for the number of phases.
    #' @param omega A value for the number of total faults.
    #' @param alpha A vector of the initial probabilities.
    #' @param rate A vector of transition rates.
    initialize = function(n, omega = 1, alpha = rep(1/n,n), rate = rep(1,n)) {
      private$n <- n
      self$params <- list(omega=omega, alpha=alpha, rate=rate)
      self$df <- 2*n
      self$name <- paste("cph", n, sep = "")
    },
    #' @description
    #' The number of total faults.
    #' @return The number of total faults.
    omega = function() { self$params$omega },
    #' @description
    #' The initial probability vector.
    #' @return The initial probability vector.
    alpha = function() { self$params$alpha },
    #' @description
    #' The rate vector.
    #' @return The rate vector.
    rate = function() { self$params$rate },
    #' @description
    #' Print model parameters.
    #' @param digits An integer to determine the number of digits for print.
    #' @param ... Others
    print = function(digits = max(3, getOption("digits") - 3), ...) {
      cat(gettextf("Model name: %s\n", self$name))
      print.default(format(self$omega(), digits = digits), print.gap = 2, quote = FALSE)
      print.default(format(self$alpha(), digits = digits), print.gap = 2, quote = FALSE)
      print.default(format(self$rate(), digits = digits), print.gap = 2, quote = FALSE)
    },
    #' @description
    #' Make a flatten parameter vector.
    #' @param params A vector of parameters.
    get_params = function(params) {
      c(params$omega, params$alpha, params$rate)
    },
    #' @description 
    #' Set initial parameters from a given data.
    #' This is used to set the initial value for
    #' the fitting algorithm.
    #' @param data Data.
    #' @param shape.init A list of shapes.
    #' @param scale.init A list of scale fraction.
    #' @param maxiter.init An integer for the number of iterations.
    #' @param verbose A Boolean whether the detailed information.
    init_params = function(data, shape.init = c(1, 4, 16, 64, 256, 1024),
                           scale.init = c(0.5, 1.0, 2.0),
                           maxiter.init = 5, verbose = FALSE) {
      self$params <- cf1.params.init(private$n, data,
                                     shape.init = shape.init,
                                     scale.init = scale.init,
                                     maxiter.init = maxiter.init,
                                     verbose = verbose)
    },
    #' @description 
    #' Set model parameters.
    #' @param params Parameters.
    set_params = function(params) {
      stopifnot(private$n == length(params$alpha))
      self$params <- params
    },
    #' @description 
    #' Set omega parameter.
    #' @param params Parameters.
    #' @param x A value of omega.
    set_omega = function(params, x) {
      params$omega <- x
      params
    },
    #' @description 
    #' Execute an EM step.
    #' @param params Parameters.
    #' @param data Data.
    #' @param eps A value for tolerance error.
    #' @param ufactor A value for a factor to make uniformed kernel.
    #' @return
    #' A list with the following
    #' \describe{
    #' \item{param}{Updated parameters.}
    #' \item{pdiff}{Absolute difference of parameter vector.}
    #' \item{llf}{Log-likelihood function for a given parameter vector.}
    #' \item{total}{The number of total faults.}
    #' }
    em = function(params, data, eps = 1.0e-8, ufactor = 1.01) {
      em_cf1_emstep(params, data, eps, ufactor)
    },
    #' @description 
    #' The log-likelihood function for a given data.
    #' @param data Data.
    #' @return Log-likelihood function.
    llf = function(data) {
      cf1llf(data, self$omega(), self$alpha(), self$rate())
    }
  )
)
