
import numpy 
import random

class Herbivore(object):
    '''
    Herbivore(Planteater) class for ecology simulation
    '''
    mutationRateMaxAge=0
    mutationRateMaxHunger=0
    mutationRateDefenseFactor=0
    mutationRateQualityOfSense=0
    mutationRateFavoriteTemperature=0
    mutationRateTemperatureSpan=0
    mutationRateChildProdFactor=0
    
    stepMaxAge=0
    stepMaxHunger=0
    stepDefenseFactor=0
    stepQualityOfSense=0
    stepFavoriteTemperature=0
    stepTemperatureSpan=0
    stepChildProdFactor=0
    
    minMaxAge=0
    minMaxHunger=0
    minDefenseFactor=0
    minQualityOfSense=0
    minFavoriteTemperature=0
    minTemperatureSpan=0
    minChildProdFactor=0
    
    maxMaxAge=0
    maxMaxHunger=0
    maxDefenseFactor=0
    maxQualityOfSense=0
    maxFavoriteTemperature=0
    maxTemperatureSpan=0
    maxChildProdFactor=0

    reproductionHungerPercent=4
    reproductionTemperaturePercent=4
    
    reproductionPercent=150
    minAgeForReproduction=0.2
    maxAgeForReproduction=0.7
    eatAtHungerPercent=0.35

    verbose=False
    verboseDeath=True


    def __init__( self, y, x, maxAge, maxHunger,\
                  defenseFactor, qualityOfSense,\
                  favoriteTemperature, temperatureSpan, childProdFactor, phyloName, phyloId, birthMonth ):
        self.age=0
        self.hunger=0
        self.x = x
        self.y = y
        self.maxAge = maxAge
        self.maxHunger = maxHunger
        self.defenseFactor = defenseFactor
        self.qualityOfSense = qualityOfSense
        self.favoriteTemperature = favoriteTemperature
        self.temperatureSpan = temperatureSpan
        self.childProductionFactor = childProdFactor
        self.phyloName = phyloName
        self.phyloId = phyloId

        self.kidCount=0
        
        self.reproductionProgress=0
       
        self.deathReason=""
        self.birthMonth=birthMonth
        self.deathMonth=0
        
        
    def getCSVEntry( self ):
        return [self.phyloName,self.age,self.maxAge,self.hunger,self.maxHunger,
                self.defenseFactor,self.qualityOfSense,self.favoriteTemperature,
                self.temperatureSpan,self.childProductionFactor,self.kidCount,
                self.birthMonth,self.deathMonth,self.deathReason]
                
    @staticmethod
    def getCSVHeader():
        return ["Phylo Name","Age","Max Age","Hunger","Max Hunger",
                "Defense Factor","Quality Of Sense","Favorite Temperature",
                "Temperature Span","Child Production Factor","Kid Count",
                "Birth Month","Death Month","Death Reason"]
        
    @staticmethod
    def setReproductionAgeRestrictions( minAFR, maxAFR ):
        Herbivore.minAgeForReproduction=minAFR# percent values of maxAge
        Herbivore.maxAgeForReproduction=maxAFR# percent values of maxAge
        
    @staticmethod
    def setEatAtHungerPercent( eAHP ):
        Herbivore.eatAtHungerPercent=eAHP
        
    @staticmethod
    def setRateOfMutation( mRMA, mRMH, mRDF, mRQOS, mRFT, mRTS, mRCPF ):
        Herbivore.mutationRateMaxAge=mRMA
        Herbivore.mutationRateMaxHunger=mRMH
        Herbivore.mutationRateDefenseFactor=mRDF
        Herbivore.mutationRateQualityOfSense=mRQOS
        Herbivore.mutationRateFavoriteTemperature=mRFT
        Herbivore.mutationRateTemperatureSpan=mRTS
        Herbivore.mutationRateChildProdFactor=mRCPF

    
    @staticmethod
    def setStepMutationVals( sRMA, sMH, sDF, sQOS, sFT, sTS, sCPF ):
        Herbivore.stepMaxAge=sRMA
        Herbivore.stepMaxHunger=sMH
        Herbivore.stepDefenseFactor=sDF
        Herbivore.stepQualityOfSense=sQOS
        Herbivore.stepFavoriteTemperature=sFT
        Herbivore.stepTemperatureSpan=sTS
        Herbivore.stepChildProdFactor=sCPF
    
    @staticmethod
    def setMinMutationVals( minRMA, minMH, minDF, minQOS, minFT, minTS, minCPF ):
        Herbivore.minMaxAge=minRMA
        Herbivore.minMaxHunger=minMH
        Herbivore.minDefenseFactor=minDF
        Herbivore.minQualityOfSense=minQOS
        Herbivore.minFavoriteTemperature=minFT
        Herbivore.minTemperatureSpan=minTS
        Herbivore.minChildProdFactor=minCPF
    
    @staticmethod
    def setMaxMutationVals( maxRMA, maxMH, maxDF, maxQOS, maxFT, maxTS, maxCPF ):
        Herbivore.maxMaxAge=maxRMA
        Herbivore.maxMaxHunger=maxMH
        Herbivore.maxDefenseFactor=maxDF
        Herbivore.maxQualityOfSense=maxQOS
        Herbivore.maxFavoriteTemperature=maxFT
        Herbivore.maxTemperatureSpan=maxTS
        Herbivore.maxChildProdFactor=maxCPF
        
    @staticmethod
    def setNeededPercentForReproduction( rP ):
        Herbivore.reproductionPercent=rP

    @staticmethod
    def setReproductionInfluencePercents( hungerPercent, temperaturePercent ):
        Herbivore.reproductionHungerPercent=hungerPercent
        Herbivore.reproductionTemperaturePercent=temperaturePercent 
    
    @staticmethod
    def setVerbose( v ):
        Herbivore.verbose=v
        
    @staticmethod
    def setVerboseDeath( v ):
        Herbivore.verboseDeath=v
        
    def setDeathMonth( self, deathMonth ):
        self.deathMonth=deathMonth
        
    def tick( self, currentTemperature  ):
        self.age+=1
        self.hunger+=1
        self.calcReproductionProgress( currentTemperature )
        # print( "HERBIVOR -- TICK -- TODO" )


    def calcReproductionProgress( self, currentTemperature ):
                
        if Herbivore.verbose:
            print( "Plant::reproduction currentTemperature=", currentTemperature )
        
        # at least:
        # + hp% if hunger==0; 0% if hunger==maxHunger
        # + tp% if perfect temperature
        hp=Herbivore.reproductionHungerPercent
        tp=Herbivore.reproductionTemperaturePercent
        
        tmpValHunger=hp*( 1-self.hunger/self.maxHunger )
        
        if Herbivore.verbose:
            print( "Herbivore::reproduction cause Hunger=", tmpValHunger)
        temperatureDeviation=abs(currentTemperature-self.favoriteTemperature)        
        tmpValTemp=tp*(1-(temperatureDeviation/self.temperatureSpan))
        if Herbivore.verbose:
            print( "Plant::reproduction cause Temp=", tmpValTemp)
            
        tmpSum=tmpValHunger+tmpValTemp
        tmpProd=tmpValHunger*tmpValTemp
        self.reproductionProgress+=max( tmpSum, tmpProd )
        if Herbivore.verbose:
            print( "Herbivore::reproductionProgress=", self.reproductionProgress-max( tmpSum, tmpProd ), "+", max( tmpSum, tmpProd ) )
    
    def getSpeed( self ):
        return( self.speed )
        
    def getQualityOfSense( self ):
        return( self.qualityOfSense )

    def deadDueRival( self, month ):
        if Herbivore.verboseDeath:
            print( "Herbivore dead, fighting!" )
        self.deathReason="age"
        self.setDeathMonth( month )
        
    def dead( self, currentTemperature ):
            
        if self.age>self.maxAge:
            if Herbivore.verboseDeath:
                print( "Herbivore dead, age!" )
            self.deathReason="age"
            return( True )
                
        if currentTemperature > ( self.favoriteTemperature + self.temperatureSpan ):
            if Herbivore.verboseDeath:
                print( "Herbivore dead, heat!" )
            self.deathReason="heat"
            return( True )
            
        if currentTemperature < ( self.favoriteTemperature - self.temperatureSpan ) :
            if Herbivore.verboseDeath:
                print( "Herbivore dead, coldness!" )
            self.deathReason="coldness"
            return( True )
            
        if self.maxHunger <= self.hunger :
            if Herbivore.verboseDeath:
                print( "Herbivore dead, famished!" )
            self.deathReason="hunger"
            return( True )
            
        return( False )
        
        
    def inTempSpan( self, t ):        
        if t > ( self.favoriteTemperature + self.temperatureSpan ):
            return( False )
            
        if t < ( self.favoriteTemperature - self.temperatureSpan ):
            return( False )
            
        return( True )
        
        
    def position( self ):
        ret=[]
        ret.append( self.y )
        ret.append( self.x )
        return( ret )
        
    def printSelf( self  ):
        if Herbivore.verbose :
            print( "X = ", self.x )
            print( "Y = ", self.y )
            print( "Phylo = ", self.phyloName )
        
    def chooseSite( self, foundFoodLocations, foundNormalLocations, temperatureList ):
        
        # Jump to first not too cold food site
        for fl in foundFoodLocations:
            if self.inTempSpan( temperatureList[fl[0]] ):
                self.x=fl[1]
                self.y=fl[0]
                return fl

        #No food found
        
        # Jump to first not too cold other site
        for nl in foundNormalLocations:
            if self.inTempSpan( temperatureList[nl[0]] ):
                self.x=nl[1]
                self.y=nl[0]
                return nl
        
        #No location found at all, no position changing
        return self.position()
        
        
    def getDefense( self ):
        return self.defenseFactor  
        
    def ate( self ):
        self.hunger=0
        
    def isHungry( self ):
        return( Herbivore.eatAtHungerPercent >= float(self.hunger)/self.maxHunger )
        
    def getChildList( self, month ):
        childList=[]
        
        if( Herbivore.minAgeForReproduction*self.maxAge > self.age ):
            return( childList ) # Empty!
            
        if( Herbivore.maxAgeForReproduction*self.maxAge < self.age ):
            return( childList ) # Empty!
        
        if( self.reproductionProgress<Herbivore.reproductionPercent ):
            return( childList ) # Empty!
            
        self.reproductionProgress=0
        
        for c in range( int( self.childProductionFactor ) ) :
            
            # Maybe mutate maximal age
            mA = self.maxAge
            if random.random() <= Herbivore.mutationRateMaxAge:
                # Mutate maximal age
                if random.random() <= 0.5:
                    if mA-Herbivore.stepMaxAge > Herbivore.minMaxAge:
                        mA-=Herbivore.stepMaxAge
                else:
                    if mA+Herbivore.stepMaxAge < Herbivore.maxMaxAge:
                        mA+=Herbivore.stepMaxAge   
                    
            # Maybe mutate maximal hunger
            mH = self.maxHunger
            if random.random() <= Herbivore.mutationRateMaxHunger:
                # Mutate maximal hunger
                if random.random() <= 0.5:
                    if mH-Herbivore.stepMaxHunger > Herbivore.minMaxHunger:
                        mH-=Herbivore.stepMaxHunger
                else:
                    if mH+Herbivore.stepMaxHunger < Herbivore.maxMaxHunger:
                        mH+=Herbivore.stepMaxHunger
                      
            # Maybe mutate defense factor
            dF = self.defenseFactor
            if random.random() <= Herbivore.mutationRateDefenseFactor:
                # Mutate defense factor
                if random.random() <= 0.5:
                    if dF-Herbivore.stepDefenseFactor > Herbivore.minDefenseFactor:
                        dF-=Herbivore.stepDefenseFactor
                else:
                    if dF+Herbivore.stepDefenseFactor < Herbivore.maxDefenseFactor:
                        dF+=Herbivore.stepDefenseFactor 
                    
            # Maybe mutate quality of sense
            qOS=self.qualityOfSense
            if random.random() <= Herbivore.mutationRateQualityOfSense:
                # Mutate quality of sense
                if random.random() <= 0.5:
                    if qOS-Herbivore.stepQualityOfSense > Herbivore.minQualityOfSense:
                        qOS-=Herbivore.stepQualityOfSense
                else:
                    if qOS+Herbivore.stepQualityOfSense < Herbivore.maxQualityOfSense:
                        qOS+=Herbivore.stepQualityOfSense
                    
            # Maybe mutate favorite temperature
            fT =self.favoriteTemperature
            if random.random() <= Herbivore.mutationRateFavoriteTemperature:
                # Mutate favorite temperature
                if random.random() <= 0.5:
                    if fT-Herbivore.stepFavoriteTemperature > Herbivore.minFavoriteTemperature:
                        fT-=Herbivore.stepFavoriteTemperature
                else:
                    if fT+Herbivore.stepFavoriteTemperature < Herbivore.maxFavoriteTemperature:
                        fT+=Herbivore.stepFavoriteTemperature 
                    
            # Maybe mutate temperature span
            tS =self.temperatureSpan
            if random.random() <= Herbivore.mutationRateTemperatureSpan:
                # Mutate temperature span
                if random.random() <= 0.5:
                    if tS-Herbivore.stepTemperatureSpan > Herbivore.minTemperatureSpan:
                        tS-=Herbivore.stepTemperatureSpan
                else:
                    if tS+Herbivore.stepTemperatureSpan < Herbivore.maxTemperatureSpan:
                        tS+=Herbivore.stepTemperatureSpan
                         
            # Maybe mutate child production factor
            cPF =self.childProductionFactor
            if random.random() <= Herbivore.mutationRateChildProdFactor:
                # Mutate child production factor
                if random.random() <= 0.5:
                    if cPF-Herbivore.stepChildProdFactor > Herbivore.minChildProdFactor:
                        cPF-=Herbivore.stepChildProdFactor
                else:
                    if cPF+Herbivore.stepChildProdFactor < Herbivore.maxChildProdFactor:
                        cPF+=Herbivore.stepChildProdFactor
                         
            # Herbivore kids will get the position from their parents.
            self.kidCount+=1
            tmpKidHerbivore=Herbivore( self.y, self.x, 
                              mA, mH, dF, qOS, fT, tS, cPF, 
                              self.phyloName+"-"+str(self.kidCount), self.phyloId, month )
            childList.append( tmpKidHerbivore )
        return( childList )
          
        
        
        
        
        