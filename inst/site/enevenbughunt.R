#install.packages("devtools")
#library(devtools)
#install_github('jMotif/jmotif-R')
#
library(jmotif)
str(CBF)
#
w <- 60 # the sliding window size
p <- 6  # the PAA size
a <- 6  # the SAX alphabet size
#
cylinders <- CBF[["data_train"]][CBF[["labels_train"]] == 1,]
str(cylinders)
# alter few of these
c <- cylinders[2,]
saxc <- series_to_wordbag(c, w, p, a, "none", 0.01)
cd <- c
cd[128] <- NA
saxcd <- series_to_wordbag(cd, w, p, a, "none", 0.01)
sum(saxc$counts)
sum(saxcd$counts)
#
cylinder <- manyseries_to_wordbag(cylinders, w, p, a, "exact", 0.01)
#cylinder <- manyseries_to_wordbag(CBF[["data_train"]][CBF[["labels_train"]] == 1,], w, p, a, "exact", 0.01)
bell <- manyseries_to_wordbag(CBF[["data_train"]][CBF[["labels_train"]] == 2,], w, p, a, "exact", 0.01)
funnel <- manyseries_to_wordbag(CBF[["data_train"]][CBF[["labels_train"]] == 3,], w, p, a, "exact", 0.01)
#
tfidf = bags_to_tfidf( list("cylinder" = cylinder, "bell" = bell, "funnel" = funnel) )
#
labels_predicted = rep(-1, length(CBF[["labels_test"]]))
labels_test = CBF[["labels_test"]]
data_test = CBF[["data_test"]]
for (i in c(1:length(data_test[,1]))) {
  series = data_test[i,]
  bag = series_to_wordbag(series, w, p, a, "exact", 0.01)
  cosines = cosine_sim(list("bag"=bag, "tfidf" = tfidf))
  labels_predicted[i] = which(cosines$cosines == max(cosines$cosines))
}

# compute the classification error
#
error = length(which((labels_test != labels_predicted))) / length(labels_test)
error

# findout which time series were misclassified
#
which((labels_test != labels_predicted))
#
#
#
library(lineprof)
library(microbenchmark)
microbenchmark(
  series_to_wordbag(c, w, p, a, "none", 0.01),
  series_to_wordbag(c, w, p, a, "exact", 0.01)
)
