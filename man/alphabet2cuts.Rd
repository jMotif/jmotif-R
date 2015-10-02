\name{alphabet2cuts}
\alias{alphabet2cuts}
\title{Translates an alphabet size into the array of corrsponding (assuming the Normal distribution) SAX cut-lines}
\usage{
alphabet2cuts(a_size)
}
\arguments{
  \item{a_size}{the alphabet size, a value between 2 and 20 (inclusive)}d}
}
\description{
Returns an array of "cut points" for SAX alphabet.
}
\examples{
alphabet2cuts(5)
}
