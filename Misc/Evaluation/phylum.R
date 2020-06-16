

library(ade4)

createPhylumString <- function(filename)
{
  carnivorPhylumData<-read.csv(filename,header=T)
  carnivorPhylumData$level<-as.numeric(1)
  carnivorPhylumData$children<-FALSE
  
  # Preparing table (i.e. creating level column and children column)
  for( i in 2:nrow( carnivorPhylumData ) )
  {
    carnivorPhylumData[carnivorPhylumData[i,3]+1,5]<-TRUE 
  }
  for( i in 2:nrow( carnivorPhylumData ) )
  {
    if( carnivorPhylumData[i,5] )
    {
      carnivorPhylumData[i,4]<-carnivorPhylumData[carnivorPhylumData[i,3]+1,4]+2
    }
    else
    {
      carnivorPhylumData[i,4]<-carnivorPhylumData[carnivorPhylumData[i,3]+1,4]+1
    }
  }
  
#  Example format:
#   name prename preid level children
#   1     C-0    <NA>    -1     1     TRUE
#   2     C-1     C-0     0     2    FALSE
#   3     C-2     C-0     0     3     TRUE
#   4    C-2A     C-2     2     5     TRUE
#   5  C-2A-Z    C-2A     3     6    FALSE
#   6  C-2A-Y    C-2A     3     6    FALSE
#   7    C-2B     C-2     2     5     TRUE
#   8  C-2B-Z    C-2B     6     6    FALSE
#   9  C-2B-Y    C-2B     6     6    FALSE
#   10 C-2B-X    C-2B     6     6    FALSE
#   11    C-3     C-0     0     2    FALSE
#   12    C-4     C-0     0     3     TRUE
#   13   C-4A     C-4    11     5     TRUE
#   14 C-4A-Z    C-4A    12     6    FALSE
  
  
  currentLevel<-0
  phylumString<-""
  # Create phylotyp tree string
  for( i in 1:nrow( carnivorPhylumData ) )
  {
    # print( paste( "i:", i, " currentLevel:", currentLevel, " carnivorPhylumData[i,4]:", carnivorPhylumData[i,4], sep="" ) )
    # Create all opening brackets
    if( currentLevel<carnivorPhylumData[i,4] )
    {
      for( j in currentLevel:(carnivorPhylumData[i,4]-1) )
      {
        phylumString<- paste( phylumString, "(", sep="" )
        currentLevel<-currentLevel+1
      }
    }
    
    #Insert phylotype name
    phylumString<- paste( phylumString, carnivorPhylumData[i,1], sep="" )
    
    # Create all closing brackets
    if( i<nrow(carnivorPhylumData) && currentLevel>carnivorPhylumData[i+1,4] )
    {
      # print( paste( "i:", i, " currentLevel:", currentLevel, " carnivorPhylumData[i+1,3]+1:", carnivorPhylumData[i+1,3]+1, sep="" ) )
      # print( paste( " (carnivorPhylumData[carnivorPhylumData[i+1,3]+1,4]+1):", (carnivorPhylumData[carnivorPhylumData[i+1,3]+1,4]+1), sep="" ) )
      for( j in currentLevel:(carnivorPhylumData[carnivorPhylumData[i+1,3]+1,4]+2) )
      {
        phylumString<- paste( phylumString, ")", sep="" )
        currentLevel<-currentLevel-1
      }
    }
    if( i<nrow(carnivorPhylumData) )
    {
      # Add comma
      phylumString<- paste( phylumString, ",", sep="" )
    }
  }
  if( currentLevel>0 )
  {
    for( j in currentLevel: 1 )
    {
      phylumString<- paste( phylumString, ")", sep="" )
      currentLevel<-currentLevel-1
    }
  }
  phylumString<- paste( phylumString, ";", sep="" )
  return(phylumString)
}


setwd("/home/simon/Desktop/StudiumTuebingen/CMEE/FinalProject/phylumTreeTest")
CarnivorPhylumTreeString = createPhylumString( "CarnivorTree2.dat" )
print( CarnivorPhylumTreeString )

#phylumString.phy <- newick2phylog(phylumString)
#print(phylumString.phy)
#plot(phylumString.phy)
#data(newick.eg)

radial.phylog( newick2phylog( CarnivorPhylumTreeString, FALSE ), cnode=1, clabel.l=0.8 )

