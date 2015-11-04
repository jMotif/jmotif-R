m3 <- sax_distance_matrix(3)

test_that("testing Distance Matrix", {
  expect_equal(m3[1, 1], 0)
  expect_equal(m3[1, 3], 0.7396 )
  expect_error(sax_distance_matrix(0))
  expect_error(sax_distance_matrix(21))
})

str1 <- c('a', 'b', 'c')
str2 <- c('c', 'b', 'a')
str3 <- c('a', 'b', 'b')

test_that("testing min_dist", {

  expect_equal( sqrt((min_dist(str1, str2, 3)) ^ 2 / 2), 0.7396 )

  expect_equal( sqrt((min_dist(str2, str3, 3)) ^ 2), 0.7396 )

  expect_equal(min_dist(str1, str3, 3), 0)

  expect_error(min_dist(str1, str3, 2))

  expect_error(min_dist(str1, str3[1:2], 2))
})
