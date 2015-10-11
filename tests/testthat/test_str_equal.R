test_that("test str equality", {
  expect_true(is_equal_str("", ""))
  expect_true(is_equal_str("asfw", "asfw"))
  expect_true(is_equal_str("asfw", "asfw"))

  expect_false(is_equal_str("asfwd", "asfw"))
  expect_false(is_equal_str("", "-1"))
  expect_false(is_equal_str("asfw", "atfw"))
})
