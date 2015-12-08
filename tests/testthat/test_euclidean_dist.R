a <- c(0.5, 0.8, 0.9)
b <- c(-0.15, 0.38, 0.92)
test_dist <- as.numeric(dist(rbind(a,b)))

test_that("early abandoned distance", {
  expect_equal(test_dist, euclidean_dist(a, b))
  expect_error(euclidean_dist(a, c(1,2)))
})
