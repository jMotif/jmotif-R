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

test_that("wordbag #1", {

  sax1 <- sax_via_window(t(dat), 6, 3, 3, "none", 0.01)
  words <- table(matrix(unlist(sax1), ncol = 1, byrow = T)[,1])

  wb1 <- series_to_wordbag(t(dat), 6, 3, 3, "none", 0.01)

  expect_equal(as.numeric(words[names(words) == "bca"]),
                    wb1[wb1$words == "bca",]$counts)

  expect_equal(as.numeric(words[names(words) == "abc"]),
               wb1[wb1$words == "abc", ]$counts)

  expect_equal(as.numeric(words[names(words) == "bbb"]),
               wb1[wb1$words == "bbb", ]$counts)

  wb2 <- manyseries_to_wordbag(t(t(rbind(dat,dat))), 6, 3, 3, "none", 0.01)

  expect_equal(wb1[wb1$words == "bbb", ]$counts * 2,
               wb2[wb2$words == "bbb", ]$counts)

})


test_that("test wordbag with mindist strategy", {

  sax1 <- sax_via_window(t(dat), 6, 3, 3, "mindist", 0.01)
  words <- table(matrix(unlist(sax1), ncol = 1, byrow = T)[,1])

  wb1 <- series_to_wordbag(t(dat), 6, 3, 3, "mindist", 0.01)

  expect_equal(as.numeric(words[names(words) == "bca"]),
               wb1[wb1$words == "bca",]$counts)

  expect_equal(as.numeric(words[names(words) == "abc"]),
               wb1[wb1$words == "abc", ]$counts)

  expect_equal(as.numeric(words[names(words) == "bbb"]),
               wb1[wb1$words == "bbb", ]$counts)

  wb2 <- manyseries_to_wordbag(t(t(rbind(dat,dat))), 6, 3, 3, "mindist", 0.01)

  expect_equal(wb1[wb1$words == "bbb", ]$counts * 2,
               wb2[wb2$words == "bbb", ]$counts)

})
