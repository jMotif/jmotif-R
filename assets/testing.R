y = c(-1, -2, -1, 0, 2, 1, 1, 0)
plot(y, type="l", col="blue",
     main="8-points time series and it PAA transform into 3 points")

points(y, pch=16, lwd=5, col="blue")

abline(v=c(1,1+7/3,1+7/3*2,8), lty=3, lwd=2, col="gray50")

y_paa3 = paa(y, 3)

segments(1,y_paa3[1],1+7/3,y_paa3[1],lwd=1,col="red")
points(x=1+7/3/2,y=y_paa3[1],col="red",pch=23,lwd=5)

segments(1+7/3,y_paa3[2],1+7/3*2,y_paa3[2],lwd=1,col="red")
points(x=1+7/3+7/3/2,y=y_paa3[2],col="red",pch=23,lwd=5)

segments(1+7/3*2,y_paa3[3],8,y_paa3[3],lwd=1,col="red")
points(x=1+7/3*2+7/3/2,y=y_paa3[3],col="red",pch=23,lwd=5)
