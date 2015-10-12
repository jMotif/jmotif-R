// This file was generated by Rcpp::compileAttributes
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// znorm
NumericVector znorm(NumericVector x, double threshold);
RcppExport SEXP jmotif_znorm(SEXP xSEXP, SEXP thresholdSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< NumericVector >::type x(xSEXP);
    Rcpp::traits::input_parameter< double >::type threshold(thresholdSEXP);
    __result = Rcpp::wrap(znorm(x, threshold));
    return __result;
END_RCPP
}
// reshape_cpp
NumericMatrix reshape_cpp(NumericMatrix a, int n, int m);
RcppExport SEXP jmotif_reshape_cpp(SEXP aSEXP, SEXP nSEXP, SEXP mSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< NumericMatrix >::type a(aSEXP);
    Rcpp::traits::input_parameter< int >::type n(nSEXP);
    Rcpp::traits::input_parameter< int >::type m(mSEXP);
    __result = Rcpp::wrap(reshape_cpp(a, n, m));
    return __result;
END_RCPP
}
// col_means_cpp
NumericVector col_means_cpp(NumericMatrix a);
RcppExport SEXP jmotif_col_means_cpp(SEXP aSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< NumericMatrix >::type a(aSEXP);
    __result = Rcpp::wrap(col_means_cpp(a));
    return __result;
END_RCPP
}
// paa
NumericVector paa(NumericVector ts, int paa_num);
RcppExport SEXP jmotif_paa(SEXP tsSEXP, SEXP paa_numSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< NumericVector >::type ts(tsSEXP);
    Rcpp::traits::input_parameter< int >::type paa_num(paa_numSEXP);
    __result = Rcpp::wrap(paa(ts, paa_num));
    return __result;
END_RCPP
}
// idx2letter_cpp
char idx2letter_cpp(int idx);
RcppExport SEXP jmotif_idx2letter_cpp(SEXP idxSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< int >::type idx(idxSEXP);
    __result = Rcpp::wrap(idx2letter_cpp(idx));
    return __result;
END_RCPP
}
// letter2idx_cpp
int letter2idx_cpp(char letter);
RcppExport SEXP jmotif_letter2idx_cpp(SEXP letterSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< char >::type letter(letterSEXP);
    __result = Rcpp::wrap(letter2idx_cpp(letter));
    return __result;
END_RCPP
}
// letters2idx_cpp
IntegerVector letters2idx_cpp(CharacterVector str);
RcppExport SEXP jmotif_letters2idx_cpp(SEXP strSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< CharacterVector >::type str(strSEXP);
    __result = Rcpp::wrap(letters2idx_cpp(str));
    return __result;
END_RCPP
}
// alphabet2cuts_cpp
NumericVector alphabet2cuts_cpp(int a_size);
RcppExport SEXP jmotif_alphabet2cuts_cpp(SEXP a_sizeSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< int >::type a_size(a_sizeSEXP);
    __result = Rcpp::wrap(alphabet2cuts_cpp(a_size));
    return __result;
END_RCPP
}
// ts2chars_cpp
CharacterVector ts2chars_cpp(NumericVector ts, int a_size);
RcppExport SEXP jmotif_ts2chars_cpp(SEXP tsSEXP, SEXP a_sizeSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< NumericVector >::type ts(tsSEXP);
    Rcpp::traits::input_parameter< int >::type a_size(a_sizeSEXP);
    __result = Rcpp::wrap(ts2chars_cpp(ts, a_size));
    return __result;
END_RCPP
}
// ts_to_string
CharacterVector ts_to_string(NumericVector ts, int a_size);
RcppExport SEXP jmotif_ts_to_string(SEXP tsSEXP, SEXP a_sizeSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< NumericVector >::type ts(tsSEXP);
    Rcpp::traits::input_parameter< int >::type a_size(a_sizeSEXP);
    __result = Rcpp::wrap(ts_to_string(ts, a_size));
    return __result;
END_RCPP
}
// subseries
NumericVector subseries(NumericVector x, int start, int end);
RcppExport SEXP jmotif_subseries(SEXP xSEXP, SEXP startSEXP, SEXP endSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< NumericVector >::type x(xSEXP);
    Rcpp::traits::input_parameter< int >::type start(startSEXP);
    Rcpp::traits::input_parameter< int >::type end(endSEXP);
    __result = Rcpp::wrap(subseries(x, start, end));
    return __result;
END_RCPP
}
// is_equal_str
bool is_equal_str(CharacterVector a, CharacterVector b);
RcppExport SEXP jmotif_is_equal_str(SEXP aSEXP, SEXP bSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< CharacterVector >::type a(aSEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type b(bSEXP);
    __result = Rcpp::wrap(is_equal_str(a, b));
    return __result;
END_RCPP
}
// sax_via_window
std::map<int, CharacterVector> sax_via_window(NumericVector ts, int w_size, int paa_size, int a_size, CharacterVector nr_strategy, double n_threshold);
RcppExport SEXP jmotif_sax_via_window(SEXP tsSEXP, SEXP w_sizeSEXP, SEXP paa_sizeSEXP, SEXP a_sizeSEXP, SEXP nr_strategySEXP, SEXP n_thresholdSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< NumericVector >::type ts(tsSEXP);
    Rcpp::traits::input_parameter< int >::type w_size(w_sizeSEXP);
    Rcpp::traits::input_parameter< int >::type paa_size(paa_sizeSEXP);
    Rcpp::traits::input_parameter< int >::type a_size(a_sizeSEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type nr_strategy(nr_strategySEXP);
    Rcpp::traits::input_parameter< double >::type n_threshold(n_thresholdSEXP);
    __result = Rcpp::wrap(sax_via_window(ts, w_size, paa_size, a_size, nr_strategy, n_threshold));
    return __result;
END_RCPP
}
// sax_by_chunking
std::map<int, CharacterVector> sax_by_chunking(NumericVector ts, int paa_size, int a_size, double n_threshold);
RcppExport SEXP jmotif_sax_by_chunking(SEXP tsSEXP, SEXP paa_sizeSEXP, SEXP a_sizeSEXP, SEXP n_thresholdSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< NumericVector >::type ts(tsSEXP);
    Rcpp::traits::input_parameter< int >::type paa_size(paa_sizeSEXP);
    Rcpp::traits::input_parameter< int >::type a_size(a_sizeSEXP);
    Rcpp::traits::input_parameter< double >::type n_threshold(n_thresholdSEXP);
    __result = Rcpp::wrap(sax_by_chunking(ts, paa_size, a_size, n_threshold));
    return __result;
END_RCPP
}
// series_to_wordbag
std::map<std::string, int> series_to_wordbag(NumericVector ts, int w_size, int paa_size, int a_size, CharacterVector nr_strategy, double n_threshold);
RcppExport SEXP jmotif_series_to_wordbag(SEXP tsSEXP, SEXP w_sizeSEXP, SEXP paa_sizeSEXP, SEXP a_sizeSEXP, SEXP nr_strategySEXP, SEXP n_thresholdSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< NumericVector >::type ts(tsSEXP);
    Rcpp::traits::input_parameter< int >::type w_size(w_sizeSEXP);
    Rcpp::traits::input_parameter< int >::type paa_size(paa_sizeSEXP);
    Rcpp::traits::input_parameter< int >::type a_size(a_sizeSEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type nr_strategy(nr_strategySEXP);
    Rcpp::traits::input_parameter< double >::type n_threshold(n_thresholdSEXP);
    __result = Rcpp::wrap(series_to_wordbag(ts, w_size, paa_size, a_size, nr_strategy, n_threshold));
    return __result;
END_RCPP
}
