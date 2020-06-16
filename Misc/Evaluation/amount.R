
library("ggplot2")

setwd("/home/simon/Desktop/StudiumTuebingen/CMEE/FinalProject/amountPlotting")
plantAmountData<-read.csv("PlantAmountHistory.csv",header=T)

g <- ggplot(data=plantAmountData, aes(x=month, y=plantamount, colour=cut)) +
     geom_path( colour="#FF0000", size=2, lineend = "round" )

myplot <- ggplot(radialData, aes(r, theta)) + 
  # I do not want nodes.
  # geom_point(size=3.5) +
  coord_polar(theta="y", start = 3/2*pi, direction=-1)+
  scale_x_continuous(breaks=seq(0,1), lim=c(0, max(radialData$r)+1)) + 
  scale_y_continuous(breaks=round(seq(0, 360, by=45),2), expand=c(0,0), lim=c(0,2*180)) +
  # I do not want my nodes labeled.
  # geom_text(aes(label=rownames(radialData)), size=4.4, hjust=0.5, vjust=-1) + 
  geom_path( colour="#FF0000", size=2, lineend = "round" )
myplot
