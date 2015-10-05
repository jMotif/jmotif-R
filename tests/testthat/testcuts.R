test_that("testing the cuts points", {
  expect_error(alphabet2cuts(0))
  expect_error(alphabet2cuts(33))
  expect_equal(length(alphabet2cuts(3)), 3)
  expect_equal(alphabet2cuts(4)[3], 0)
})
