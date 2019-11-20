#' Class for phase-type NHPP-based software reliability model (canonical form 1)
#'
#' @docType class
#' @return Object of \code{\link{R6Class}} with methods for NHPP-based software reliability model.
#' @format \code{\link{R6Class}} object.
#' @field name A character string for the name of model.
#' @field params A list of the model parameters.
#' @field df An integer for the degrees of freedom of the model.
#' @field data Data to esimate parameters.
#'
#' @section Methods:
#' \describe{
#'   \item{\code{print()}}{This method prints model parameters.}
#'   \item{\code{omega()}}{This method returns the number of total faults.}
#'   \item{\code{mvf(t)}}{This method returns the mean value function at time t.}
#'   \item{\code{dmvf(t)}}{This method returns the mean value function on discrete time domain.}
#'   \item{\code{inv_mvf(x)}}{This method returns the time at which the mean value function attains x.}
#'   \item{\code{intensity(t)}}{This method returns the intensity function at time t.}
#'   \item{\code{reliab(t, s)}}{This method returns the software reliability at time t from the orign s.}
#'   \item{\code{residual(t)}}{This method returns the expected residual number of faults at time t.}
#'   \item{\code{ffp(t)}}{This method returns the fault-free probability at time t.}
#'   \item{\code{imtbf(t)}}{This method returns the instantaneous MTBF at time t.}
#'   \item{\code{cmtbf(t)}}{This method returns the cumulative MTBF at time t.}
#'   \item{\code{median(s, p = 0.5)}}{This method returns the time at which the software reliability attains the proability p from the orign s.}
#'   \item{\code{init_params(data)}}{This method changes the model parameters based on a given data. This is used to set the initial value for the fitting algorithm.}
#'   \item{\code{set_params(params)}}{This method sets the model parameters.}
#'   \item{\code{em(params, data)}}{This method returns a list with an updated parameter vector (param),
#'          absolute difference of parameter vector (pdiff),
#'          log-likelihood function for a given parameter vector (llf),
#'          the number of total faults (total) via EM algorithm for a given data.
#'          \emph{eps} is the tolerance error for uniformization.
#'          \emph{ufactor} is a uniformization factor.}
#'   \item{\code{llf(data)}}{This method returns the log-likelihood function for a given data.}
#' }

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
    initialize = function(n, omega = 1, alpha = rep(1/n,n), rate = rep(1,n)) {
      private$n <- n
      self$params <- list(omega=omega, alpha=alpha, rate=rate)
      self$df <- 2*n
      self$name <- paste("cph", n, sep = "")
    },
    omega = function() { self$params$omega },
    alpha = function() { self$params$alpha },
    rate = function() { self$params$rate },
    print = function(digits = max(3, getOption("digits") - 3), ...) {
      cat(gettextf("Model name: %s\n", self$name))
      print.default(format(self$omega(), digits = digits), print.gap = 2, quote = FALSE)
      print.default(format(self$alpha(), digits = digits), print.gap = 2, quote = FALSE)
      print.default(format(self$rate(), digits = digits), print.gap = 2, quote = FALSE)
    },
    init_params = function(data, shape.init = c(1, 4, 16, 64, 256, 1024),
                           scale.init = c(0.5, 1.0, 2.0),
                           maxiter.init = 5, verbose = FALSE) {
      self$params <- cf1.params.init(private$n, data,
                                     shape.init = shape.init,
                                     scale.init = scale.init,
                                     maxiter.init = maxiter.init,
                                     verbose = verbose)
    },
    set_params = function(params) {
      stopifnot(private$n == length(params$alpha))
      self$params <- params
    },
    em = function(params, data, eps = 1.0e-8, ufactor = 1.01) {
      em_cf1_emstep(params, data, eps, ufactor)
    },
    llf = function(data) {
      cf1llf(data, self$omega(), self$alpha(), self$rate())
    }
  )
)
