test_that("testing the cuts points", {
  expect_error(alphabet2cuts(0))
  expect_error(alphabet2cuts(33))
  expect_equal(length(alphabet2cuts(3)), 3)
  expect_equal(alphabet2cuts(4)[3], 0)
  for(i in c(2:20)){
    cuts = alphabet2cuts(i)
    expect_equal(cuts[1], -Inf)
    expect_equal(length(cuts), i)
    expect_true(cuts[length(cuts)] < 2.0)
  }
})

test_that("testing the cuts points, CPP", {
  expect_error(alphabet2cuts_cpp(0))
  expect_error(alphabet2cuts_cpp(33))
  expect_equal(length(alphabet2cuts_cpp(3)), 3)
  expect_equal(alphabet2cuts_cpp(4)[3], 0)
  for(i in c(2:20)){
    cuts = alphabet2cuts_cpp(i)
    expect_equal(cuts[1], -Inf)
    expect_equal(length(cuts), i)
    expect_true(cuts[length(cuts)] < 2.0)
  }
})
