y <- c(-1, -2, -1, 0, 2, 1, 1, 0)
v <- t(c(-1.375, 0.75, 0.625))

test_that("points to points via PAA", {
  expect_equal(paa(y, 3), v)
  expect_equal(paa(y, 8), t(y))
  expect_equal(paa(y, 2), t(c(-1, 1)))
})
