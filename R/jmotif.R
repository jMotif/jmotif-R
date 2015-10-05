#
#   Build and Reload Package:  'Ctrl + Shift + B'
#   Check Package:             'Ctrl + Shift + E'
#   Test Package:              'Ctrl + Shift + T'

letters <- c("a",  "b",  "c",  "d",  "e", "f",  "g",  "h",  "i",  "j",
             "k",  "l",  "m",  "n",  "o", "p",  "q",  "r",  "s",  "t",
             "u",  "v",  "w",  "x",  "y",  "z")

##
## Converts the given alphabet size into a set of cut points (lines)
##
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

##
## computes distance matrix for the alphabet size specified
##
sax_distance_matrix <- function(a_size) {
  if (a_size > 1 && a_size <= 20) {
    cutlines <- alphabet2cuts(a_size)[2:a_size]
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

##
## Converts the specified resolution into the cut points
##
idx2letter <- function(idx) {
  letters[idx]
}

##
## Translates a letter to the alphabet index
##
letter2idx <- function(letter) {
  which(letters %in% letter)
}

##
## Converts the specified resolution into the cut points
##
letters2idx <- function(str) {
  as.vector(plyr::aaply(str, 1, letter2idx))
}

##
## Converts the timeseries into string
##
ts2string <- function(ts, a_size) {
  cut_points <- alphabet2cuts(a_size)
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
  idx2letter(res)
}

##
## compute distance between strings
##
min_dist <- function(str1, str2, alphabet_size, compression_ratio = 1) {
  if (length(str1) != length(str2)) {
    stop("error: the strings must have equal length")
  }else{
    if ( any(letters2idx(str1) > alphabet_size) |
        any(letters2idx(str2) > alphabet_size)) {
           stop('error: some symbol(s) in the string(s) exceed(s)
                the alphabet size!');
    }else{
      dist_table <- sax_distance_matrix(alphabet_size)
      dist <- 0
      dist <- sqrt(
         compression_ratio *
          sum(diag(dist_table[letters2idx(str1), letters2idx(str2)]) ^ 2)
        )
      dist
    }
  }
}

##
## time series normalization
##
znorm <- function(ts, threshold = 0.01) {

  if (is.atomic(ts) && is.matrix(ts)) {

    # check dimensions
    if (1 == dim(ts)[1]) {
      ts_sd <- sd(ts[1,])
      if (ts_sd < threshold) {
        return(unlist(ts[1,], use.names = F))
      }else{
        return(unlist((ts[1,] - mean(ts[1,]))/ts_sd, use.names = F))
      }

    } else if (1 == dim(ts)[2]) {
      ts_sd <- sd(ts[,1])
      if (ts_sd < threshold) {
        return(unlist(ts[,1], use.names = F))
      }else{
        return(unlist((ts[,1] - mean(ts[,1]))/ts_sd, use.names = F))
      }

    } else {
      stop(paste("don't know how to process matrix of dimension",dim(ts)))
    }

  }else if (is.atomic(ts) && is.vector(ts)) {
    ts_sd <- sd(ts)
    if (ts_sd < threshold) {
      return(ts)
    }else{
      return(unlist((ts - mean(ts))/ts_sd, use.names = F))
    }


  } else {
    stop(sprintf("argument %s must be matrix or array", sQuote("ts")))
  }

}

paa <- function(ts, npoints) {

  len <- length(ts)

  if (len != npoints) {

    if ( (len %% npoints) == 0 ) {
      res <- matlab::reshape(matrix(ts, ncol = 1), c(len %/% npoints, npoints))
    }else{
      tmp <- matrix(rep(ts, npoints), byrow = T, nrow = npoints)
      res <- matlab::reshape(tmp, len, npoints)
    }

    matrix(colMeans(res), nrow = 1, ncol = npoints)
  } else {

    matrix(ts, nrow = 1)

  }

}
