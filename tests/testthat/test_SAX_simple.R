test_that("points to letters, i.e. SAX", {
  expect_equal(series_to_chars(c(-1.375, 0.75, 0.625), 3), c('a', 'c', 'c'))
  expect_equal(strsplit(series_to_string(c(-1.375, 0.75, 0.625), 3), "")[[1]],
               c("a", "c", "c"))
})

dat1 <- c(2.02, 2.33, 2.99, 6.85, 9.2, 8.8, 7.5, 6, 5.85,
          3.85, 4.85, 3.85, 2.22, 1.45, 1.34)

dats1_9_7 <- "bcggfddba"
dats1_10_11 <- "bcjkiheebb"
dats1_14_10 <- "bcdijjhgfeecbb"

test_that("points to letters, i.e. SAX, CPP #2", {
  str1_10_11 <- paste( matrix(unlist(sax_by_chunking(dat1, 10, 11, 0.01)),
                             nrow = 10, byrow = T)[,1], collapse = "")
  str1_14_10 <- paste( matrix(unlist(sax_by_chunking(dat1, 14, 10, 0.01)),
                             nrow = 14, byrow = T)[,1], collapse = "")
  str1_9_7 <- paste( matrix(unlist(sax_by_chunking(dat1, 9, 7, 0.01)),
                             nrow = 9, byrow = T)[,1], collapse = "")
  expect_equal(str1_10_11, dats1_10_11)
  expect_equal(str1_14_10, dats1_14_10)
  expect_equal(str1_9_7, dats1_9_7)
})

dat2 <- c(0.5, 1.29, 2.58, 3.83, 3.25, 4.25, 3.83, 5.63, 6.44, 6.25, 8.75,
          8.83, 3.25, 0.75, 0.72)

dats2_9_7 <- "accdefgda"
dats2_10_11 <- "bcefgijkdb"
dats2_14_10 <- "bbdeeffhijjfbb"

test_that("points to letters, i.e. SAX, CPP #3", {
  str2_10_11 <- paste( matrix(unlist(sax_by_chunking(dat2, 10, 11, 0.01)),
                             nrow = 10, byrow = T)[,1], collapse = "")
  str2_14_10 <- paste( matrix(unlist(sax_by_chunking(dat2, 14, 10, 0.01)),
                             nrow = 14, byrow = T)[,1], collapse = "")
  str2_9_7 <- paste( matrix(unlist(sax_by_chunking(dat2, 9, 7, 0.01)),
                           nrow = 9, byrow = T)[,1], collapse = "")
  expect_equal(str2_10_11, dats2_10_11)
  expect_equal(str2_14_10, dats2_14_10)
  expect_equal(str2_9_7, dats2_9_7)
})
