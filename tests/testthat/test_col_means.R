a <- matrix(rnorm(100), ncol = 20, nrow = 5)
test_that("colmeans", {
  expect_equal(colMeans(a), col_means(a))
})
