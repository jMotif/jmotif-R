dat <- read.table(textConnection(
gsub("\n", " ", "0 0 0 0 0 -0.270340178359072 -0.367828308500142 0.666980581124872 1.87088147328446
2.14548907684624 -0.480859313143032 -0.72911654245842 -0.490308602315934 -0.66152028906509
-0.221049033806403 0.367003418871239 0.631073992586373 0.0487728723414486 0.762655178750436
0.78574757843331 0.338239686422963 0.784206454089066 -2.14265084073625 2.11325193044223
0.186018356196443 0 0 0 0 0 0 0 0 0 0 0.519132472499234 -2.604783141655
-0.244519550114012 -1.6570790528784 3.34184602886343 2.10361226260999 1.9796808733979
-0.822247322003058 1.06850578033292 -0.678811824405992 0.804225748913681 0.57363964388698
0.437113583759113 0.437208643628268 0.989892093383503 1.76545983424176 0.119483882364649
-0.222311941138971 -0.74669456611669 -0.0663660879732063 0 0 0 0 0")
), as.is = T)
# plot(as.numeric(dat),t="l")
dat <- as.numeric(dat)

test_that("SAX with NAs", {

  sax1 <- sax_via_window(dat, 6, 3, 3, "none", 0.01)
  expect_equal(length(sax1), 55)

  dat2 <- dat
  dat2[60] <- NA
  sax2 <- sax_via_window(dat2, 6, 3, 3, "none", 0.01)
  expect_equal(length(sax2), 54)

  dat3 <- dat
  dat3[50] <- NA
  sax3 <- sax_via_window(dat3, 6, 3, 3, "none", 0.01)
  expect_equal(length(sax3), 44)

  expect_warning(sax_via_window(dat3, 6, 3, 3, "none", 0.01))
  expect_error(sax_via_window(dat, 70, 3, 3, "none", 0.01))

})

test_that("SAX with NAs #2, sax-vsm bags", {
  w <- 60 # the sliding window size
  p <- 6  # the PAA size
  a <- 6  # the SAX alphabet size

    cylinders <- CBF[["data_train"]][CBF[["labels_train"]] == 1,]

    c <- cylinders[2,]
  saxc <- series_to_wordbag(c, w, p, a, "none", 0.01)
  cd <- c
  cd[128] <- NA
  saxcd <- series_to_wordbag(cd, w, p, a, "none", 0.01)

  expect_equal(sum(saxc$counts), 69)
  expect_equal(sum(saxcd$counts), 68)

  cd[100] <- NA
  saxcd <- series_to_wordbag(cd, w, p, a, "none", 0.01)
  expect_equal(sum(saxcd$counts), 40)

})
