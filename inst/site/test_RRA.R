library(jmotif)

w <- 100
p <- 4
a <- 4
#
ecg_sax <- sax_via_window(ecg0606, w, p, a, "exact", 0.01)
#
ecg_str <- paste(ecg_sax, collapse = " ")
#
ecg_grammar <- str_to_repair_grammar(ecg_str)
#
#
library(plyr)
sax_df <- ldply(ecg_sax,function(x){x})
names(sax_df) <- c("idx","word")
sax_df$idx <- as.numeric(sax_df$idx)
#
rule_intervals <- ldply(ecg_grammar, function(x){
  starts <- x$rule_interval_starts
  ends <- x$rule_interval_ends
  starts <- sax_df[starts + 1, ]$idx
  ends <- sax_df[ends + 1, ]$idx + w
  data.frame(rule = rep(x$rule_name, length(starts)),
             frequency = rep(length(starts), length(starts)),
             start = starts, end = ends)
})
#
head(rule_intervals)
unique(cbind(rule_intervals$rule, rule_intervals$frequency))

