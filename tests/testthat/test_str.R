test_that("idx to letter", {
  expect_true(is.character(idx_to_letter(3)))
  expect_equal(idx_to_letter(3), "c")
})

test_that("letter to idx", {
  expect_true(is.numeric(letter_to_idx('c')))
  expect_equal(letter_to_idx('c'), 3)
})

test_that("letters to idx", {
  expect_true(is.numeric(letters_to_idx(c('a', 'b', 'c', 'a'))))
  expect_true(is.vector(letters_to_idx(c('a', 'b', 'c', 'a'))))
  expect_equal(length(letters_to_idx(c('a', 'b', 'c', 'a'))), 4)
  expect_equal(letters_to_idx(c('a', 'b', 'c', 'a'))[3], 3)
})
