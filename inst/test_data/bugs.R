library(jmotif)
discords <- find_discords_rra(ecg0606, 100, 4, 4, "mindist", 0.01, 4)


library(jmotif)
str_to_repair_grammar("adcc ccbb bbbd bbcb bdbb cbbc accb ccbb daba cbbc bbdb bcbb dabc bbcb adcc ccbc dabb	cbbd bbcb cdbb cbbc bbdc adcc cbcb bbbd bbcb bdbb cbbc accb cdbb dbbb cbbd bccb cdab cbbc accb ccbb dabb cbbd bbcb bdbb cbbc accb ccbb cbbd bbcb bdbb cbbc accb ccbb dabb cbbd bbcb bdbb cbbc accc ccbc dacb bbbd 	bbcb bdbb cbbc accb ccbc dbba cbbc bbdb bcbb dbbc bbcb adcc ccbc daba cbbc bbdb bcbb dbbc bbcb adcb bdac dcbb cbbd bbcb bdbb cbbc accb cdbb dbbb cbbd bccb bdab cbbc accb cdbb dbbb cbbd bbcb bdbb cbbc accc ccbb cbbd bbcb bdbb cbbc accb")



library(jmotif)
str_to_repair_grammar("bbcb bdbb cbbc accb cdbb dbbb cbbd bccb bdab cbbc accb cdbb dbbb cbbd bbcb bdbb cbbc accc ccbb cbbd bbcb bdbb cbbc accb")


data = fread("/Users/psenin/workspace/grammarviz2_src/data/mitdb__100_180.txt")
sax = sax_via_window(data$V1, 160, 4, 4, "exact", 0.001)
str = paste(unlist(sax), collapse =" ")
grammar = str_to_repair_grammar(str)

str1 = grammar[[1]]$rule_string
for(i in c(2:95)){
	str1 = gsub(paste(" R", i - 1, " ", sep = ""), paste(" ", grammar[[i]]$expanded_rule_string, " ", sep = ""), str1)
}

source("http://bioconductor.org/biocLite.R")