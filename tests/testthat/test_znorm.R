X <- c(-1, -2, -1, 0, 2, 1, 1, 0)
Xscaled <- as.numeric(X) / 100  # ensure it's a numeric vector

test_that("testing ZNorm computation", {

  # Basic ZNorm: mean 0, sd 1
  z <- znorm(X)
  expect_equal(sd(z), 1, tolerance = 1e-8)
  expect_equal(mean(z), 0, tolerance = 1e-8)

  # ZNorm with small threshold returns original vector
  expect_equal(znorm(Xscaled, 0.1), Xscaled, tolerance = 1e-8)

  # Invalid input triggers an error
  # expect_error(znorm(list("c", "d")))
})

test_that("concrete znorm examples match expected values", {

  # First example
  dat1 <- c(2.02, 2.33, 2.99, 6.85, 9.2, 8.8, 7.5, 6, 5.85, 3.85, 4.85, 3.85, 2.22, 1.45, 1.34)
  dat1_znorm <- c(-0.9796808, -0.8622706, -0.6123005, 0.8496459, 1.739691, 1.588194,
                  1.095829, 0.5277147, 0.4709033, -0.2865819, 0.0921607, -0.2865819,
                  -0.9039323, -1.195564, -1.237226)
  expect_equal(znorm(dat1), dat1_znorm, tolerance = 1e-6)

  # Test cloning behavior: threshold higher than stdev
  expect_equal(znorm(dat1, 3.0), dat1, tolerance = 1e-6)

  # Second example
  dat2 <- c(0.5, 1.29, 2.58, 3.83, 3.25, 4.25, 3.83, 5.63, 6.44, 6.25, 8.75, 8.83, 3.25, 0.75, 0.72)
  dat2_znorm <- c(-1.289433, -0.9992189, -0.5253246, -0.06612478, -0.2791935, 0.08816637,
                  -0.06612478, 0.595123, 0.8926845, 0.8228861, 1.741286, 1.770675,
                  -0.2791935, -1.197593, -1.208614)
  expect_equal(znorm(dat2), dat2_znorm, tolerance = 1e-6)

})

