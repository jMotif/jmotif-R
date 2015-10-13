### An R implementation of SAX-VSM and z-Normalization, PAA, and SAX

[![Build Status](https://travis-ci.org/jMotif/jmotif-R.svg?branch=master)](https://travis-ci.org/jMotif/jmotif-R) [![codecov.io](http://codecov.io/github/jMotif/jmotif-R/coverage.svg?branch=master)](http://codecov.io/github/jMotif/jmotif-R?branch=master)
[![License](http://img.shields.io/:license-gpl2-green.svg)](http://www.gnu.org/licenses/gpl-2.0.html)

Implements time series symbolic discretization via z-Normalization [1], PAA [2], and SAX [3] in R. Implements essentials of VSM [4], and SAX-VSM interpretable time-series classification [5].

[1] Dina Goldin and Paris Kanellakis,
_On similarity queries for time-series data: Constraint specification and implementation._ 
In Principles and Practice of Constraint Programming – CP ’95, pages 137–153. (1995)

[2] Keogh, E., Chakrabarti, K., Pazzani, M., & Mehrotra, S., 
_Dimensionality reduction for fast similarity search in large time series databases._
Knowledge and information Systems, 3(3), 263-286. (2001)

[3] Lonardi, S., Lin, J., Keogh, E., & Patel, P., 
_Finding motifs in time series._ 
In Proc. of the 2nd Workshop on Temporal Data Mining (pp. 53-68). (2002)

[4] Salton, G., Wong, A., Yang., C., [*A vector space model for automatic indexing*](http://dl.acm.org/citation.cfm?id=361220). Commun. ACM 18, 11, 613–620, 1975.

[5] Senin, P.; Malinchik, S., 
_SAX-VSM: Interpretable Time Series ClassiCfication Using SAX and Vector Space Model._
Data Mining (ICDM), 2013 IEEE 13th International Conference on, pp.1175,1180, 7-10 Dec. 2013.

#### 0.0 Installation
    install.packages("devtools")
    library(devtools)
    install_github('jMotif/jmotif-R')

#### 1.0 z-Normalization (`znorm(ts, threshold)`)
Z-normalization is a time series preprocessing step proposed Goldin&Kannelakis whose goal is to enable downstream analyses to focus on the time series structural similarities/differences instead of their amplitude.

    x = seq(0, pi*4, 0.02)
    y = sin(x) * 5 + rnorm(length(x))

    plot(x, y, type="l", col="blue", 
      main="A scaled sine wave with a random noise and its z-normalization")

    lines(x, *znorm(y, 0.01)*, type="l", col="red")
    abline(h=c(1,-1), lty=2, col="gray50")
    legend(0, -4, c("scaled sine wave","z-normalized wave"),
      lty=c(1,1), lwd=c(1,1), col=c("blue","red"), cex=0.8)
      
![z-normalization of a scaled sine wave](https://raw.githubusercontent.com/jMotif/jmotif-R/master/assets/fig_znorm.png)

#### 2.0 Piecewise Aggregate Approximation (i.e., PAA) (`paa(ts, paa_num)`)
PAA reduces the time series dimensionality by averaging values of equal-sized segments of the original time series. In the following example the time series of dimensionality 8 points is reduced to 3 points.

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
      
![PAA transform of an 8-points time series into 3 points](https://raw.githubusercontent.com/jMotif/jmotif-R/master/assets/fig_paa83.png)

#### 3.0 SAX transform (`ts2string(ts, a_size)`)
Transforms each of the input time series points into a letter. Typically, SAX applied to time series of reduced with PAA dimensionality in order to accelerate the time series search by their indexing in the symbolic space. Before processing with PAA and SAX, time series are usually z-Normalized.

The figure below illustrates the PAA+SAX procedure: 8 points time series is converted into 3-points PAA representation at the first step, PAA values are converted into letters by using 3 letters alphabet at the second step.

    y <- seq(-2,2, length=100)
    x <- dnorm(y, mean=0, sd=1)
    lines(x,y, type="l", lwd=5, col="magenta")
    abline(h = alphabet2cuts(3)[2:3], lty=2, lwd=2, col="magenta")
    text(0.7,-1,"a",cex=2,col="magenta")
    text(0.7, 0,"b",cex=2,col="magenta")
    text(0.7, 1,"c",cex=2,col="magenta")

    > ts2string(y_paa3, 3)
    [1] "a" "c" "c"
      
![an application of SAX transform (3 letters word size and 3 letters alphabet size) to an 8 points time series ](https://raw.githubusercontent.com/jMotif/jmotif-R/master/assets/fig_sax83.png)

#### 4.0 SAX-VSM classifier
While the sampler is yet to be coded, the SAX-VSM-based classification of UCR data can be performed with this li version:

    # load the TRAIN data
    #
    data = read.table("assets/test_data/Gun_Point/Gun_Point_TRAIN", as.is=T)
    labels = unlist(data[,1])
    unique(labels)
    data = matrix(unlist(data[,-1]), nrow=length(labels))
    str(data)
    plot(data[1,], type="l")

    # separate data sets according to labels
    #
    ones = data[labels == 1,]
    twos = data[labels == 2,]

    # SAX parameters to use
    w=30
    p=15
    a=10

    # convert these to word bags
    #
    bag1 = manyseries_to_wordbag(ones, w, p, a, "exact", 0.01)
    bag2 = manyseries_to_wordbag(twos, w, p, a, "exact", 0.01)

    # compute tf*idf weights 
    #
    tfidf = tf_idf(merge(bag1, bag2, by = c("words"), all = T))

    # load the TEST data
    #
    test = read.table("assets/test_data/Gun_Point/Gun_Point_TEST", as.is=T)
    test_labels = test[,1]
    
    # get ready for classification
    #
    predicted = rep(-1,length(labels))
    test = matrix(unlist(test[,-1]), nrow=length(test_labels))

    # classify time series
    #
    for (i in c(1:length(test[,1]))) {
    
      # the current time series pre-processing
      series = test[i,]  
      bag = series_to_wordbag(series, w, p, a, "exact", 0.01)
      
      # classification
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
    
