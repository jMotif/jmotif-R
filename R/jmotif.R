#
#   Build and Reload Package:  'Ctrl + Shift + B'
#   Check Package:             'Ctrl + Shift + E'
#   Test Package:              'Ctrl + Shift + T'
#   Roxigenize                 'Ctrl + Shift + D'?

#'@importFrom Rcpp evalCpp
letters <- c("a",  "b",  "c",  "d",  "e", "f",  "g",  "h",  "i",  "j",
             "k",  "l",  "m",  "n",  "o", "p",  "q",  "r",  "s",  "t",
             "u",  "v",  "w",  "x",  "y",  "z")

#' Translates an alphabet size into the array of corresponding
#' SAX cut-lines assuming the Normal distribution.
#'
#' @param a_size the alphabet size, a value between 2 and 20 (inclusive)
#' @return Returns an array of "cut points" for SAX alphabet
#' @export
#' @examples
#' alphabet2cuts(5)
alphabet2cuts <- function(a_size) {

  if (a_size > 20 || a_size < 2) {
    stop(paste("unable to get alphabet cuts for the alphabet size ",
               a_size, ", valid sizes 2 - 20.",sep = ""))
  }

  switch(a_size,
         0.00, #1
         c(-Inf,  0.00), #2
         c(-Inf, -0.43,  0.43), #3
         c(-Inf, -0.67,  0.00,  0.67), #4
         c(-Inf, -0.84, -0.25,  0.25,  0.84), #5
         c(-Inf, -0.97, -0.43,  0.00,  0.43,  0.97), #6
         c(-Inf, -1.07, -0.57, -0.18,  0.18,  0.57,  1.07), #7
         c(-Inf, -1.15, -0.67, -0.32,  0.00,  0.32,  0.67,  1.15), #8
         c(-Inf, -1.22, -0.76, -0.43, -0.14,  0.14,  0.43,  0.76,  1.22), #9
         c(-Inf, -1.28, -0.84, -0.52, -0.25,  0.00,  0.25,  0.52,  0.84,  1.28), #10
         c(-Inf, -1.34, -0.91, -0.60, -0.35, -0.11,  0.11,  0.35,  0.60,  0.91, 1.34), #11
         c(-Inf, -1.38, -0.97, -0.67, -0.43, -0.21,  0.00,  0.21,  0.43,  0.67, 0.97, 1.38), #12
         c(-Inf, -1.43, -1.02, -0.74, -0.50, -0.29, -0.10,  0.10,  0.29,  0.50, 0.74, 1.02, 1.43), #13
         c(-Inf, -1.47, -1.07, -0.79, -0.57, -0.37, -0.18,  0.00,  0.18,  0.37, 0.57, 0.79, 1.07, 1.47), #14
         c(-Inf, -1.50, -1.11, -0.84, -0.62, -0.43, -0.25, -0.08,  0.08,  0.25, 0.43, 0.62, 0.84, 1.11, 1.5), #15
         c(-Inf, -1.53, -1.15, -0.89, -0.67, -0.49, -0.32, -0.16,  0.00,  0.16, 0.32, 0.49, 0.67, 0.89, 1.15, 1.53), #16
         c(-Inf, -1.56, -1.19, -0.93, -0.72, -0.54, -0.38, -0.22, -0.07,  0.07, 0.22, 0.38, 0.54, 0.72, 0.93, 1.19, 1.56), #17
         c(-Inf, -1.59, -1.22, -0.97, -0.76, -0.59, -0.43, -0.28, -0.14,  0.00, 0.14, 0.28, 0.43, 0.59, 0.76, 0.97, 1.22, 1.59), #18
         c(-Inf, -1.62, -1.25, -1.00, -0.80, -0.63, -0.48, -0.34, -0.20, -0.07, 0.07, 0.20, 0.34, 0.48, 0.63, 0.80, 1.00, 1.25, 1.62), #19
         c(-Inf, -1.64, -1.28, -1.04, -0.84, -0.67, -0.52, -0.39, -0.25, -0.13, 0.00, 0.13, 0.25, 0.39, 0.52, 0.67, 0.84, 1.04, 1.28, 1.64) #20
  )

}

#' Translates an alphabet size value into the corresponding MINDIST distance matrix
#'
#' @param a_size the alphabet size, a value between 2 and 20 (inclusive)
#' @return Returns a MINDIST distance matrix for a given alphabet value
#' @export
#' @examples
#' sax_distance_matrix(5)
sax_distance_matrix <- function(a_size) {
  if (a_size > 1 && a_size <= 20) {
    cutlines <- jmotif::alphabet2cuts(a_size)[2:a_size]
    distance_matrix <- matrix(rep(0, a_size * a_size), byrow = T, nrow = a_size, ncol = a_size)
    i <- 1
    while (i <= a_size) {
      # the min_dist for adjacent symbols are 0, so we start with i+2
      j <- i + 2;
      while (j <= a_size) {
        # square the distance now for future use
        distance_matrix[i,j] <- (cutlines[i] - cutlines[j - 1]) * (cutlines[i] - cutlines[j - 1])
        # the distance matrix is symmetric
        distance_matrix[j,i] <- distance_matrix[i,j]
        j <- j + 1
      }
      i <- i + 1
    }
    distance_matrix
  } else {
    stop(paste("unable to get a distance matrix for the alphabet size",a_size))
  }
}

