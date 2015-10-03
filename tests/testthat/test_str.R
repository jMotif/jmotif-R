test_that("idx to letter", {
  expect_true(is.character(idx2letter(3)))
  expect_equal(idx2letter(3), "c")
  expect_true(is.numeric(letter2idx('c')))
  expect_equal(letter2idx('c'), 3)

})
