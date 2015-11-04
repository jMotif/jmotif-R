test_that("testing is_equal_mindist", {

  expect_true( is_equal_mindist("aaa", "bbb")  )

  expect_false( is_equal_mindist("aaa", "bbba")  )

  expect_false( is_equal_mindist("aaa", "aca")  )

})
