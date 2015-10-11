library(microbenchmark)
data = read.table("assets/300_signal1.txt")
system.time(znorm(data, 0.01))
system.time(jmotif::znorm_cpp(data, 0.01))

y = c(-1, -2, -1, 0, 2, 1, 1, 0)
microbenchmark (
  znorm_cpp(y),
  znorm(y)
)

library(matlab)
a = matrix(rnorm(100), nrow=20, ncol=5)
dim(a)
b = matlab::reshape(a, 5, 20)
dim(b)
c = jmotif::reshape_cpp(a, 5, 20)
dim(b)
y = c(-1, -2, -1, 0, 2, 1, 1, 0)

microbenchmark (
  matlab::reshape(a, 5, 20),
  jmotif::reshape_cpp(a, 5, 20)
)

microbenchmark (
  paa(y, 3),
  paa_cpp(y,3)
)

microbenchmark (
  idx2letter(sample(1:21, 1)),
  idx2letter_cpp(sample(1:21, 1))
)

microbenchmark (
  letter2idx('c'),
  letter2idx_cpp('c')
)

microbenchmark (
  letters2idx_cpp(c('a','e','s','g')),
  letters2idx(c('a','e','s','g'))
)

microbenchmark (
  ts2string(c(-1.375, 0.75, 0.625), 3),
  ts2chars_cpp(c(-1.375, 0.75, 0.625), 3),
  ts2string_cpp(c(-1.375, 0.75, 0.625), 3)
)

y = c(-1, -2, -1, 0, 2, 1, 1, 0)
sax_via_window(y, 3, 2, 3, "none", 0.01)
sax_via_window(y, 3, 2, 3, "exact", 0.01)

