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

test_that("SAX exact #1", {

  sax2 <- sax_via_window(t(dat), 6, 3, 3, "exact", 0.01)

  expect_equal(length(sax2), 46)

  expect_equal(sax2$'0', "bba")
  expect_equal(sax2[[0 + 1]], "bba")

  expect_equal(sax2$'52', "acc")
  expect_equal(sax2[[45]], "acc")

  expect_equal(sax2[['10']], "aac")
  expect_null(sax2[['11']])
  expect_equal(sax2[['12']], "abc")

})

test_that("SAX exact #2", {

  sax4 <- sax_via_window(t(dat), 30, 3, 3, "exact", 0.01)
  expect_equal(length(sax4), 5)
  expect_equal(sax4[['29']], "acb")
  expect_equal(sax4[['30']], "bbb")
})

test_that("SAX mindist #1", {
  sax5 <- sax_via_window(t(dat), 30, 3, 3, "mindist", 0.01)
  expect_equal(length(sax5), 1)
  expect_equal(sax5[['0']], "bbb")

  sax6 <- sax_via_window(t(dat), 30, 6, 5, "mindist", 0.01)
  expect_equal(length(sax6), 8)
  expect_equal(sax6[['26']], "ccccdb")
  expect_equal(sax6[['29']], "caeccc")

})
