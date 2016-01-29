library(plyr)
library(dplyr)

library(cvTools)
library(nloptr)

library(jmotif)

cverror <- function(x) {

  w <- round(x[1], digits = 0)
  p <- round(x[2], digits = 0)
  a <- round(x[3], digits = 0)

  m <- length(train_labels)
  c <- length(unique(train_labels))

  folds <- cvFolds(m, K = nfolds, type = "random")

  errors <- list()
  for (i in c(1:nfolds)) {

    set_test <- which(folds$which == i)
    set_train <- setdiff(1:m, set_test)

    bags <- alply(unique(train_labels),1,function(x){x})
    for (j in 1:c) {
      ll <- which(train_labels[set_train] == unique(train_labels)[j])
      bags[[unique(train_labels)[j]]] <-
                  manyseries_to_wordbag( (train_data[set_train, ])[ll,], w, p, a, "exact", 0.01)
    }
    tfidf = bags_to_tfidf( bags )

    labels_predicted = rep(-1, length(set_test))
    labels_test = train_labels[set_test]
    data_test = train_data[set_test,]

    for (j in c(1:length(labels_predicted))) {
      bag <- NA
      if (length(labels_predicted) > 1) {
        bag <- series_to_wordbag(data_test[j,], w, p, a, "exact", 0.01)
      } else {
        bag <- series_to_wordbag(data_test, w, p, a, "exact", 0.01)
      }
      cosines <- cosine_sim(list("bag" = bag, "tfidf" = tfidf))
      if (!any(is.na(cosines$cosines))) {
       labels_predicted[j] <- which(cosines$cosines == max(cosines$cosines))
      }
    }

    # compute the classification error
    #
    error <- length(which((labels_test != labels_predicted))) / length(labels_test)
    errors[i] <- error

  }

  err <- mean(laply(errors,function(x){x}))

  print(paste(w, p, a, " -> ", err))

  err

}

train_data <- CBF[["data_train"]]
train_labels <- CBF[["labels_train"]]
nfolds <- 30
S <- directL(cverror, rep(c(10, 2, 2)), rep(c(120, 60, 12)),
             nl.info = TRUE, control = list(xtol_rel = 1e-8, maxeval = 30))

train_data <- Gun_Point[["data_train"]]
train_labels <- Gun_Point[["labels_train"]]
nfolds <- 50
S <- directL(cverror, rep(c(10, 2, 2)), rep(c(140, 50, 12)),
             nl.info = TRUE, control = list(xtol_rel = 1e-8, maxeval = 30))
cverror( S$par)

data <- read.table("../../sax-vsm_classic.git/src/resources/data/Beef/Beef_TRAIN")
train_labels <- unlist(data[, 1])
train_data <- matrix(unlist(data[, -1]), nrow = length(train_labels))
nfolds <- 30

S <- directL(cverror, rep(c(10, 2, 2)), rep(c(470, 100, 16)),
             nl.info = TRUE, control = list(xtol_rel = 1e-8, maxeval = 1000))

cverror( S$par)

# 19 40 14
# nloptr.print.options()
ls()
memory.profile()
