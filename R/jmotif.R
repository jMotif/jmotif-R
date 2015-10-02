# Hello, world!
#
#   Build and Reload Package:  'Ctrl + Shift + B'
#   Check Package:             'Ctrl + Shift + E'
#   Test Package:              'Ctrl + Shift + T'

znorm <- function(ts, threshold=0.01){

  if(is.matrix(ts)){

    # check dimensions
    if(1 == dim(ts)[1]){
     ts_sd = sd(ts[1,])
      if(ts_sd<threshold){
        return (unlist(ts[1,], use.names=F))
      }else{
        return (unlist((ts[1,] - mean(ts[1,]))/ts_sd, use.names=F))
      }

    } else if(1 == dim(ts)[2]){
      ts_sd = sd(ts[,1])
      if(ts_sd<threshold){
        return (unlist(ts[,1], use.names=F))
      }else{
        return (unlist((ts[,1] - mean(ts[,1]))/ts_sd, use.names=F))
      }

    } else {
      stop(paste("don't know how to process matrix of dimension",dim(ts)))
    }

  }else if (is.vector(ts)){
    ts_sd = sd(ts)
    if(ts_sd<threshold){
      return(ts)
    }else{
      return (unlist((ts - mean(ts))/ts_sd, use.names=F))
    }


  } else {
    stop(sprintf("argument %s must be matrix or array", sQuote("ts")))
  }

}

#library(jmotif)
#x=seq(0,pi*4,0.02)
#y=sin(x)*5+rnorm(length(x))
#plot(x,y,type="l",col="blue")
#lines(x,znorm(y,0.01),type="l",col="red")
#dim(y)<-c(1,2)
#lines(x,znorm(y,0.01),type="l",col="green")
#y=t(t(y))
#lines(x,znorm(y,0.01),type="l",col="orange")
