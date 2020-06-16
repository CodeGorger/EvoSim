
import random

class Plant(object):
    '''
    Plant class for ecology simulation
    '''

    mutationRateMaxWaterDistance=0
    mutationRateSeedProdFactor=0
    mutationRateSeedLightness=0
    mutationRateMaxAge=0
    mutationRateFoodChunks=0
    mutationRateDefenseFactor=0
    mutationRateFavoriteTemperature=0
    mutationRateTemperatureSpan=0
    
    stepMaxWaterDistance=0
    stepSeedProdFactor=0
    stepSeedLightness=0
    stepMaxAge=0
    stepFoodChunks=0
    stepDefenseFactor=0
    stepFavoriteTemperature=0
    stepTemperatureSpan=0
    
    minMaxWaterDistance=0
    minSeedProdFactor=0
    minSeedLightness=0
    minMaxAge=0
    minFoodChunks=0
    minDefenseFactor=0
    minFavoriteTemperature=0
    minTemperatureSpan=0
    
    maxMaxWaterDistance=0
    maxSeedProdFactor=0
    maxSeedLightness=0
    maxMaxAge=0
    maxFoodChunks=0
    maxDefenseFactor=0
    maxFavoriteTemperature=0
    maxTemperatureSpan=0

    reproductionWaterDistancePercent=5
    reproductionTemperaturePercent=5
    
    reproductionPercent=100
    
    verbose=False
    verboseDeath=True

    def __init__( self, y, x, maxWaterDistance, seedProdFactor, seedLightness, maxAge,\
                  foodChunks, defenseFactor, favoriteTemperature,\
                  temperatureSpan, phyloName, phyloId, birthMonth ):
        self.age=0
        self.reproductionProgress=0
        self.x = x
        self.y = y
        self.maxWaterDistance = maxWaterDistance
        self.seedProdFactor = seedProdFactor
        self.seedLightness = seedLightness
        self.maxAge = maxAge
        self.foodChunks = foodChunks
        self.defenseFactor = defenseFactor
        self.favoriteTemperature = favoriteTemperature
        self.temperatureSpan = temperatureSpan
        self.phyloName=phyloName
        self.phyloId=phyloId
        
        self.isFoodChunks=self.foodChunks
        self.isWaterDistance = 1 #??? we can't know this here        
        
        self.kidCount=0

        self.deathReason=""
        self.birthMonth=birthMonth
        self.deathMonth=0
    
    def getCSVEntry( self ):
        return [self.phyloName,self.age,self.maxAge,self.isWaterDistance,self.maxWaterDistance,
                self.defenseFactor,self.foodChunks,self.favoriteTemperature,
                self.temperatureSpan,self.seedLightness,self.seedProdFactor,self.kidCount,
                self.birthMonth,self.deathMonth,self.deathReason]
                
    @staticmethod
    def getCSVHeader():
        return ["Phylo Name", "Age", "Max Age", "Is Water Distance", "Max Water Distance",
                "Defense Factor", "Food Chunks","Favorite Temperature",
                "Temperature Span", "Seed Lightness", "Seed Prod Factor", "Kid Count",
                "Birth Month", "Death Month", "Death Reason"]
        
    @staticmethod
    def setRateOfMutation( mRMWD, mRSPF, mRSL, mRMA, mRFC, mRDF, mRFT, mRTS ):
        Plant.mutationRateMaxWaterDistance=mRMWD
        Plant.mutationRateSeedProdFactor=mRSPF
        Plant.mutationRateSeedLightness=mRSL
        Plant.mutationRateMaxAge=mRMA
        Plant.mutationRateFoodChunks=mRFC
        Plant.mutationRateDefenseFactor=mRDF
        Plant.mutationRateFavoriteTemperature=mRFT
        Plant.mutationRateTemperatureSpan=mRTS
    
    @staticmethod
    def setStepMutationVals( sMWD, sSPF, sSL, sMA, sFC, sDF, sFT, sTS ):
        Plant.stepMaxWaterDistance=sMWD
        Plant.stepSeedProdFactor=sSPF
        Plant.stepSeedLightness=sSL
        Plant.stepMaxAge=sMA
        Plant.stepFoodChunks=sFC
        Plant.stepDefenseFactor=sDF
        Plant.stepFavoriteTemperature=sFT
        Plant.stepTemperatureSpan=sTS
    
    @staticmethod
    def setMinMutationVals( minMWD, minSPF, minSL, minMA, minFC, minDF, minFT, minTS ):
        Plant.minMaxWaterDistance=minMWD
        Plant.minSeedProdFactor=minSPF
        Plant.minSeedLightness=minSL
        Plant.minMaxAge=minMA
        Plant.minFoodChunks=minFC
        Plant.minDefenseFactor=minDF
        Plant.minFavoriteTemperature=minFT
        Plant.minTemperatureSpan=minTS
    
    @staticmethod
    def setMaxMutationVals( maxMWD, maxSPF, maxSL, maxMA, maxFC, maxDF, maxFT, maxTS ):
        Plant.maxMaxWaterDistance=maxMWD
        Plant.maxSeedProdFactor=maxSPF
        Plant.maxSeedLightness=maxSL
        Plant.maxMaxAge=maxMA
        Plant.maxFoodChunks=maxFC
        Plant.maxDefenseFactor=maxDF
        Plant.maxFavoriteTemperature=maxFT
        Plant.maxTemperatureSpan=maxTS
        
    @staticmethod
    def setNeededPercentForReproduction( rP ):
        Plant.reproductionPercent=rP

    @staticmethod
    def setReproductionInfluencePercents( waterDistancePercent, temperaturePercent ):
        Plant.reproductionWaterDistancePercent=waterDistancePercent
        Plant.reproductionTemperaturePercent=temperaturePercent 
        
    @staticmethod
    def setVerbose( v ):
        Plant.verbose=v
        
    @staticmethod
    def setVerboseDeath( v ):
        Plant.verboseDeath=v
        
    def setDeathMonth( self, deathMonth ):
        self.deathMonth=deathMonth
        
    def getDefense( self ):
        return self.defenseFactor        
        
    def eaten( self ):
        self.isFoodChunks-=1        
        
    def setWaterDistance( self, distance ):
        self.isWaterDistance=distance
        
    def getChildList( self, month ):
        childList=[]
        if( self.reproductionProgress>Plant.reproductionPercent ):
            self.reproductionProgress-=Plant.reproductionPercent
            
            for c in range( int( self.seedProdFactor ) ):
                
                # Maybe mutate water distance
                mWD=self.maxWaterDistance
                if random.random() <= Plant.mutationRateMaxWaterDistance:
                    # Mutate water distance
                    if random.random() <= 0.5:
                        if mWD-Plant.stepMaxWaterDistance > Plant.minMaxWaterDistance:
                            mWD-=Plant.stepMaxWaterDistance
                    else:
                        if mWD+Plant.stepMaxWaterDistance < Plant.maxMaxWaterDistance:
                            mWD+=Plant.stepMaxWaterDistance
                      
                # Maybe mutate seed production factor
                sPF=self.seedProdFactor
                if random.random() <= Plant.mutationRateSeedProdFactor:
                    # Mutate seed production factor
                    if random.random() <= 0.5:
                        if sPF-Plant.stepSeedProdFactor > Plant.minSeedProdFactor:
                            sPF-=Plant.stepSeedProdFactor
                    else:
                        if sPF+Plant.stepSeedProdFactor < Plant.maxSeedProdFactor:
                            sPF+=Plant.stepSeedProdFactor
                        
                # Maybe mutate seed lightness
                sL=self.seedLightness
                if random.random() <= Plant.mutationRateSeedLightness:
                    # Mutate seed lightness
                    if random.random() <= 0.5:
                        if sL-Plant.stepSeedLightness > Plant.minSeedLightness:
                            sL-=Plant.stepSeedLightness
                    else:
                        if sL+Plant.stepSeedLightness < Plant.maxSeedLightness:
                            sL+=Plant.stepSeedLightness
                
                # Maybe mutate maximal age
                mA =self.maxAge
                if random.random() <= Plant.mutationRateMaxAge:
                    # Mutate maximal age
                    if random.random() <= 0.5:
                        if mA-Plant.stepMaxAge > Plant.minMaxAge:
                            mA-=Plant.stepMaxAge
                    else:
                        if mA+Plant.stepMaxAge < Plant.maxMaxAge:
                            mA+=Plant.stepMaxAge    
                            
                # Maybe mutate food chunks
                fC =self.foodChunks
                if random.random() <= Plant.mutationRateFoodChunks:
                    # Mutate food chunks
                    if random.random() <= 0.5:
                        if fC-Plant.stepFoodChunks > Plant.minFoodChunks:
                            fC-=Plant.stepFoodChunks
                    else:
                        if fC+Plant.stepFoodChunks < Plant.maxFoodChunks:
                            fC+=Plant.stepFoodChunks    
                            
                # Maybe mutate defense factor
                dF =self.defenseFactor
                if random.random() <= Plant.mutationRateDefenseFactor:
                    # Mutate defense factor
                    if random.random() <= 0.5:
                        if dF-Plant.stepDefenseFactor > Plant.minDefenseFactor:
                            dF-=Plant.stepDefenseFactor
                    else:
                        if dF+Plant.stepDefenseFactor < Plant.maxDefenseFactor:
                            dF+=Plant.stepDefenseFactor 
                            
                # Maybe mutate favorite temperature
                fT =self.favoriteTemperature
                if random.random() <= Plant.mutationRateFavoriteTemperature:
                    # Mutate favorite temperature
                    if random.random() <= 0.5:
                        if fT-Plant.stepFavoriteTemperature > Plant.minFavoriteTemperature:
                            fT-=Plant.stepFavoriteTemperature
                    else:
                        if fT+Plant.stepFavoriteTemperature < Plant.maxFavoriteTemperature:
                            fT+=Plant.stepFavoriteTemperature 
                        
                # Maybe mutate temperature span
                tS =self.temperatureSpan
                if random.random() <= Plant.mutationRateTemperatureSpan:
                    # Mutate temperature span
                    if random.random() <= 0.5:
                        if tS-Plant.stepTemperatureSpan > Plant.minTemperatureSpan:
                            tS-=Plant.stepTemperatureSpan
                    else:
                        if tS+Plant.stepTemperatureSpan < Plant.maxTemperatureSpan:
                            tS+=Plant.stepTemperatureSpan
                        
                # The here created kids will get the position from their parents first,
                # because it is needed to find a seedlocation in max-distance range of
                # its parents position:
                self.kidCount+=1
                tmpKidPlant=Plant( self.y, self.x, 
                                   mWD, sPF, sL, mA, fC, dF, fT, tS, 
                                  self.phyloName+"-"+str(self.kidCount), self.phyloId, month )
                childList.append( tmpKidPlant )
        return( childList )
        
    def tick( self, currentTemperature ):
        self.age+=1
        self.calcReproductionProgress( currentTemperature )
    
    def getSeedLightness( self ):
        return( self.seedLightness )
        
    def calcReproductionProgress( self, currentTemperature ):
                
        if Plant.verbose:
            print( "Plant::reproduction currentTemperature=", currentTemperature )
        
        # at least:
        # + wp% if water is distance 1 +0% if water is at maxWaterDistance
        # + tp% if perfect temperature
        wp=Plant.reproductionWaterDistancePercent
        tp=Plant.reproductionTemperaturePercent
        if self.maxWaterDistance == 1:
            self.maxWaterDistance+=0.1
        tmpValWater=wp*(1-(self.isWaterDistance-1)/(self.maxWaterDistance-1))
        
        if Plant.verbose:
            print( "Plant::reproduction cause Water=", tmpValWater)
        temperatureDeviation=abs(currentTemperature-self.favoriteTemperature)        
        tmpValTemp=tp*(1-(temperatureDeviation/self.temperatureSpan))
        if Plant.verbose:
            print( "Plant::reproduction cause Temp=", tmpValTemp)
            
        tmpSum=tmpValWater+tmpValTemp
        tmpProd=tmpValWater*tmpValTemp
        self.reproductionProgress+=max( tmpSum, tmpProd )
        if Plant.verbose:
            print( "Plant::reproductionProgress=", self.reproductionProgress-max( tmpSum, tmpProd ), "+", max( tmpSum, tmpProd ) )
        
        
    def dead( self, currentTemperature ):
        if self.age>self.maxAge:
            if Plant.verboseDeath:
                print( "Plant dead, age!" )
            self.deathReason="age"
            return( True )
                
        if self.maxWaterDistance<self.isWaterDistance:
            if Plant.verboseDeath:
                print( "Plant dead, aridification!" )#(",self.maxWaterDistance,",",self.isWaterDistance,")" )
            self.deathReason="aridification"
            return( True )
            
        if currentTemperature > ( self.favoriteTemperature + self.temperatureSpan ):
            if Plant.verboseDeath:
                print( "Plant dead, heat!" )
            self.deathReason="heat"
            return( True )
            
        if currentTemperature < ( self.favoriteTemperature - self.temperatureSpan ) :
            if Plant.verboseDeath:
                print( "Plant dead, coldness!" )
            self.deathReason="coldness"
            return( True )
            
        if self.isFoodChunks <= 0 :
            if Plant.verboseDeath:
                print( "Plant dead, eaten!" )
            self.deathReason="eaten"
            return( True )
            
        return( False )
        
    def position( self ):
        ret=[]
        ret.append( self.y )
        ret.append( self.x )
        return( ret )
        
    def setPosition( self, y, x ):
        self.y = y
        self.x = x
        
    def printSelf( self ):
        if Plant.verbose:
            print( "              Phylo = ", self.phyloName )
            print( "                X,Y = ", self.x, self.y )
            print( "        Replication = ", self.reproductionProgress )
            print( "   maxWaterDistance = ", self.maxWaterDistance )
            print( "     seedProdFactor = ", self.seedProdFactor )
            print( "      seedLightness = ", self.seedLightness )
            print( "             maxAge = ", self.maxAge )
            print( "         foodChunks = ", self.foodChunks )
            print( "      defenseFactor = ", self.defenseFactor )
            print( "favoriteTemperature = ", self.favoriteTemperature )
            print( "    temperatureSpan = ", self.temperatureSpan )
            
            