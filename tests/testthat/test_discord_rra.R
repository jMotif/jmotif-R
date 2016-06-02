test_that("find discord with RRA", {
  discords <- find_discords_rra(ecg0606, 100, 4, 4, "none", 0.01, 4)
  expect_true(discords[1,]$start < 420)
  expect_true(discords[1,]$end > 420)
})
