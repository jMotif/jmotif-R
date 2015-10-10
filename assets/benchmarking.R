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
