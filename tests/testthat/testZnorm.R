X = c(-1, -2, -1, 0, 2, 1, 1, 0)
Xscaled = X/100

test_that("testing ZNorm", {

  expect_equal(sd(znorm(X)), 1)
  expect_equal(mean(znorm(X)), 0)

  expect_equal(sd(znorm(t(X))), 1)
  expect_equal(mean(znorm(t(X))), 0)

  expect_equal(sd(znorm(t(t(X)))), 1)
  expect_equal(mean(znorm(t(t(X)))), 0)

  expect_equal(mean(znorm(t(t(X)))), 0)


  expect_equal(znorm(Xscaled, 0.1), Xscaled)
  expect_equal(znorm(t(Xscaled), 0.1), Xscaled)
  expect_equal(znorm(t(t(Xscaled)), 0.1), Xscaled)

  expect_error(znorm(matrix(X, ncol = 2)))

  expect_error(znorm(list("c", "d")))

})
