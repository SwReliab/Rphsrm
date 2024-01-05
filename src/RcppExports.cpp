// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

#ifdef RCPP_USE_GLOBAL_ROSTREAM
Rcpp::Rostream<true>&  Rcpp::Rcout = Rcpp::Rcpp_cout_get();
Rcpp::Rostream<false>& Rcpp::Rcerr = Rcpp::Rcpp_cerr_get();
#endif

// C_cf1pdf
NumericVector C_cf1pdf(NumericVector dx, NumericVector alpha, NumericVector rate, double eps, double ufactor, bool log);
RcppExport SEXP _Rphsrm_C_cf1pdf(SEXP dxSEXP, SEXP alphaSEXP, SEXP rateSEXP, SEXP epsSEXP, SEXP ufactorSEXP, SEXP logSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< NumericVector >::type dx(dxSEXP);
    Rcpp::traits::input_parameter< NumericVector >::type alpha(alphaSEXP);
    Rcpp::traits::input_parameter< NumericVector >::type rate(rateSEXP);
    Rcpp::traits::input_parameter< double >::type eps(epsSEXP);
    Rcpp::traits::input_parameter< double >::type ufactor(ufactorSEXP);
    Rcpp::traits::input_parameter< bool >::type log(logSEXP);
    rcpp_result_gen = Rcpp::wrap(C_cf1pdf(dx, alpha, rate, eps, ufactor, log));
    return rcpp_result_gen;
END_RCPP
}
// C_cf1cdf
NumericVector C_cf1cdf(NumericVector dx, NumericVector alpha, NumericVector rate, double eps, double ufactor, bool lower, bool log);
RcppExport SEXP _Rphsrm_C_cf1cdf(SEXP dxSEXP, SEXP alphaSEXP, SEXP rateSEXP, SEXP epsSEXP, SEXP ufactorSEXP, SEXP lowerSEXP, SEXP logSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< NumericVector >::type dx(dxSEXP);
    Rcpp::traits::input_parameter< NumericVector >::type alpha(alphaSEXP);
    Rcpp::traits::input_parameter< NumericVector >::type rate(rateSEXP);
    Rcpp::traits::input_parameter< double >::type eps(epsSEXP);
    Rcpp::traits::input_parameter< double >::type ufactor(ufactorSEXP);
    Rcpp::traits::input_parameter< bool >::type lower(lowerSEXP);
    Rcpp::traits::input_parameter< bool >::type log(logSEXP);
    rcpp_result_gen = Rcpp::wrap(C_cf1cdf(dx, alpha, rate, eps, ufactor, lower, log));
    return rcpp_result_gen;
END_RCPP
}
// C_cf1sample
NumericVector C_cf1sample(int n, NumericVector alpha, NumericVector rate);
RcppExport SEXP _Rphsrm_C_cf1sample(SEXP nSEXP, SEXP alphaSEXP, SEXP rateSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< int >::type n(nSEXP);
    Rcpp::traits::input_parameter< NumericVector >::type alpha(alphaSEXP);
    Rcpp::traits::input_parameter< NumericVector >::type rate(rateSEXP);
    rcpp_result_gen = Rcpp::wrap(C_cf1sample(n, alpha, rate));
    return rcpp_result_gen;
END_RCPP
}
// C_cf1reform
List C_cf1reform(NumericVector alpha, NumericVector rate);
RcppExport SEXP _Rphsrm_C_cf1reform(SEXP alphaSEXP, SEXP rateSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< NumericVector >::type alpha(alphaSEXP);
    Rcpp::traits::input_parameter< NumericVector >::type rate(rateSEXP);
    rcpp_result_gen = Rcpp::wrap(C_cf1reform(alpha, rate));
    return rcpp_result_gen;
END_RCPP
}
// C_cf1sojourn
NumericVector C_cf1sojourn(NumericVector alpha, NumericVector rate, NumericVector f, NumericVector b, double t, double eps, double ufactor);
RcppExport SEXP _Rphsrm_C_cf1sojourn(SEXP alphaSEXP, SEXP rateSEXP, SEXP fSEXP, SEXP bSEXP, SEXP tSEXP, SEXP epsSEXP, SEXP ufactorSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< NumericVector >::type alpha(alphaSEXP);
    Rcpp::traits::input_parameter< NumericVector >::type rate(rateSEXP);
    Rcpp::traits::input_parameter< NumericVector >::type f(fSEXP);
    Rcpp::traits::input_parameter< NumericVector >::type b(bSEXP);
    Rcpp::traits::input_parameter< double >::type t(tSEXP);
    Rcpp::traits::input_parameter< double >::type eps(epsSEXP);
    Rcpp::traits::input_parameter< double >::type ufactor(ufactorSEXP);
    rcpp_result_gen = Rcpp::wrap(C_cf1sojourn(alpha, rate, f, b, t, eps, ufactor));
    return rcpp_result_gen;
END_RCPP
}
// em_cf1_emstep
List em_cf1_emstep(List params, List data, double eps, double ufactor);
RcppExport SEXP _Rphsrm_em_cf1_emstep(SEXP paramsSEXP, SEXP dataSEXP, SEXP epsSEXP, SEXP ufactorSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< List >::type params(paramsSEXP);
    Rcpp::traits::input_parameter< List >::type data(dataSEXP);
    Rcpp::traits::input_parameter< double >::type eps(epsSEXP);
    Rcpp::traits::input_parameter< double >::type ufactor(ufactorSEXP);
    rcpp_result_gen = Rcpp::wrap(em_cf1_emstep(params, data, eps, ufactor));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_Rphsrm_C_cf1pdf", (DL_FUNC) &_Rphsrm_C_cf1pdf, 6},
    {"_Rphsrm_C_cf1cdf", (DL_FUNC) &_Rphsrm_C_cf1cdf, 7},
    {"_Rphsrm_C_cf1sample", (DL_FUNC) &_Rphsrm_C_cf1sample, 3},
    {"_Rphsrm_C_cf1reform", (DL_FUNC) &_Rphsrm_C_cf1reform, 2},
    {"_Rphsrm_C_cf1sojourn", (DL_FUNC) &_Rphsrm_C_cf1sojourn, 7},
    {"_Rphsrm_em_cf1_emstep", (DL_FUNC) &_Rphsrm_em_cf1_emstep, 4},
    {NULL, NULL, 0}
};

RcppExport void R_init_Rphsrm(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
