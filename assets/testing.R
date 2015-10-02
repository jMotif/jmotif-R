y = c(-1, -2, -1, 0, 2, 1, 1, 0)
plot(y, type="l", col="blue", xlim=c(0,8),
     main="8-points time series and its PAA transform into 3 points")

points(y, pch=16, lwd=5, col="blue")

abline(v=c(1,1+7/3,1+7/3*2,8), lty=3, lwd=2, col="gray50")

y_paa3 = paa(y, 3)

segments(1,y_paa3[1],1+7/3,y_paa3[1],lwd=1,col="red")
points(x=1+7/3/2,y=y_paa3[1],col="red",pch=23,lwd=5)

segments(1+7/3,y_paa3[2],1+7/3*2,y_paa3[2],lwd=1,col="red")
points(x=1+7/3+7/3/2,y=y_paa3[2],col="red",pch=23,lwd=5)

segments(1+7/3*2,y_paa3[3],8,y_paa3[3],lwd=1,col="red")
points(x=1+7/3*2+7/3/2,y=y_paa3[3],col="red",pch=23,lwd=5)

#
# #############
#
y = c(-1, -2, -1, 0, 2, 1, 1, 0)
plot(y, type="l", col="blue", xlim=c(0,8),
     main="8-points time series and its SAX transform into 3 letters")
points(y, pch=16, lwd=5, col="blue")
abline(v=c(1,1+7/3,1+7/3*2,8), lty=3, lwd=2, col="gray50")
y_paa3 = paa(y, 3)
segments(1,y_paa3[1],1+7/3,y_paa3[1],lwd=1,col="red")
points(x=1+7/3/2,y=y_paa3[1],col="red",pch=23,lwd=5)
segments(1+7/3,y_paa3[2],1+7/3*2,y_paa3[2],lwd=1,col="red")
points(x=1+7/3+7/3/2,y=y_paa3[2],col="red",pch=23,lwd=5)
segments(1+7/3*2,y_paa3[3],8,y_paa3[3],lwd=1,col="red")
points(x=1+7/3*2+7/3/2,y=y_paa3[3],col="red",pch=23,lwd=5)

y <- seq(-2,2, length=100)
x <- dnorm(y, mean=0, sd=1)
lines(x,y, type="l", lwd=5, col="magenta")
abline(h = alphabet2cuts(3)[2:3], lty=2, lwd=2, col="magenta")

