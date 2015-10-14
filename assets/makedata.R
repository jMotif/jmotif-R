data = read.table("data/Gun_Point/Gun_Point_TRAIN", as.is = T)
labels = unlist(data[,1])
unique(labels)

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
