#1 setup
install.packages("devtools")
library(devtools)
install_github('jMotif/jmotif-R')

# 2
x <- seq(0, pi * 4, 0.02)
y <- sin(x) * 5 + rnorm(length(x))

plot(x, y, type="l", col="blue",
     main="A scaled sine wave with a random noise and its z-normalization")

lines(x, znorm(y, 0.01), type = "l", col = "red")
abline(h=c(1,-1), lty = 2, col = "gray50")
legend(0, -4, c("scaled sine wave","z-normalized wave"),
       lty = c(1,1), lwd = c(1,1), col = c("blue","red"), cex = 0.8)

# 3
y <- c(-1, -2, -1, 0, 2, 1, 1, 0)
plot(y, type = "l", col = "blue",
    main = "8-points time series and it PAA transform into 3 points")
points(y, pch = 16, lwd = 5, col = "blue")

abline(v = c(1,1+7/3,1+7/3*2,8), lty = 3, lwd = 2, col = "gray50")

y_paa3 <- paa(y, 3)

segments(1,y_paa3[1],1+7/3,y_paa3[1],lwd=1,col="red")
points(x=1+7/3/2,y=y_paa3[1],col="red",pch=23,lwd=5)

segments(1+7/3,y_paa3[2],1+7/3*2,y_paa3[2],lwd=1,col="red")
points(x=1+7/3+7/3/2,y=y_paa3[2],col="red",pch=23,lwd=5)

segments(1+7/3*2,y_paa3[3],8,y_paa3[3],lwd=1,col="red")
points(x=1+7/3*2+7/3/2,y=y_paa3[3],col="red",pch=23,lwd=5)

# 4
y <- seq(-2,2, length=100)
x <- dnorm(y, mean=0, sd=1)
lines(x,y, type="l", lwd=5, col="magenta")
abline(h = alphabet_to_cuts(3)[2:3], lty=2, lwd=2, col="magenta")
text(0.7,-1,"a",cex=2,col="magenta")
text(0.7, 0,"b",cex=2,col="magenta")
text(0.7, 1,"c",cex=2,col="magenta")

series_to_chars(y_paa3, 3)
series_to_string(y_paa3, 3)

# 5
# load Cylinder-Bell-Funnel data
data("CBF")
str(CBF)

# set the discretization parameters
#
w <- 60 # the sliding window size
p <- 6  # the PAA size
a <- 6  # the SAX alphabet size

# convert the train classes to wordbags (the dataset has three labels: 1, 2, 3)
#
cylinder <- manyseries_to_wordbag(CBF[["data_train"]][CBF[["labels_train"]] == 1,],
                                  w, p, a, "exact", 0.01)
bell <- manyseries_to_wordbag(CBF[["data_train"]][CBF[["labels_train"]] == 2,],
                              w, p, a, "exact", 0.01)
funnel <- manyseries_to_wordbag(CBF[["data_train"]][CBF[["labels_train"]] == 3,],
                                w, p, a, "exact", 0.01)

# compute tf*idf weights for three bags
#
tfidf = bags_to_tfidf(
  list("cylinder" = cylinder, "bell" = bell, "funnel" = funnel) )
tail(tfidf)

# make up a sample time-series
#
sample = (CBF[["data_train"]][CBF[["labels_train"]] == 3,])[1,]
sample_bag = sax_via_window(sample, w, p, a, "exact", 0.01)
df = data.frame(index = as.numeric(names(sample_bag)),
                words = unlist(sample_bag))

# weight the found patterns
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

# plot the weighted patterns
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
