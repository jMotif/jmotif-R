# set the discretization parameters
#
w <- 60 # the sliding window size
p <- 6  # the PAA size
a <- 6  # the SAX alphabet size

# convert the train classes to wordbags (the dataset has three labels: 1, 2, 3)
#
cylinder <- manyseries_to_wordbag(CBF[["data_train"]][CBF[["labels_train"]] == 1, ],
                                  w, p, a, "exact", 0.01)
bell <- manyseries_to_wordbag(CBF[["data_train"]][CBF[["labels_train"]] == 2, ],
                              w, p, a, "exact", 0.01)
funnel <- manyseries_to_wordbag(CBF[["data_train"]][CBF[["labels_train"]] == 3, ],
                                w, p, a, "exact", 0.01)

# compute tf*idf weights for three bags
#
tfidf <- bags_to_tfidf(list("cylinder" = cylinder, "bell" = bell, "funnel" = funnel))

# classify the test data
#
# init the vars
labels_predicted <- rep(-1, length(CBF[["labels_test"]]))
labels_test <- CBF[["labels_test"]]
data_test <- CBF[["data_test"]]

# assign labels using the cosine similarity
#
for (i in c(seq_len(length(data_test[, 1])))) {
  series <- data_test[i, ]
  bag <- series_to_wordbag(series, w, p, a, "exact", 0.01)
  cosines <- cosine_sim(list("bag" = bag, "tfidf" = tfidf))
  labels_predicted[i] <- which(cosines$cosines == max(cosines$cosines))
}

# compute the classification error
#
error <- length(which(labels_test != labels_predicted)) / length(labels_test)
error

# findout which time series were misclassified
#
which(labels_test != labels_predicted)
