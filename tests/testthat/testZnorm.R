X = c(-1, -2, -1, 0, 2, 1, 1, 0)
test_that("testing ZNorm", {
  expect_equal(sd(znorm(X)), 1)
  expect_equal(mean(znorm(X)), 0)
})
