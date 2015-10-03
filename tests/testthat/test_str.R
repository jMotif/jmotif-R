test_that("idx to letter", {
  expect_true(is.character(idx2letter(3)))
  expect_equal(idx2letter(3), "c")
})

test_that("letter to idx", {
  expect_true(is.numeric(letter2idx('c')))
  expect_equal(letter2idx('c'), 3)
})

test_that("letters to idx", {
  expect_true(is.numeric(letters2idx(c('a','b','c','a'))))
  expect_true(is.vector(letters2idx(c('a','b','c','a'))))
  expect_equal(length(letters2idx(c('a','b','c','a'))), 4)
  expect_equal(letters2idx(c('a','b','c','a'))[3], 3)
})

