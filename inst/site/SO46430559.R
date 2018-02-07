library(jmotif)

# create an example dataframe, list work the best cause library is not "vectorized"
example1 <- list(flow = c(1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9),
                 weight1 = c(7.1, 7.2, 7.3, 7.4, 7.5, 7.6, 7.7, 8.8, 9.9),
                 weight2 = c(8.1, 8.2, 8.3, 8.4, 8.5, 8.6, 8.7, 8.8, 8.9))

# this makes working with not-vectorized code easier
library(plyr)

# z-normalize
examplezn <- llply(example1, function(x){znorm(x, threshold = 0.01)})

# perform piecewise aggregate approximation, not really nedded if you do paa while saxifying..
llply(examplezn, function(x){paa(x, 3)})

# represent time series as SAX
example_sax <- llply(example1, function(x){sax_via_window(x, 3, 2, 3, "none", 0.1)})

# to a data frame, by rows though
df_by_row <- ldply(example_sax, unlist)

df_by_column <- as.data.frame(t(df_by_row))

clusters <- hclust(df_by_column)
plot(clusters)
