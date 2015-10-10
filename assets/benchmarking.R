data = read.table("assets/300_signal1.txt")
system.time(znorm(data, 0.01))
library(Rcpp)
system.time(jmotif::znorm_cpp(data, 0.01))

y = c(-1, -2, -1, 0, 2, 1, 1, 0)
system.time(znorm_cpp(data$V1))
system.time(mean(data$V1))
