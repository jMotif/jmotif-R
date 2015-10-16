data("CBF")

#
w <- 60
p <- 20
a <- 6

# convert these to word bags
#
unique(CBF[["labels_train"]])
bag1 <- manyseries_to_wordbag(CBF[["data_train"]][CBF[["labels_train"]] == 1,], w, p, a, "exact", 0.01)
bag2 <- manyseries_to_wordbag(CBF[["data_train"]][CBF[["labels_train"]] == 2,], w, p, a, "exact", 0.01)
bag3 <- manyseries_to_wordbag(CBF[["data_train"]][CBF[["labels_train"]] == 3,], w, p, a, "exact", 0.01)

# compute tf*idf
#
str(bag1)
ll = list("1" = bag1, "aa2" = bag2, "3" = bag3)

bag1 = data.frame(words = c("aaa","bbb","ccc"), counts = c(1,2,0), stringsAsFactors = FALSE)
bag2 = data.frame(words = c("aaa","bbb","ccc"), counts = c(3,0,5), stringsAsFactors = FALSE)
bag3 = data.frame(words = c("aaa","bbb","ccc"), counts = c(0,1,1), stringsAsFactors = FALSE)
ll = list("1bb" = bag1, "aa2" = bag2, "3cc" = bag3)
tfidf = bags_to_tfidf( ll )
tfidf




str(bag1)
merge_word_bags(bag1, bag2, bag3, by = c("words"), all = T)
tfidf = tf_idf(merge(bag1, bag2, bag3, by = c("words"), all = T))

# classify the test data
#
predicted = rep(-1, length(CBF[["labels_test"]]))
test = CBF[["labels_test"]]

for (i in c(1:length(test[,1]))) {
  series = test[i,]
  bag = series_to_wordbag(series, w, p, a, "exact", 0.01)
  mm = merge(tfidf, bag, by = c("words"), all = T)
  mm[is.na(mm)] <- 0.0
  cosine[1] = cosineSim(rbind(mm$`1`,mm$counts))
  cosine[2] = cosineSim(rbind(mm$`2`,mm$counts))
  cosine[3] = cosineSim(rbind(mm$`2`,mm$counts))
  prediction = ifelse(cosine1 < cosine2, 1, 2)
  predicted[i] = prediction
}

error = length(which((test_labels != predicted))) / length(test_labels)

error

which((test_labels != predicted))

#
#
#
#
#

series = test[23, -1]

bag = series_to_wordbag(unlist(series), w, p, a, "exact", 0.01)
mm = merge(tfidf, bag, by = c("words"), all = T)
weights = mm[!(is.na(mm$counts)),]
weights[is.na(weights)] <- 0.0
weights = weights[rowSums(weights[,2:3]) != 0, ]

sax = list_to_df(sax_via_window(unlist(series), w, p, a, "exact", 0.01))
names(sax) <- c("index","word")

patterns = data.frame(0,0,0,0)
for (i in c(1:length(weights[,1]))) {
  word = weights[i,1]
  if (word %in% sax$word) {
    patterns = rbind(patterns,
        c(word, sax[sax$word == word,1], tfidf[tfidf$words == word, 2],tfidf[tfidf$words == word, 3]))
  }
}
patterns = patterns[-1,]
names(patterns) <- c("pattern", "pos", "weight1", "weight2")

i=23

list_to_df <- function(list_for_df) {
  list_for_df <- as.list(list_for_df)

  nm <- names(list_for_df)
  if (is.null(nm))
    nm <- seq_along(list_for_df)

  df <- data.frame(name = nm, stringsAsFactors = FALSE)
  df$value <- unname(list_for_df)
  df
}
