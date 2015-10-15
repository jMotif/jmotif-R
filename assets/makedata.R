## makes a gun-point dataset
#
dtrain <- read.table("data/Gun_Point/Gun_Point_TRAIN", as.is = T)
dtest <- read.table("data/Gun_Point/Gun_Point_TEST", as.is = T)

labels_train = unlist(dtrain[,1])
labels_test = unlist(dtest[,1])

data_train = matrix(unlist(dtrain[,-1]), nrow = length(labels_train))
data_test = matrix(unlist(dtest[,-1]), nrow = length(labels_test))

Gun_Point = list("labels_train" = labels_train, "labels_test" = data_train,
                 "labels_test" = labels_test, "data_test" = data_test)

save(Gun_Point, file = "data/Gun_Point.rda")

## makes the CBF dataset
#
dtrain <- read.table("data/CBF/CBF_TRAIN", as.is = T)
dtest <- read.table("data/CBF/CBF_TEST", as.is = T)

labels_train = unlist(dtrain[,1])
labels_test = unlist(dtest[,1])

data_train = matrix(unlist(dtrain[,-1]), nrow = length(labels_train))
data_test = matrix(unlist(dtest[,-1]), nrow = length(labels_test))

CBF = list("labels_train" = labels_train, "labels_test" = data_train,
                 "labels_test" = labels_test, "data_test" = data_test)

save(CBF, file = "data/CBF.rda")
