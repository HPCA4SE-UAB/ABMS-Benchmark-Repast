df <- read.table("node-100.data")

require(ggplot2)

p <- ggplot(df, aes(df$V1,df$V2)) + labs(x = "X") + labs(y = "Y") + labs(title = "Distribució agents RepastHPC")
p <- p + stat_bin2d(bins = 20)
pdf("AgentsDistribRepast.pdf")
plot(p)
dev.off()
