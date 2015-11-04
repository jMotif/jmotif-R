X <- c(-1, -2, -1, 0, 2, 1, 1, 0)
Xscaled <- X / 100

test_that("testing ZNorm computation", {

  expect_equal(sd(znorm(X)), 1)
  expect_equal(mean(znorm(X)), 0)

  expect_equal(sd(znorm(t(X))), 1)
  expect_equal(mean(znorm(t(X))), 0)

  expect_equal(sd(znorm(t(t(X)))), 1)
  expect_equal(mean(znorm(t(t(X)))), 0)

  expect_equal(mean(znorm(t(t(X)))), 0)


  expect_equal(znorm(Xscaled, 0.1), Xscaled)

  expect_error(znorm(list("c", "d")))

})

dat <- c(2.02,2.33,2.99,6.85,9.2,8.8,7.5,6,5.85,3.85,4.85,3.85,2.22,1.45,1.34)
dat_znorm <- c(-0.9796808,-0.8622706,-0.6123005,0.8496459,1.739691,1.588194,
               1.095829,0.5277147,0.4709033,-0.2865819,0.0921607,-0.2865819,
               -0.9039323,-1.195564,-1.237226)

test_that("test a concrete znorm example (cross check with my java code) via CPP", {
  expect_equal(znorm(dat), dat_znorm, tolerance = 1e-6)
})

test_that("test a znorm clone example", {
  expect_equal(znorm(dat, 3.0), dat, tolerance = 1e-6)
})

dat <- c(0.5,1.29,2.58,3.83,3.25,4.25,3.83,5.63,6.44,6.25,8.75,8.83,3.25,0.75,0.72)
dat_znorm <- c(-1.289433,-0.9992189,-0.5253246,-0.06612478,-0.2791935,0.08816637,
               -0.06612478,0.595123,0.8926845,0.8228861,1.741286,1.770675,
               -0.2791935,-1.197593,-1.208614)
test_that("test a second concrete znorm example (cross check with my java code) via CPP", {
  expect_equal(znorm(dat), dat_znorm, tolerance = 1e-6)
})
