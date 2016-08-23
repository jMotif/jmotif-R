#
# discord #0 "abdc", at 430 distance to closest neighbor: 5.252618285620532, info string:
#   "position 430, NN distance 5.252618285620532, elapsed time: 79ms, distance calls: 24125"
#
# discord #1 "dabb", at 318 distance to closest neighbor: 4.154825115936453, info string:
#   "position 318, NN distance 4.154825115936453, elapsed time: 64ms, distance calls: 46423"
#
# discord #2 "dbba", at 2080 distance to closest neighbor: 2.381003269560112, info string:
#   "position 2080, NN distance 2.381003269560112, elapsed time: 67ms, distance calls: 55186"
#
# discord #3 "dbca", at 25 distance to closest neighbor: 2.363633774221037, info string:
#   "position 25, NN distance 2.363633774221037, elapsed time: 106ms, distance calls: 46608"

test_that("find discord with HOT SAX", {
  discords <- find_discords_hotsax(ecg0606, 100, 4, 4, 0.01, 4)
  expect_equal(discords[discords$position == 430,]$nn_distance, 5.252618285620532)
  expect_equal(discords[discords$position == 318,]$nn_distance, 4.154825115936453)
  expect_equal(discords[discords$position == 2080,]$nn_distance, 2.381003269560112)
  expect_equal(discords[discords$position == 25,]$nn_distance, 2.363633774221037)
})
