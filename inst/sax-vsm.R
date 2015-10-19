# load Cylinder-Bell-Funnel data
data("CBF")
str(CBF)

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

head(bag1)

# compute tf*idf
tfidf = bags_to_tfidf(
          list("cylinder" = bag1, "bell" = bag2, "funnel" = bag3) )

head(tfidf)
library(dplyr)
head(arrange(tfidf, desc(funnel)))

# make up a sample time-series
#
sample = (CBF[["data_train"]][CBF[["labels_train"]] == 3,])[1,]
sample_bag = sax_via_window(sample, w, p, a, "exact", 0.01)
df = data.frame(index = as.numeric(names(sample_bag)),
                   words = unlist(sample_bag))
# weight found patterns
#
weighted_patterns = merge(df, tfidf)
specificity = rep(0, length(sample))
for(i in 1:length(weighted_patterns$words)){
  pattern = weighted_patterns[i,]
  for(j in 1:w){
    specificity[pattern$index+j] = specificity[pattern$index+j] +
      pattern$funnel - pattern$bell - pattern$cylinder
  }
}

# plot weighted patterns
#
library(ggplot2)
library(scales)
ggplot(data=data.frame(x=c(1:length(sample)), y=sample, col=rescale(specificity)),
     aes(x=x,y=y,color=col)) + geom_line(size=1.2) + theme_bw() +
     ggtitle("The funnel class-characteristic pattern example") +
     scale_colour_gradientn(name = "Class specificity:  ",limits=c(0,1),
        colours=c("red","yellow","green","lightblue","darkblue"),
        breaks=c(0,0.5,1),labels=c("negative","neutral","high"),
        guide = guide_colorbar(title.theme=element_text(size=14, angle=0),title.vjust=1,
        barheight=0.6, barwidth=6, label.theme=element_text(size=10, angle=0))) +
     theme(legend.position="bottom",plot.title=element_text(size=18),
        axis.title.x=element_blank(), axis.title.y=element_blank(),
        axis.text.x=element_text(size=12),axis.text.y=element_blank(),
        panel.grid.major.y = element_blank(), panel.grid.minor.y = element_blank(),
        axis.ticks.y = element_blank())


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