#' Translates a string to the alphabet index sequence
#'
#' @param idx the letter's index
#' @return Returns a letter at the specified English alphabet's position
#' @export
#' @examples
#' #letter 'b'
#' idx2letter(2)
idx2letter <- function(idx) {
  letters[idx]
}

#' Translates a letter to the alphabet index
#'
#' @param letter the letter to translate
#' @return Returns an index corresponding to the letter.
#' @export
#' @examples
#' # letter 'b' translates to 2
#' letter2idx(2)
letter2idx <- function(letter) {
  which(letters %in% letter)
}

#' Translates a string to the alphabet index sequence
#'
#' @param str the input string
#' @return Returns an index vector corresponding to the string.
#' @export
#' @examples
#' letters2idx(c('a','b','c','a'))
letters2idx <- function(str) {
  as.vector(plyr::aaply(str, 1, jmotif::letter2idx))
}

#' Transforms a time series into a char array
#'
#' @param ts the timeseries
#' @param a_size the alphabet size
#' @return Returns a SAX transform for the time series and the alphabet size.
#' @export
#' @examples
#' y = c(-1, -2, -1, 0, 2, 1, 1, 0)
#' y_paa3 = paa(y, 3)
#' ts2string(y_paa3, 3)
ts2string <- function(ts, a_size) {
  cut_points <- jmotif::alphabet2cuts(a_size)
  len <- 0
  if (is.vector(ts)) {
    len <- length(ts)
  }else if (is.matrix(ts)) {
    len <- ncol(ts)
  }
  res <- rep(0, len)
  for (i in 1:len) {
    res[i] <- length(cut_points[cut_points <= ts[i]])
  }
  jmotif::idx2letter(res)
}

#' Time series PAA transform
#'
#' @param str1 the first string
#' @param str2 the second string
#' @param alphabet_size the used alphabet size
#' @param compression_ratio the distance compression ratio
#' @return Returns the distance between strings
#' @export
#' @examples
#' str1 <- c('a', 'b', 'c')
#' str2 <- c('c', 'b', 'a')
#' min_dist(str1, str2, 3)
min_dist <- function(str1, str2, alphabet_size, compression_ratio = 1) {
  if (length(str1) != length(str2)) {
    stop("error: the strings must have equal length")
  }else{
    if ( any(letters2idx(str1) > alphabet_size) |
        any(letters2idx(str2) > alphabet_size)) {
           stop('error: some symbol(s) in the string(s) exceed(s)
                the alphabet size!');
    }else{
      dist_table <- jmotif::sax_distance_matrix(alphabet_size)
      dist <- 0
      dist <- sqrt(
         compression_ratio *
          sum(diag(dist_table[letters2idx(str1), letters2idx(str2)]) ^ 2)
        )
      dist
    }
  }
}

#' Time series z-normalization
#'
#' @param ts the timeseries
#' @param threshold the normalization threshold value
#' @return Returns a z-normalized time series.
#' @export
#' @examples
#' x = seq(0, pi*4, 0.02)
#' y = sin(x) * 5 + rnorm(length(x))
#' plot(x, y, type="l", col="blue")
#' lines(x, znorm(y, 0.01), type="l", col="red")
znorm <- function(ts, threshold = 0.01) {

  if (is.atomic(ts) && is.matrix(ts)) {

    # check dimensions
    if (1 == dim(ts)[1]) {
      ts_sd <- sd(ts[1,])
      if (ts_sd < threshold) {
        return(unlist(ts[1,], use.names = F))
      }else{
        return(unlist((ts[1,] - mean(ts[1,])) / ts_sd, use.names = F))
      }

    } else if (1 == dim(ts)[2]) {
      ts_sd <- sd(ts[,1])
      if (ts_sd < threshold) {
        return(unlist(ts[,1], use.names = F))
      }else{
        return(unlist((ts[,1] - mean(ts[,1])) / ts_sd, use.names = F))
      }

    } else {
      stop(paste("don't know how to process matrix of dimension",dim(ts)))
    }

  }else if (is.atomic(ts) && is.vector(ts)) {
    ts_sd <- sd(ts)
    if (ts_sd < threshold) {
      return(ts)
    }else{
      return(unlist((ts - mean(ts)) / ts_sd, use.names = F))
    }


  } else {
    stop(sprintf("argument %s must be matrix or array", sQuote("ts")))
  }

}

#' Time series PAA transform
#'
#' @param ts the timeseries
#' @param npoints a desired number of PAA segments
#' @return Returns the PAA transformed input.
#' @export
#' @examples
#' x = c(-1, -2, -1, 0, 2, 1, 1, 0)
#' plot(x,type="l",main="8-points time series and it PAA transform into three points")
#' points(x,pch=16,lwd=5)
#' #segments
#' abline(v=c(1,1+7/3,1+7/3*2,8),lty=3,lwd=2)
paa <- function(ts, npoints) {

  len <- length(ts)

  if (len != npoints) {

    if ( (len %% npoints) == 0 ) {
      res <- jmotif::reshape_cpp(matrix(ts, ncol = 1), len %/% npoints, npoints)
    }else{
      tmp <- matrix(rep(ts, npoints), byrow = T, nrow = npoints)
      res <- jmotif::reshape_cpp(tmp, len, npoints)
    }

    matrix(colMeans(res), nrow = 1, ncol = npoints)
  } else {

    matrix(ts, nrow = 1)

  }

}
