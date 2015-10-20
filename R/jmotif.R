#' @useDynLib jmotif
#' @importFrom Rcpp sourceCpp
NULL

#' Generates a SAX MinDist distance matrix (i.e. the "lookup table") for a given alphabet size.
#'
#' @param a_size the desired alphabet size (a value between 2 and 20, inclusive)
#' @return Returns a distance matrix (for SAX minDist) for a specified alphabet size
#' @export
#' @references Lonardi, S., Lin, J., Keogh, E., Patel, P.,
#' Finding motifs in time series.
#' In Proc. of the 2nd Workshop on Temporal Data Mining (pp. 53-68).
#' @examples
#' sax_distance_matrix(5)
sax_distance_matrix <- function(a_size) {
  if (a_size > 1 && a_size <= 20) {
    cutlines <- jmotif::alphabet_to_cuts(a_size)[2:a_size]
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

#' Computes the mindist value for two strings
#'
#' @param str1 the first string
#' @param str2 the second string
#' @param alphabet_size the used alphabet size
#' @param compression_ratio the distance compression ratio
#' @return Returns the distance between strings
#' @export
#' @references Lonardi, S., Lin, J., Keogh, E., Patel, P.,
#' Finding motifs in time series.
#' In Proc. of the 2nd Workshop on Temporal Data Mining (pp. 53-68).
#' @examples
#' str1 <- c('a', 'b', 'c')
#' str2 <- c('c', 'b', 'a')
#' min_dist(str1, str2, 3)
min_dist <- function(str1, str2, alphabet_size, compression_ratio = 1) {
  if (length(str1) != length(str2)) {
    stop("error: the strings must have equal length")
  }else{
    if ( any(letters_to_idx(str1) > alphabet_size) |
        any(letters_to_idx(str2) > alphabet_size)) {
           stop('error: some symbol(s) in the string(s) exceed(s)
                the alphabet size!');
    }else{
      dist_table <- sax_distance_matrix(alphabet_size)
      dist <- 0
      dist <- sqrt(
         compression_ratio *
          sum(diag(dist_table[letters_to_idx(str1), letters_to_idx(str2)]) ^ 2)
        )
      dist
    }
  }
}

#' Computes the cosine similarity between numeric vectors
#'
#' @param m the data matrix
#' @return Returns the cosine similarity
#' @export
#' @examples
#' a <- c(2, 1, 0, 2, 0, 1, 1, 1)
#' b <- c(2, 1, 1, 1, 1, 0, 1, 1)
#' sim <- cosine_dist(rbind(a,b))
cosine_dist <- function(m) {
  stats::as.dist(1 - m %*% t(m) / (sqrt(rowSums(m ^ 2) %*% t(rowSums(m ^ 2)))))
}
