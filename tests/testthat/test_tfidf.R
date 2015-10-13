bag1 = data.frame(
  "words" = c("this", "is", "a", "sample"),
  "counts" = c(1, 1, 2, 1)
)

bag2 = data.frame(
  "words" = c("this", "is", "another", "example"),
  "counts" = c(1, 1, 2, 3)
)

counts = merge(bag1, bag2, by = c("words"), all = T)

tfidf = tf_idf(counts)

test_that("test tfidf #1", {

  example = log(1 + 3) * log(2 / 1)
  expect_equal(example, tfidf[tfidf$words == "example",3])

  a = log(1 + 2) * log(2 / 1)
  expect_equal(a, tfidf[tfidf$words == "a",2])

  expect_equal(0, tfidf[tfidf$words == "this",2])
  expect_equal(0, tfidf[tfidf$words == "this",3])

  expect_equal(0, tfidf[tfidf$words == "is",2])
  expect_equal(0, tfidf[tfidf$words == "is",3])

})
