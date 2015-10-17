# load Cylinder-Bell-Funnel data
data("CBF")

# set parameters
w <- 60
p <- 6
a <- 6

# convert data to wordbags, dataset has three labels: 1, 2, 3
bag1 <- manyseries_to_wordbag(CBF[["data_train"]][CBF[["labels_train"]] == 1,],
                              w, p, a, "exact", 0.01)
bag2 <- manyseries_to_wordbag(CBF[["data_train"]][CBF[["labels_train"]] == 2,],
                              w, p, a, "exact", 0.01)
bag3 <- manyseries_to_wordbag(CBF[["data_train"]][CBF[["labels_train"]] == 3,],
                              w, p, a, "exact", 0.01)

# compute tf*idf
tfidf = bags_to_tfidf(
          list("cylinder" = bag1, "bell" = bag2, "funnel" = bag3) )

# classify the test data
labels_predicted = rep(-1, length(CBF[["labels_test"]]))
labels_test = CBF[["labels_test"]]
data_test = CBF[["data_test"]]
for (i in c(1:length(data_test[,1]))) {
  series = data_test[i,]
  bag = series_to_wordbag(series, w, p, a, "exact", 0.01)
  cosines = cosine_sim(list("bag"=bag, "tfidf" = tfidf))
  labels_predicted[i] = which(cosines$cosines == max(cosines$cosines))
}
error = length(which((labels_test != labels_predicted))) / length(labels_test)
error

which((labels_test != labels_predicted))
