data = read.table("assets/test_data/Gun_Point/Gun_Point_TRAIN")
unique(data[,1])

ones = data[data[,1] == 1,-1]
twos = data[data[,1] == 2,-1]

bag1 = series_to_wordbag(unlist(ones[1,]), 33, 17, 15, "exact", 0.01)
for (i in c(2:length(ones[,1]))) {
  bag = series_to_wordbag(unlist(ones[i,]), 33, 17, 15, "exact", 0.01)
  bag1 = rbind.fill(list(bag1, bag))
  bag1 <- ddply(bag1, .(words), function(x) { sum(x[,-1], na.rm = T) })
}

bag2 = series_to_wordbag(unlist(twos[1,]), 33, 17, 15, "exact", 0.01)
for (i in c(2:length(twos[,1]))) {
  bag = series_to_wordbag(unlist(twos[i,]), 33, 17, 15, "exact", 0.01)
  bag2 = rbind.fill(list(bag2, bag))
  bag2 <- ddply(bag2, .(words), function(x) { sum(x[,-1], na.rm = T) })
}

tfidf = tf_idf(merge(bag1, bag2, by = c("words"), all = T))

test = read.table("assets/test_data/Gun_Point/Gun_Point_TEST")
labels = test[,1]
predicted = rep(-1,length(labels))
for (i in c(1:length(test[,1]))) {
  series = test[i,-1]
  bag = series_to_wordbag(unlist(series), 33, 17, 15, "exact", 0.01)
  mm = merge(tfidf, bag, by = c("words"), all = T)
  mm[is.na(mm)] <- 0.0
  cosine1 = cosineSim(rbind(mm$`1`,mm$counts))
  cosine2 = cosineSim(rbind(mm$`2`,mm$counts))
  prediction = ifelse(cosine1 < cosine2, 1, 2)
  predicted[i] = prediction
}

sum((labels == predicted))


