test_that("find discord with HOT SAX", {
  discords <- find_discords_hotsax(ecg0606, 100, 4, 4, 0.01, 4)
  expect_equal(discords[discords$position == 411,]$nn_distance, 1.5045847, tolerance = 1e-6)
  expect_equal(discords[discords$position == 37,]$nn_distance, 0.4787745, tolerance = 1e-6)
  expect_equal(discords[discords$position == 1566,]$nn_distance, 0.4437060, tolerance = 1e-6)
  expect_equal(discords[discords$position == 539,]$nn_distance, 0.4437060, tolerance = 1e-6)
})
