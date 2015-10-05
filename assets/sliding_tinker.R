x = seq(0, pi*4, 0.02)
ts = sin(x) * 5 + rnorm(length(x))
paa_size = 5
alphabet_size = 3
window_size = 10
n_threshold = 0.01

library(plyr)
#ts2sax_via_window <- function(ts, paa_size, alphabet_size, window_size, n_threshold){

  res = list()
  for (i in c(1:(length(ts) - window_size - 1))) {
    tmp = ts[i:(i + window_size)]
    tmp_znorm = znorm(tmp, n_threshold)
    tmp_paa = paa(tmp_znorm, paa_size)
    tmp_sax = paste(ts2string(tmp_paa, alphabet_size), sep = "", collapse = "")
    res = c(res, c(i, tmp_sax))
  }


#}

i=100
