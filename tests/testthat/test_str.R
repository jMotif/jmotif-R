test_that("idx to letter", {
  expect_true(is.character(idx2letter(3)))
  expect_equal(idx2letter(3), "c")
})
