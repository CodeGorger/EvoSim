
import numpy
import random
import math

class LandWaterMap(object):
    '''
    Map class for ecology simulation
    '''
    
    verbose=True
    
    def __init__(self, height, width, probabilityLand, alpha=0.1 ):
        self.sigmaRatioThreshold=0.1
        tmpM=numpy.zeros(shape=(height,width))
        self.m=numpy.zeros(shape=(height,width))
        # self.m[ -y- , -x- ]
        for i in range(height):
            for j in range(width):
                if probabilityLand > random.random():
                    tmpM[i,j]=1
        
        #print "width: ", width
        #print "height: ", height
        for i in range(height):
            for j in range(width):
                prob=0
                if i>0 and j>0:
                    prob=prob+tmpM[i-1,j-1]*alpha
                if j>0:
                    prob=prob+tmpM[i,j-1]*alpha
                if i>0:
                    prob=prob+tmpM[i-1,j]*alpha
                if i<height-1 and j<width-1:
                    prob=prob+tmpM[i+1,j+1]*alpha
                if j<width-1:
                    prob=prob+tmpM[i,j+1]*alpha
                if i<height-1:
                    prob=prob+tmpM[i+1,j]*alpha
                if i<height-1 and j>0:
                    prob=prob+tmpM[i+1,j-1]*alpha
                if i>0 and j<width-1:
                    prob=prob+tmpM[i-1,j+1]*alpha
                prob=prob+tmpM[i,j]*alpha
                
                if 0.5 < prob:                    
                    self.m[i,j]=tmpM[i,j]
                    
        
        foundSpawn=False
        while foundSpawn == False:
            self.spawnX=math.floor(random.normalvariate(width/2, width*1/8))
            self.spawnY=math.floor(random.normalvariate(height/2, height*1/8))
            #print "spawn X:", self.spawnX
            #print "spawn Y:", self.spawnY
            #print "X - self.m.shape[1]:", self.m.shape[1]
            #print "Y - self.m.shape[0]:", self.m.shape[0]
            if self.spawnX>=0 and self.spawnX<self.m.shape[1] and self.spawnY>=0 and self.spawnY<self.m.shape[0] and 1==self.m[self.spawnY,self.spawnX]:
                foundSpawn=True
                # self.m[ -y- , -x- ] && Y - self.m.shape[0] && X - self.m.shape[1]
            
        # numpy.savetxt( "LW.txt", tmpM )
        # numpy.savetxt( "isingLW.txt", self.m )
        # print tmpM
        # print self.m
            
    @staticmethod
    def setVerbose( v ):
        LandWaterMap.verbose=v
        
    def getMap( self ):
        return( self.m )
        
    def isLand( self, y, x ):
        return( self.m[ y, x ] )
        # self.m[ -y- , -x- ] && Y - self.m.shape[0] && X - self.m.shape[1]
        
    def searchFirstSpawn( self, sigmaRatio, tries=-1 ):
        tries=math.floor( tries )
        
        if sigmaRatio < self.sigmaRatioThreshold:
            sigmaRatio=self.sigmaRatioThreshold
           
        while tries != 0:
            if tries > 0:
                tries=tries-1
            tryX=math.floor( random.normalvariate( self.spawnX, self.m.shape[1]*sigmaRatio ) )
            tryY=math.floor( random.normalvariate( self.spawnY, self.m.shape[0]*sigmaRatio ) )
            if tryX>=0 and tryX<self.m.shape[1] and tryY>=0 and tryY<self.m.shape[0]:
                if 1==self.m[tryY,tryX]:
                    ret=[]
                    ret.append( int(tryY) )
                    ret.append( int(tryX) )
                    return( ret )
        # self.m[ -y- , -x- ] && Y - self.m.shape[0] && X - self.m.shape[1]
                    
    def searchSpawn( self, tries, maxDist, ySearchPos, xSearchPos ):
        tries=math.floor( tries )
        
        while tries != 0:
            if tries > 0:
                if LandWaterMap.verbose:
                    print( "tries left:", tries )
                tries=tries-1
            tryX=math.floor( random.uniform(xSearchPos-maxDist, xSearchPos+maxDist) )
            tryY=math.floor( random.uniform(ySearchPos-maxDist, ySearchPos+maxDist) )
            if tryX>=0 and tryX<self.m.shape[1] and tryY>=0 and tryY<self.m.shape[0]:
                if LandWaterMap.verbose:
                    print( "in bounds of map" )
                xSquareDiff=(tryX-xSearchPos)*(tryX-xSearchPos)
                ySquareDiff=(tryY-ySearchPos)*(tryY-ySearchPos)
                if maxDist >= numpy.sqrt( xSquareDiff + ySquareDiff ):
                    if LandWaterMap.verbose:
                        print( "in max Distance(lightness) range" )
                    if 1==self.m[tryY,tryX]:
                        if LandWaterMap.verbose:
                            print( "is land" )
                        ret=[]
                        ret.append( int(tryY) )
                        ret.append( int(tryX) )
                        return( ret )
        # self.m[ -y- , -x- ] && Y - self.m.shape[0] && X - self.m.shape[1]
                    
        
    def getWaterDistance( self, yp, xp ):
        foundWater=False
        waterAtX=0
        waterAtY=0
        sigma=0.3
        while foundWater==False:
            waterAtX = math.floor( random.normalvariate(xp, sigma) )
            waterAtY = math.floor( random.normalvariate(yp, sigma) )
            if waterAtX>=0 and waterAtX<self.m.shape[1] and waterAtY>=0 and waterAtY<self.m.shape[0]:
                if 0==self.m[waterAtY,waterAtX]:
                    foundWater=True
            sigma+=0.1
        
        difSqX=(waterAtX-xp)*(waterAtX-xp)
        difSqY=(waterAtY-yp)*(waterAtY-yp)
        return numpy.sqrt( difSqX+difSqY )
        
    def getSpawn( self ):
        ret=[]
        ret.append( self.spawnY )
        ret.append( self.spawnX )
        return( ret )
        
        
        
        
        
        
        
        
        
        
        