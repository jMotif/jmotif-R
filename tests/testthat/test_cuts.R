test_that("testing the cuts points", {
  expect_error(alphabet_to_cuts(0))
  expect_error(alphabet_to_cuts(33))
  expect_equal(length(alphabet_to_cuts(3)), 3)
  expect_equal(alphabet_to_cuts(4)[3], 0)
  for (i in c(2:20)) {
    cuts <- alphabet_to_cuts(i)
    expect_equal(cuts[1], -Inf)
    expect_equal(length(cuts), i)
    expect_true(cuts[length(cuts)] < 2.0)
  }
})
