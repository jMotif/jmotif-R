a <- c(0.5, 0.8, 0.9)
b <- c(-0.15, 0.38, 0.92)
test_dist <- as.numeric(dist(rbind(a,b)))

test_that("early abandoned distance", {
  expect_equal(test_dist, early_abandoned_dist(a, b, 1))
  expect_error(early_abandoned_dist(a, c(1,2), 1))
  expect_true(is.nan(early_abandoned_dist(a, b, 0.01)))
})
