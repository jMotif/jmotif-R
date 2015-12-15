library(cvTools)


w=60
p=6
a=6

cverror = function( w, p, a ) {

  n_folds = 15
  folds = cvFolds(30, K = n_folds, type = "random")

  for(i in c(1:n_folds)){

    set_test = which(folds$which==i)
    set_train = setdiff(1:30, set_test)

    CBF[["labels_train"]]

    cyl_idx = which(dtrain[,1]==1)
    bel_idx = which(CBF[["labels_train"]][set_train]==2)
    fun_idx = which(CBF[["labels_train"]][set_train]==3)

    cylinder <- manyseries_to_wordbag(CBF[["data_train"]][cyl_idx,], w, p, a, "exact", 0.01)
    bell <- manyseries_to_wordbag(CBF[["data_train"]][bel_idx,], w, p, a, "exact", 0.01)
    funnel <- manyseries_to_wordbag(CBF[["data_train"]][fun_idx,], w, p, a, "exact", 0.01)

    tfidf = bags_to_tfidf( list("cylinder" = cylinder, "bell" = bell, "funnel" = funnel) )

    labels_predicted = rep(-1, length(set_test))
    labels_test = CBF[["labels_train"]][set_test]
    data_test = CBF[["data_train"]][set_test,]
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


}
