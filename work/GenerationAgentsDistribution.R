df <- read.table("node-100.data")

require(ggplot2)

p <- ggplot(df, aes(df$V1,df$V2)) + labs(x = "X") + labs(y = "Y") + labs(title = "RepastHPC agent distribution")
p <- p + stat_bin2d(bins = 20) + scale_fill_gradient(low = "lightblue", high = "red")
pdf("AgentsDistribRepast.pdf")
plot(p)
dev.off()
