test_that("points to letters, i.e. SAX", {
  expect_equal(ts2string(c(-1.375, 0.75, 0.625), 3), c('a', 'c', 'c'))
  expect_equal(ts2string(t(c(-1.375, 0.75, 0.625)), 3), c('a', 'c', 'c'))
})

test_that("points to letters, i.e. SAX, CPP", {
  expect_equal(ts2chars_cpp(c(-1.375, 0.75, 0.625), 3), c('a', 'c', 'c'))
  expect_equal(strsplit(ts2string_cpp(c(-1.375, 0.75, 0.625), 3), "")[[1]],
               c("a", "c", "c"))
})
