#discord #0 "#0", at 411 distance to closest neighbor: 1.5045846602966542, info string: "elapsed time: 1s699ms, distance calls: 4403702"
#discord #1 "#1", at 37 distance to closest neighbor: 0.4787744771810631, info string: "elapsed time: 1s312ms, distance calls: 4004102"
#discord #2 "#2", at 1566 distance to closest neighbor: 0.44370598373247144, info string: "elapsed time: 1s199ms, distance calls: 3725326"
#discord #3 "#3", at 539 distance to closest neighbor: 0.44370598373247144, info string: "elapsed time: 1s94ms, distance calls: 3325726"
#discord #4 "#4", at 188 distance to closest neighbor: 0.41770204691861385, info string: "elapsed time: 980ms, distance calls: 3069982"
test_that("find discord with HOT SAX", {
  discords <- find_discords_hot_sax(ecg0606, 100, 4, 4, 0.01, 4)
  expect_equal(discords[discords$position == 411,]$nn_distance, 1.5045846602966542)
  expect_equal(discords[discords$position == 37,]$nn_distance, 0.4787744771810631)
  expect_equal(discords[discords$position == 539,]$nn_distance, 0.44370598373247144)
  expect_equal(discords[discords$position == 1566,]$nn_distance, 0.44370598373247144)
})
