a <- c(2, 1, 0, 2, 0, 1, 1, 1)
b <- c(2, 1, 1, 1, 1, 0, 1, 1)

test_that("cosine similarity test", {

  expect_equal(0.1784162, as.numeric(cosine_dist(rbind(a,b))), tolerance = 1e-6)

})
