library(jmotif)

## makes a gun-point dataset
#
dtrain <- read.table("data/Gun_Point/Gun_Point_TRAIN", as.is = TRUE)
dtest <- read.table("data/Gun_Point/Gun_Point_TEST", as.is = TRUE)

labels_train = unlist(dtrain[,1])
labels_test = unlist(dtest[,1])

data_train <- matrix(unlist(dtrain[,-1]), nrow = length(labels_train))
data_test <- matrix(unlist(dtest[,-1]), nrow = length(labels_test))

Gun_Point <- list("labels_train" = labels_train, "data_train" = data_train,
                 "labels_test" = labels_test, "data_test" = data_test)

save(Gun_Point, file = "data/Gun_Point.rda")

## makes the CBF dataset
#
dtrain <- read.table("data/CBF/CBF_TRAIN", as.is = TRUE)
dtest <- read.table("data/CBF/CBF_TEST", as.is = TRUE)

labels_train <- unlist(dtrain[,1])
labels_test <- unlist(dtest[,1])

data_train <- matrix(unlist(dtrain[,-1]), nrow = length(labels_train))
data_test <- matrix(unlist(dtest[,-1]), nrow = length(labels_test))

CBF <- list("labels_train" = labels_train, "data_train" = data_train,
                 "labels_test" = labels_test, "data_test" = data_test)

save(CBF, file = "data/CBF.rda")

## makes ECG0606
#
ecg0606 <- read.table("inst/test_data/ecg0606_1.csv", as.is = T, header = F)$V1

save(ecg0606, file = "data/ECG0606.rda")
