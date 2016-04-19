str <- "abc abc cba cba bac xxx abc abc cba cba bac"

test_that("RePair test #1", {

  grammar <- str_to_repair_grammar(str)

  expect_equal("R4 xxx R4", grammar[[1]]$rule_string)

  expect_equal("R3", grammar[[4]]$rule_string)

})
