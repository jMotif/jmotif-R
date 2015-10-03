test_that("points to letters", {
  expect_equal(ts2string(c(-1.375, 0.75, 0.625), 3), c('a', 'c', 'c'))
})
