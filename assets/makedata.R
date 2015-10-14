dtrain <- read.table("data/Gun_Point/Gun_Point_TRAIN", as.is = T)
dtest <- read.table("data/Gun_Point/Gun_Point_TEST", as.is = T)
labels_train = unlist(dtrain[,1])
labels_test = unlist(dtest[,1])

dtrain = data.frame( class = rep("train", length(labels_train)), label = labels_train,
                     matrix(unlist(dtrain[,-1]), nrow = length(labels_train)) )
names(dtrain) <- c("class", "label", paste(1:150))

dtest = data.frame( class = rep("test", length(labels_test)), label = labels_test,
                     matrix(unlist(dtest[,-1]), nrow = length(labels_test)) )
names(dtest) <- c("class", "label", paste(1:150))

gun_point = rbind(dtrain, dtest)

cbind(t(t(labels_train)),


str(data)
data <- cbind(t(t(labels)),data)
names(data) <- c("label",paste(1:150))

data = matrix(unlist(data[,-1]), nrow = length(labels))
str(data)
data <- cbind(t(t(labels)),data)
names(data) <- c("label",paste(1:150))

save(data, file = "data/Gun_Point_TRAIN.rda")

data = read.table("data/Gun_Point/Gun_Point_TEST", as.is = T)
labels = unlist(data[,1])
unique(labels)

data = matrix(unlist(data[,-1]), nrow = length(labels))
str(data)
data <- cbind(t(t(labels)),data)
names(data) <- c("label",paste(1:150))
save(data, file = "data/Gun_Point_TEST.rda")
#
##
#
data = read.table("data/CBF/CBF_TRAIN", as.is = T)
labels = unlist(data[,1])
unique(labels)

data = matrix(unlist(data[,-1]), nrow = length(labels))
str(data)
data <- cbind(t(t(labels)),data)
names(data) <- c("label",paste(1:128))
save(data, file = "data/CBF_TRAIN.rda")

data = read.table("data/CBF/CBF_TEST", as.is = T)
labels = unlist(data[,1])
unique(labels)

data = matrix(unlist(data[,-1]), nrow = length(labels))
str(data)
data <- cbind(t(t(labels)),data)
names(data) <- c("label",paste(1:128))
save(data, file = "data/CBF_TEST.rda")
