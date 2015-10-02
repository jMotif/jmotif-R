\name{znorm}
\alias{znorm}
\title{Time series z-normalization}
\usage{
znorm(ts, threshold)
}
\arguments{
  \item{ts}{an input time series}
  \item{threshold}{an optional parameter specifying the normalization threshold}
}
\description{
Subtracts the mean and brings the standard deviation to a unit value.
}
\examples{
x = seq(0, pi*4, 0.02)
y = sin(x) * 5 + rnorm(length(x))
plot(x, y, type="l", col="blue")
lines(x, znorm(y, 0.01), type="l", col="red")
}
