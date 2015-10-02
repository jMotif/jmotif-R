\name{paa}
\alias{paa}
\title{Time series PAA transform}
\usage{
  paa(ts, npoints)
}
\arguments{
  \item{ts}{an input time series}
  \item{npoints}{a desired number of PAA segments}
}
\description{
  Performs piecewise aggregate approximation of a time series.
}
\examples{
  x = c(-1, -2, -1, 0, 2, 1, 1, 0)
  plot(x,type="l",main="8-points time series and it PAA transform into three points")
  points(x,pch=16,lwd=5)
  #segments
  abline(v=c(1,1+7/3,1+7/3*2,8),lty=3,lwd=2)
}
