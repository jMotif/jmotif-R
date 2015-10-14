dd=load("data/CBF_TEST.rda")




# load the test data
#
data = read("data/CBF_TRAIN.rda", as.is = T)
labels = unlist(data[,1])
unique(labels)

data = matrix(unlist(data[,-1]), nrow = length(labels))
str(data)
data <- cbind(t(t(labels)),data)
names(data) <- c("label",paste(1:150))
save(data, file = "data/Gun_Point_TRAIN.rda")

data <- as.data.frame(data)

# separate data sets according to labels
#
ones <- data[labels == 1,]
twos <- data[labels == 2,]

# plot the data
#
library(ggplot2)
library(reshape)
library(grid)
library(gridExtra)
pgun <- ggplot(data = melt(cbind(idx = c(1:length(ones[,1])), as.data.frame(ones)), c("idx")),
                aes(x = variable,y = value,group = idx)) + geom_line(col = "cornflowerblue") +
            theme(rect = element_blank(), line = element_blank(), text = element_blank(),
                  axis.ticks.margin = unit(0, "lines"))
ppoint <- ggplot(data = melt(cbind(idx = c(1:length(twos[,1])), as.data.frame(twos)), c("idx")),
               aes(x = variable,y = value,group = idx)) + geom_line(col = "darkgoldenrod") +
  theme(rect = element_blank(), line = element_blank(), text = element_blank(),
        axis.ticks.margin = unit(0, "lines"))

grid.arrange(pgun, ppoint, ncol = 1)

#
w <- 60
p <- 20
a <- 6

# convert these to word bags
#
bag1 <- manyseries_to_wordbag(ones, w, p, a, "exact", 0.01)
bag2 <- manyseries_to_wordbag(twos, w, p, a, "exact", 0.01)

# compute tf*idf
#
tfidf = tf_idf(merge(bag1, bag2, by = c("words"), all = T))

# classify the test data
#
test = read.table("assets/test_data/Gun_Point/Gun_Point_TEST", as.is=T)
test_labels = test[,1]
predicted = rep(-1,length(labels))
test = matrix(unlist(test[,-1]), nrow=length(test_labels))

for (i in c(1:length(test[,1]))) {
  series = test[i,]
  bag = series_to_wordbag(series, w, p, a, "exact", 0.01)
  mm = merge(tfidf, bag, by = c("words"), all = T)
  mm[is.na(mm)] <- 0.0
  cosine1 = cosineSim(rbind(mm$`1`,mm$counts))
  cosine2 = cosineSim(rbind(mm$`2`,mm$counts))
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

diamonds
