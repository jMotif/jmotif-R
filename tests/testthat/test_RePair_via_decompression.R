sax_string = paste(
              unlist(sax_via_window(ecg0606, 160, 4, 4, "none", 0.001)),
              collapse = " "
             )

test_that("find discord with HOT SAX", {

  grammar <- str_to_repair_grammar(sax_string)

  uncompressed_R0 = paste(grammar[[1]]$rule_string, " ", sep = "")

  while (grepl("R", uncompressed_R0)) {
    for (i in c(2:length(grammar))) {
      uncompressed_R0 = gsub(
        paste(" R", i - 1, " ", sep = ""),
        paste(" ", grammar[[i]]$expanded_rule_string, " ", sep = ""),
        uncompressed_R0
      )
    }
  }

  expect_equal(sax_string, sub("\\s+$", "", uncompressed_R0))

})
