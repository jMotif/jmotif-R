### An R implementation of SAX-VSM

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
[*SAX-VSM: Interpretable Time Series Classification Using SAX and Vector Space Model.*](https://scholar.google.fr/citations?view_op=view_citation&citation_for_view=pVTcEd8AAAAJ:5nxA0vEk-isC)
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
      
![z-normalization of a scaled sine wave](https://raw.githubusercontent.com/jMotif/jmotif-R/master/inst/fig_znorm.png)

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
      
![PAA transform of an 8-points time series into 3 points](https://raw.githubusercontent.com/jMotif/jmotif-R/master/inst/fig_paa83.png)

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
      
![an application of SAX transform (3 letters word size and 3 letters alphabet size) to an 8 points time series ](https://raw.githubusercontent.com/jMotif/jmotif-R/master/inst/fig_sax83.png)

#### 4.0 Time series SAX transform via sliding window (`sax_via_window(ts, win_size, paa_size, alp_size, nr_strategy, n_threshold)`)


#### 5.0 SAX-VSM classifier
While the parameters optimization sampler is yet to be coded, the current code version illustrates SAX-VSM-based interpretable classification. 

For this, at the *first step*, a training dataset needs to be discretized and represented as a bag of words list, where each list element represents a training class. The CBF is a standard UCR time series classification dataset that consists of three time series classes of length 128. The data embedded into the `jmotif` library:


    # load Cylinder-Bell-Funnel data
    data("CBF")
    str(CBF)
    
the dataset overview:
    
    List of 4
    $ labels_train: num [1:30] 1 1 1 3 2 2 1 3 2 1 ...
    $ data_train  : num [1:30, 1:128] -0.464 -0.897 -0.465 -0.187 -1.136 ...
    $ labels_test : num [1:900] 2 2 1 2 2 3 1 3 2 3 ...
    $ data_test   : num [1:900, 1:128] -1.517 -0.703 -1.412 -0.955 -1.449 ...

at the *second step*, the three classes of training data are discretized into bags of words using `manyseries_to_wordbag` function:

    # set the discretization parameters
    w <- 60 # the sliding window size
    p <- 6  # the PAA size
    a <- 6  # the SAX alphabet size

    # convert the data to wordbags (the dataset has three labels: 1, 2, 3)
    bag1 <- manyseries_to_wordbag(CBF[["data_train"]][CBF[["labels_train"]] == 1,], 
                                                                w, p, a, "exact", 0.01)
    bag2 <- manyseries_to_wordbag(CBF[["data_train"]][CBF[["labels_train"]] == 2,],
                                                                w, p, a, "exact", 0.01)
    bag3 <- manyseries_to_wordbag(CBF[["data_train"]][CBF[["labels_train"]] == 3,],
                                                                w, p, a, "exact", 0.01)

each of these bags is a two-column data frame:

    > head(bag1)
       words counts
    1 aabeee      2
    2 aabeef      1
    3 aaceee      7
    4 aacfee      1
    5 aadeee      7
    6 aaedde      1

`TF*IDF` weights are computed at the *third step*, for this a list of _named_ (by class tokens) list of word bags is fed into `bags_to_tfidf` function:

    # compute tf*idf for three bags
    tfidf = bags_to_tfidf(
          list("cylinder" = bag1, "bell" = bag2, "funnel" = bag3) )

this yields a data frame of four variables: the words which are "important" in `TF*IDF` terms (i.e. not presented at least in one of the bags) and their class-corresponding weights:

    > tail(tfidf)
         words  cylinder     bell funnel
    640 ffcbbb 0.6525709 0.445449 0.0000
    641 ffdbab 0.0000000 0.000000 0.7615
    642 ffdbbb 1.7681483 0.000000 0.0000
    643 ffdcaa 0.0000000 0.000000 0.7615
    644 ffdcba 0.0000000 0.000000 0.7615
    645 ffebbb 1.5230000 0.000000 0.0000

which makes it easy to find which exact pattern contributes the most to the class:

    > library(dplyr)
    > head(arrange(tfidf, desc(cylinder)))
       words cylinder bell funnel
    1 aaeeee 2.413898    0      0
    2 aaceee 2.284500    0      0
    3 aadeee 2.284500    0      0

    > head(arrange(tfidf, desc(funnel)))
       words cylinder bell   funnel
    1 fedcba        0    0 2.975097
    2 fedbba        0    0 2.284500
    3 adfecb        0    0 1.968449
    
or to visualize those:

    # make up a sample time-series
    #
    sample = (CBF[["data_train"]][CBF[["labels_train"]] == 3,])[1,]
    sample_bag = sax_via_window(sample, w, p, a, "exact", 0.01)
    df = data.frame(index = as.numeric(names(sample_bag)),
                   words = unlist(sample_bag))
    # weight found patterns
    #
    weighted_patterns = merge(df, tfidf)
    specificity = rep(0, length(sample))
    for(i in 1:length(weighted_patterns$words)){
      pattern = weighted_patterns[i,]
      for(j in 1:w){
        specificity[pattern$index+j] = specificity[pattern$index+j] +
        pattern$funnel - pattern$bell - pattern$cylinder
      }
    }

    # plot weighted patterns
    #
    library(ggplot2)
    library(scales)
    ggplot(data=data.frame(x=c(1:length(sample)), y=sample, col=rescale(specificity)),
     aes(x=x,y=y,color=col)) + geom_line(size=1.2) + theme_bw() +
     ggtitle("The funnel class-characteristic pattern example") +
     scale_colour_gradientn(name = "Class specificity:  ",limits=c(0,1),
        colours=c("red","yellow","green","lightblue","darkblue"),
        breaks=c(0,0.5,1),labels=c("negative","neutral","high"),
        guide = guide_colorbar(title.theme=element_text(size=14, angle=0),title.vjust=1,
        barheight=0.6, barwidth=6, label.theme=element_text(size=10, angle=0))) +
     theme(legend.position="bottom",plot.title=element_text(size=18),
        axis.title.x=element_blank(), axis.title.y=element_blank(),
        axis.text.x=element_text(size=12),axis.text.y=element_blank(),
        panel.grid.major.y = element_blank(), panel.grid.minor.y = element_blank(),
        axis.ticks.y = element_blank())
    
![interpretable time series representation](https://raw.githubusercontent.com/jMotif/jmotif-R/master/inst/fig-weighted_funnel.png)    
    
or to classify the test data:

    # classify the test data
    labels_predicted = rep(-1, length(CBF[["labels_test"]]))
    labels_test = CBF[["labels_test"]]
    data_test = CBF[["data_test"]]
    for (i in c(1:length(data_test[,1]))) {
        series = data_test[i,]
        bag = series_to_wordbag(series, w, p, a, "exact", 0.01)
        cosines = cosine_sim(list("bag"=bag, "tfidf" = tfidf))
        labels_predicted[i] = which(cosines$cosines == max(cosines$cosines))
    }
    error = length(which((labels_test != labels_predicted))) / length(labels_test)
    error

    which((labels_test != labels_predicted))
