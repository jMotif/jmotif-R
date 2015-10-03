str1 = c('a', 'b', 'c')
str2 = c('c', 'b', 'a')
str3 = c('a', 'b', 'b')

test_that("testing ZNorm", {
  expect_equal( sqrt((min_dist(str1, str2, 3))^2 / 2), 0.7396 )
  expect_equal( sqrt((min_dist(str2, str3, 3))^2), 0.7396 )
  expect_equal(min_dist(str1, str3, 3), 0)
  expect_error(min_dist(str1, str3))
  expect_error(min_dist(str1, str3, 2))
})
