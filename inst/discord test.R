dat=rnorm(100)
get_discords_brute_force(dat,10,1)
dat=read.table("inst/test_data/ecg0606_1.csv",header=F,as.is=T);
plot(dat$V1, type="l")
get_discords_brute_force(dat$V1,100,1)
