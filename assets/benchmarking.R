library(microbenchmark)
data = read.table("assets/300_signal1.txt")
system.time(znorm(data, 0.01))
system.time(jmotif::znorm_cpp(data, 0.01))

y = c(-1, -2, -1, 0, 2, 1, 1, 0)
microbenchmark (
  znorm_cpp(y),
  znorm(y)
)
