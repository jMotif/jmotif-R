### R (Rcpp) implementation of SAX, HOT-SAX, VSM, and SAX-VSM (with parameters optimization)

[![Build Status](https://travis-ci.org/jMotif/jmotif-R.svg?branch=master)](https://travis-ci.org/jMotif/jmotif-R)
[![codecov.io](http://codecov.io/github/jMotif/jmotif-R/coverage.svg?branch=master)](http://codecov.io/github/jMotif/jmotif-R?branch=master)
[![CRAN](http://www.r-pkg.org/badges/version/jmotif)](http://cran.rstudio.com/package=jmotif)
[![License](http://img.shields.io/:license-gpl2-green.svg)](http://dplwww.gnu.org/licenses/gpl-2.0.html)
[![Downloads](http://cranlogs.r-pkg.org/badges/jmotif?color=brightgreen)](https://github.com/jMotif/jmotif-R)

Implements a set of R functions for time series pattern mining based on Symbolic Aggregate approXimation (i.e., SAX) and Vector Space Model (i.e., VSM) -- which makes SAX-VSM! 

Specifically, this library implements the full stack of tools needed for SAX-VSM [5] -- an algorithm for interpretable time series classification and characteristic patterns discovery -- among which are the time series z-Normalization [1], PAA [2], SAX [3], and VSM [4]. In addition, implements HOT-SAX -- the time series discord (time series anomaly) discovery algorithm [6].

[1] Dina Goldin and Paris Kanellakis,
[*On similarity queries for time-series data: Constraint specification and implementation*](http://dl.acm.org/citation.cfm?id=726176),
In Principles and Practice of Constraint Programming – CP ’95, pages 137–153. (1995)

[2] Keogh, E., Chakrabarti, K., Pazzani, M., & Mehrotra, S., 
[*Dimensionality reduction for fast similarity search in large time series databases*](https://scholar.google.fr/scholar?cluster=12943037197327240079&hl=en&as_sdt=0,5&as_vis=1),
Knowledge and information Systems, 3(3), 263-286. (2001)

[3] Lonardi, S., Lin, J., Keogh, E., & Patel, P., 
[*Finding motifs in time series*](https://scholar.google.com/scholar?cluster=9053042128452360340&hl=en&as_sdt=0,5), In Proc. of the 2nd Workshop on Temporal Data Mining (pp. 53-68). (2002)

[4] Salton, G., Wong, A., Yang., C., [*A vector space model for automatic indexing*](http://dl.acm.org/citation.cfm?id=361220), Commun. ACM 18, 11, 613–620, 1975.

[5] Senin Pavel and Malinchik Sergey, 
[*SAX-VSM: Interpretable Time Series Classification Using SAX and Vector Space Model.*](https://scholar.google.fr/citations?view_op=view_citation&citation_for_view=pVTcEd8AAAAJ:5nxA0vEk-isC),
Data Mining (ICDM), 2013 IEEE 13th International Conference on, pp.1175,1180, 7-10 Dec. 2013.

[6] Keogh, E., Lin, J., Fu, A.,
[*HOT SAX: Efficiently finding the most unusual time series subsequence*](http://www.cs.ucr.edu/~eamonn/HOT%20SAX%20%20long-ver.pdf),
In Proc. ICDM (2005)

#### 0.0 Installation
    install.packages("devtools")
    library(devtools)
    install_github('jMotif/jmotif-R')

#### 1.0 z-Normalization
Z-normalization (`znorm(ts, threshold)`) is a common in time series pattern mining preprocessing step proposed by Goldin & Kannelakis whose goal is to allow the downstream analyses to focus on the time series structural similarities/differences instead of the signal amplitude.

    x = seq(0, pi*4, 0.02)
    y = sin(x) * 5 + rnorm(length(x))

    plot(x, y, type="l", col="blue", main="A scaled sine wave with a random noise and its z-normalization")

    lines(x, znorm(y, 0.01), type="l", col="red")
    abline(h=c(1,-1), lty=2, col="gray50")
    legend(0, -4, c("scaled sine wave","z-normalized wave"), lty=c(1,1), lwd=c(1,1), 
                                                                    col=c("blue","red"), cex=0.8)
      
![z-normalization of a scaled sine wave](https://raw.githubusercontent.com/jMotif/jmotif-R/master/inst/site/fig_znorm.png)

#### 2.0 Piecewise Aggregate Approximation (i.e., PAA)
PAA (`paa(ts, paa_num)`) is designed to reduce the input time series dimensionality by splitting it into equally-sized segments (PAA size) and averaging values of points within each segment. Typically, PAA is applied to z-Normalized time series. In the following example the time series of dimensionality 8 points is reduced to 3 points.

    y = c(-1, -2, -1, 0, 2, 1, 1, 0)
    plot(y, type="l", col="blue", main="8-points time series and it PAA transform into 3 points")

    points(y, pch=16, lwd=5, col="blue")

    abline(v=c(1,1+7/3,1+7/3*2,8), lty=3, lwd=2, col="gray50")

    y_paa3 = paa(y, 3)

    segments(1,y_paa3[1],1+7/3,y_paa3[1],lwd=1,col="red")
    points(x=1+7/3/2,y=y_paa3[1],col="red",pch=23,lwd=5)

    segments(1+7/3,y_paa3[2],1+7/3*2,y_paa3[2],lwd=1,col="red")
    points(x=1+7/3+7/3/2,y=y_paa3[2],col="red",pch=23,lwd=5)

    segments(1+7/3*2,y_paa3[3],8,y_paa3[3],lwd=1,col="red")
    points(x=1+7/3*2+7/3/2,y=y_paa3[3],col="red",pch=23,lwd=5)
      
![PAA transform of an 8-points time series into 3 points](https://raw.githubusercontent.com/jMotif/jmotif-R/master/inst/site/fig_paa83.png)

#### 3.0 SAX transform
SAX transform (`series_to_string(ts, alphabet_size)`) is a discretization algorithm which transforms a sequence of rational values (time series points) into a sequence of discrete values - symbols taken from a finite alphabet. This procedure enables the application of numerous algorithms for discrete data analysis to continuous time series data. 

Typically, SAX applied to time series of reduced with PAA dimensionality, which effectively yields a low-dimensional, discrete representation of the input time series which preserves (to some extent) its structural characteristics. By employing this representation it is possible to design efficient algorithms for common time series pattern mining tasks as one can rely on the indexing of data in symbolic space. Note, that before processing with PAA and SAX, time series are z-Normalized.

The figure below illustrates the PAA+SAX procedure: 8 points time series is converted into 3-points PAA representation at the first step, PAA values are converted into letters by using 3 letters alphabet at the second step.

    y <- seq(-2,2, length=100)
    x <- dnorm(y, mean=0, sd=1)
    lines(x,y, type="l", lwd=5, col="magenta")
    abline(h = alphabet_to_cuts(3)[2:3], lty=2, lwd=2, col="magenta")
    text(0.7,-1,"a",cex=2,col="magenta")
    text(0.7, 0,"b",cex=2,col="magenta")
    text(0.7, 1,"c",cex=2,col="magenta")

    > series_to_string(y_paa3, 3)
    [1] "acc"
    
    > series_to_chars(y_paa3, 3)
    [1] "a" "c" "c"
      
![an application of SAX transform (3 letters word size and 3 letters alphabet size) to an 8 points time series ](https://raw.githubusercontent.com/jMotif/jmotif-R/master/inst/site/fig_sax83.png)

#### 4.0 Time series SAX transform via sliding window 
Another common way to use SAX is to apply the procedure to sliding window-extracted subseries (`sax_via_window(ts, win_size, paa_size, alp_size, nr_strategy, n_threshold)`). This technique is used in SAX-VSM, where it unables the conversion of a time series into the word bags. Note, the use of a numerosity reduction strategy.

#### 5.0 SAX-VSM classifier
I use the one of [standard UCR time series datasets](http://www.cs.ucr.edu/~eamonn/time_series_data/) to illustrate the implemented approach. The Cylinder-Bell-Funnel dataset (Saito, N: *Local feature extraction and its application using a library of bases.* PhD thesis, Yale University (1994)) consists of three time series classes. The dataset is embedded into the `jmotif` library:  

    # load Cylinder-Bell-Funnel data
    data("CBF")
    
where it is wrapped into a list of four elements: train and test sets and their labels: 

    > str(CBF)
    List of 4
    $ labels_train: num [1:30] 1 1 1 3 2 2 1 3 2 1 ...
    $ data_train  : num [1:30, 1:128] -0.464 -0.897 -0.465 -0.187 -1.136 ...
    $ labels_test : num [1:900] 2 2 1 2 2 3 1 3 2 3 ...
    $ data_test   : num [1:900, 1:128] -1.517 -0.703 -1.412 -0.955 -1.449 ...

##### 5.1 Pre-processing and bags of words construction
At the first step, each class of the training data needs to be transformed into a bag of words using the `manyseries_to_wordbag` function, which z-normalizes each of time series and converts it into a set of words which added to the resulting bag:

    # set the discretization parameters
    #
    w <- 60 # the sliding window size
    p <- 6  # the PAA size
    a <- 6  # the SAX alphabet size

    # convert the train classes to wordbags (the dataset has three labels: 1, 2, 3)
    #
    cylinder <- manyseries_to_wordbag(CBF[["data_train"]][CBF[["labels_train"]] == 1,], w, p, a, "exact", 0.01)
    bell <- manyseries_to_wordbag(CBF[["data_train"]][CBF[["labels_train"]] == 2,], w, p, a, "exact", 0.01)
    funnel <- manyseries_to_wordbag(CBF[["data_train"]][CBF[["labels_train"]] == 3,], w, p, a, "exact", 0.01)

each of these bags is a two-columns data frame:

    > head(cylinder)
       words counts
    1 aabeee      2
    2 aabeef      1
    3 aaceee      7
    4 aacfee      1
    5 aadeee      7
    6 aaedde      1

##### 5.2 `TF*IDF` weighting
`TF*IDF` weights are computed at the second step with `bags_to_tfidf` function which accepts a single argument -- a list of _named_ (by class label) word bags:

    # compute tf*idf weights for three bags
    #
    tfidf = bags_to_tfidf( list("cylinder" = cylinder, "bell" = bell, "funnel" = funnel) )

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
    
and to visualize those on data:

    # make up a sample time-series
    #
    sample = (CBF[["data_train"]][CBF[["labels_train"]] == 3,])[1,]
    sample_bag = sax_via_window(sample, w, p, a, "exact", 0.01)
    df = data.frame(index = as.numeric(names(sample_bag)), words = unlist(sample_bag))
                   
    # weight the found patterns
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

    # plot the weighted patterns
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
    
![interpretable time series representation](https://raw.githubusercontent.com/jMotif/jmotif-R/master/inst/site/fig-weighted_funnel.png)    

![interpretable time series representation](https://raw.githubusercontent.com/jMotif/jmotif-R/master/inst/site/fig-weighted_bell.png)

![interpretable time series representation](https://raw.githubusercontent.com/jMotif/jmotif-R/master/inst/site/fig-weighted_cylinder.png)    
    
##### 5.3 SAX-VSM classification
Using the weighted patterns obtained at the previous step and the cosine similarity measure it is also easy to classify unlabeled data using the `cosine_sim` function which accepts a list of two elements: the bag-of-words representation of the input time series (constructed with `series_to_wordbag` function) and the `TF*IDF` weights table obtained at the previous step:

    # classify the test data
    #
    labels_predicted = rep(-1, length(CBF[["labels_test"]]))
    labels_test = CBF[["labels_test"]]
    data_test = CBF[["data_test"]]
    for (i in c(1:length(data_test[,1]))) {
        series = data_test[i,]
        bag = series_to_wordbag(series, w, p, a, "exact", 0.01)
        cosines = cosine_sim(list("bag"=bag, "tfidf" = tfidf))
        labels_predicted[i] = which(cosines$cosines == max(cosines$cosines))
    }
    
    # compute the classification error
    #
    error = length(which((labels_test != labels_predicted))) / length(labels_test)
    error

    # findout which time series were misclassified
    #
    which((labels_test != labels_predicted))

#### 6.0 SAX-VSM discretization parameters optimization
Her I shall show how the classification task discretization parameters optimization can be done with third-party libraries, specifically [nloptr](https://cran.r-project.org/web/packages/nloptr/) which implements DIRECT and [cvTools](https://cran.r-project.org/web/packages/cvTools/) which facilitates CV process. But not forget the magic of [plyr](https://github.com/hadley/plyr)!!! So here is the code:

    library(plyr)
    library(cvTools)
    library(nloptr)

    # the cross-validation error function
    # uses the following global variables
    #   1) nfolds -- specifies folds for the cross-validation
    #                if equal to the number of instances, then it is
    #                LOOCV
    #
    cverror <- function(x) {

      # the vector x suppos to contain reational values for the
      # discretization parameters
      #
      w = round(x[1], digits = 0)
      p = round(x[2], digits = 0)
      a = round(x[3], digits = 0)

      # few local vars to simplify the process
      m <- length(train_labels)
      c <- length(unique(train_labels))
      folds <- cvFolds(m, K = nfolds, type = "random")

      # saving the error for each folds in this array
      errors <- list()

      # cross-valiadtion business
      for (i in c(1:nfolds)) {

        # define data sets
        set_test <- which(folds$which == i)
        set_train <- setdiff(1:m, set_test)

        # compute the TF-IDF vectors
        bags <- alply(unique(train_labels),1,function(x){x})
        for (j in 1:c) {
          ll <- which(train_labels[set_train] == unique(train_labels)[j])
          bags[[unique(train_labels)[j]]] <-
            manyseries_to_wordbag( (train_data[set_train, ])[ll,], w, p, a, "exact", 0.01)
        }
        tfidf = bags_to_tfidf(bags)

        # compute the eror
        labels_predicted <- rep(-1, length(set_test))
        labels_test <- train_labels[set_test]
        data_test <- train_data[set_test,]

        for (j in c(1:length(labels_predicted))) {
          bag=NA
          if (length(labels_predicted)>1) {
            bag = series_to_wordbag(data_test[j,], w, p, a, "exact", 0.01)
          } else {
            bag = series_to_wordbag(data_test, w, p, a, "exact", 0.01)
          }
          cosines = cosine_sim(list("bag" = bag, "tfidf" = tfidf))
          if (!any(is.na(cosines$cosines))) {
            labels_predicted[j] = which(cosines$cosines == max(cosines$cosines))
          }
        }

        # the actual error value
        error = length(which((labels_test != labels_predicted))) / length(labels_test)
        errors[i] <- error
      }

      # output the mean cross-validation error as the result
      err = mean(laply(errors,function(x){x}))
      print(paste(w,p,a, " -> ", err))
      err
    }

    # define the data for CV
    train_data <- CBF[["data_train"]]
    train_labels <- CBF[["labels_train"]]
    nfolds = 15

    # perform the parameters optimization
    S <- directL(cverror, c(10,2,2), c(120,60,12),
                 nl.info = TRUE, control = list(xtol_rel = 1e-8, maxeval = 10))

The optimization process goes as follows:

    [1] "65 31 7  ->  1"
    [1] "65 31 7  ->  1"
    [1] "65 31 7  ->  1"
    [1] "28 31 7  ->  1"
    [1] "102 31 7  ->  1"
    [1] "65 12 7  ->  0.366666666666667"
    [1] "65 50 7  ->  1"
    [1] "65 31 4  ->  1"
    [1] "65 31 10  ->  1"
    [1] "28 12 7  ->  0.833333333333333"
    [1] "102 12 7  ->  0.666666666666667"
    [1] "65 12 4  ->  0"

    Call:
    nloptr(x0 = x0, eval_f = fn, lb = lower, ub = upper, opts = opts)
    Minimization using NLopt version 2.4.2 

    NLopt solver status: 5 ( NLOPT_MAXEVAL_REACHED: Optimization stopped because maxeval (above) was 
    reached. )

    Number of Iterations....: 10 
    Termination conditions:  stopval: -Inf xtol_rel: 1e-08 maxeval: 10 ftol_rel: 0 ftol_abs: 0 
    Number of inequality constraints:  0 
    Number of equality constraints:    0 
    Current value of objective function:  0 
    Current value of controls: 65 11.66667 3.666667

At this point S contains the best SAX parameters which were found using 10 DIRECT iterations, which we can use for the classification of the test data:

    w = round(S$par[1], digits = 0)
    p = round(S$par[2], digits = 0)
    a = round(S$par[3], digits = 0)

    # compute the TF-IDF vectors
    #
    bags <- alply(unique(train_labels),1,function(x){x})
    for (j in 1:length(unique(train_labels))) {
      ll <- which(train_labels == unique(train_labels)[j])
      bags[[unique(train_labels)[j]]] <-
        manyseries_to_wordbag( train_data[ll,], w, p, a, "exact", 0.01)
    }
    tfidf = bags_to_tfidf(bags)

    # classify the test data
    #
    labels_predicted = rep(-1, length(CBF[["labels_test"]]))
    labels_test = CBF[["labels_test"]]
    data_test = CBF[["data_test"]]
    for (i in c(1:length(data_test[,1]))) {
      print(paste(i))
      series = data_test[i,]
      bag = series_to_wordbag(series, w, p, a, "exact", 0.01)
      cosines = cosine_sim(list("bag"=bag, "tfidf" = tfidf))
      if (!any(is.na(cosines$cosines))) {
        labels_predicted[i] = which(cosines$cosines == max(cosines$cosines))
      }
    }

    # compute the classification error
    #
    error = length(which((labels_test != labels_predicted))) / length(labels_test)
    error

    # findout which time series were misclassified
    #
    which((labels_test != labels_predicted))
    par(mfrow=c(3,1))
    plot(data_test[316,], type="l")
    plot(data_test[589,], type="l")
    plot(data_test[860,], type="l")
    
which shows us that one instance of each of the classes was misclassified....

#### 7.0 HOT-SAX algorithm for time series discord discovery
Given a time series _T_, its subsequence _C_  is called *discord* if it has the largest Euclidean distance to its nearest non-self match. Thus, time series discord is a subsequence within a time series that is _maximally different_ to all the rest of subsequences in the time series, and therefore naturaly captures the most unusual subsequence within the time series.

The library embeds the ECG0606 dataset taken from [PHYSIONET FTP](http://physionet.org/physiobank/database/qtdb/). The raw data was transformed with their `rdsamp` utility 

    rdsamp -r sele0606 -f 120.000 -l 60.000 -p -c | sed -n '701,3000p' >0606.csv

and conists of 15 heartbeats:

![ECG0606 data](https://raw.githubusercontent.com/jMotif/jmotif-R/master/inst/site/ecg0606.png)

We know, that the third heartbeat of this dataset contains the true anomaly as it was discussed in HOTSAX paper by Eamonn Keogh, Jessica Lin, and Ada Fu. Note, that the authors were specifically interested in finding anomalies which are shorter than a regular heartbeat following a suggestion given by the domain expert: ''_We conferred with cardiologist, Dr. Helga Van Herle M.D., who informed us that heart irregularities can sometimes manifest themselves at scales significantly shorter than a single heartbeat_.'' Figure 13 of the paper further explains the nature of this true anomaly:

![ECG0606 clusters](https://raw.githubusercontent.com/jMotif/jmotif-R/master/inst/site/demo-ecg0606_cluster.png)

Two implementation of discord discovery provided within the code: the brute-force didcord discovery and HOT-SAX. 

The brute-force takes 14 seconds to discover 5 discords in the data (with early-abandoning distance):

    > lineprof( find_discords_brute_force(ecg0606, 100, 5) )
    Reducing depth to 2 (from 18)
        time    alloc  release dups                    ref            src
    1 13.951 6211.306 6209.214    2                ".Call" .Call         
    2  0.001    0.000    0.000   44 c(".Call", "tryCatch") .Call/tryCatch

whereas HOT-SAX finishes under a second:

    > lineprof( find_discords_hot_sax(ecg0606, 100, 4, 4, 0.01, 5) )
       time   alloc release dups     ref   src
    1 0.637 249.577 245.077   55 ".Call" .Call

The discords returned as a data frame sorted by the position:

    > discords = find_discords_hot_sax(ecg0606, 100, 4, 4, 0.01, 5)
    > discords
      nn_distance position
    1   0.4787745       37
    2   0.4177020      188
    3   1.5045847      411
    4   0.4437060      539
    5   0.4437060     1566
    
The best discord is the third one at 411:

    discords = find_discords_hot_sax(ecg0606, 100, 4, 4, 0.01, 5)
    plot(ecg0606, type = "l", col = "cornflowerblue", main = "ECG 0606")
    lines(x=c(discords[3,2]:(discords[3,2]+100)),
        y=ecg0606[discords[3,2]:(discords[3,2]+100)], col="red")

![ECG0606 clusters](https://raw.githubusercontent.com/jMotif/jmotif-R/master/inst/site/ecg0606_discord.png)


It is easy to sort discord by the nearest neighbor distance:

    > library(dplyr)
    > arrange(discords,desc(nn_distance))
      nn_distance position
    1   1.5045847      411
    2   0.4787745       37
    3   0.4437060      539
    4   0.4437060     1566
    5   0.4177020      188

