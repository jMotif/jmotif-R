library(jmotif)

ecg <- ecg0606

require(ggplot2)
require(grid)
require(gridExtra)

df <- data.frame(time = c(1:length(ecg)), value = ecg)
p1 <- ggplot(df, aes(time, value)) + geom_line(lwd = 1.1, color = "blue1") + theme_classic() +
  ggtitle("Dataset ECG qtdb 0606 [701-3000]") +
  theme(plot.title = element_text(size = rel(1.5)),
        axis.title.x = element_blank(), axis.title.y = element_blank(),
        axis.ticks.y = element_blank(), axis.text.y = element_blank()) +
  geom_line(data = df[390:510, ], aes(time, value), lwd = 2, col = "red")

w <- 100
p <- 8
a <- 8

ecg_sax <- sax_via_window(ecg, w, p, a, "none", 0.01)
ecg_str <- paste(ecg_sax, collapse = " ")
ecg_grammar <- str_to_repair_grammar(ecg_str)

density_curve <- rep(0, length(ecg))
for (i in 2:length(ecg_grammar)) {
  rule <- ecg_grammar[[i]]
  for (j in 1:length(rule$rule_interval_starts)) {
    xs <- rule$rule_interval_starts[j]
    xe <- rule$rule_interval_ends[j] + w
    density_curve[xs:xe] <- density_curve[xs:xe] + 1;
  }
}

which(density_curve == min(density_curve))
min_values <- data.frame(x = c(444:476), y = rep(0, (476 - 443)))

density_df <- data.frame(time = c(1:length(density_curve)), value = density_curve)
shade <- rbind(c(0,0), density_df, c(2229,0))
names(shade) <- c("x", "y")
p2 <- ggplot(density_df, aes(x = time, y = value)) +
  geom_line(col = "cyan2") + theme_classic() +
  geom_polygon(data = shade, aes(x, y), fill = "cyan", alpha = 0.5) +
  ggtitle("RePair rules density for (w=100,p=6,a=6)") +
  theme(plot.title = element_text(size = rel(1.5)), axis.title.x = element_blank(),
        axis.title.y = element_blank(),
        axis.ticks.y = element_blank(), axis.text.y = element_blank()) +
  geom_line(data = min_values, aes(x, y), lwd = 2, col = "red")
p2

grid.arrange(p1, p2, ncol = 1)
