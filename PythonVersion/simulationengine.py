
import herbivore
import plant
import landwatermap
import random
import numpy
import matplotlib
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
import matplotlib.cm as cm
from matplotlib import colors
import csv
import time
 
from landwatermap import LandWaterMap
from herbivore import Herbivore
from plant import Plant
from copy import copy, deepcopy



class SimulationEngine(object):
    '''
    SimulationEngine class for ecology simulation
    '''
    
    verbose=True
    verboseImportant=False
        
    def __init__( self, world ):
        self.landwatermap=world # The actual map
        self.herbivores = []    # List of all plant eater
        # TODO: Delete this.        
        self.plants = []
        self.plantMap = [ [ None for i in range(world.m.shape[1])] for j in range(world.m.shape[0]) ]
#        self.herbivoreMap = np.zeros((world.shape[0],world.shape[1],0))
        self.tickCounter=0
        self.plantCounter=0
        self.herbivorCounter=0
        self.plantsExtinct=False
        self.herbivoreExtinct=False
        self.herbivoreSpawnStarted=False
        self.plantAbundanceHistory=[]
        self.herbivoreAbundanceHistory=[]
        self.temperatureList=[ 0 for i in range(world.m.shape[1])] 
        
        self.deadHerbivore=[]
        self.deadPlants=[]        
        self.CSVRequired=False
        
        self.screenOn=False
        
    def setCSVRequired( self, required ):
        self.CSVRequired=required
        
    def getAllHerbivoreExtinct( self ):
        return self.herbivoreExtinct

    def getAllPlantsExtinct( self ):
        return self.plantsExtinct

    @staticmethod
    def setSilent( s ):
        SimulationEngine.verboseImportant=False
        SimulationEngine.verbose=False        
        LandWaterMap.setVerbose( False )
        Plant.setVerbose( False )
        Plant.setVerboseDeath( False )
        Herbivore.setVerbose( False )
        Herbivore.setVerboseDeath( False )            
        if s>1:
            Plant.setVerboseDeath( True )
            Herbivore.setVerboseDeath( True )
            SimulationEngine.verboseImportant=True
        if s>2:
            SimulationEngine.verbose=True      
            LandWaterMap.setVerbose( True )
            Plant.setVerbose( True )
            Herbivore.setVerbose( True )
            
    # pFGMWD := plantFirstGenerationMaxWaterDistance
    # pFGSPF := plantFirstGenerationSeedProdFactor
    # pFGSL := plantFirstGenerationSeedLightness
    # pFGMA  := plantFirstGenerationMaxAge
    # pFGFC  := plantFirstGenerationFoodChunks
    # pFGDF  := plantFirstGenerationDefenseFactor
    # pFGFT  := plantFirstGenerationFavoriteTemperature
    # pFGTS  := plantFirstGenerationTemperatureSpan
    def spawnPlants( self, initialSpawnAmountPlants,\
                     pFGMWD, pFGSPF, pFGSL, pFGMA, pFGFC, pFGDF, pFGFT, pFGTS, month ):
        #Create -initialSpawnAmountPlants- amount of plants
        for i in range( initialSpawnAmountPlants ):
            failedSetTries=0
            badPlantPos=True
            #Retry finding a space until the place is good.
            while badPlantPos==True and failedSetTries<30:
                #Check the map for a random space near the spawn on land.
                tryPos = self.landwatermap.searchFirstSpawn( 0.2 )
                badPlantPos=False
                if self.plantMap[ tryPos[0] ][ tryPos[1] ] is not None: # Y, X
                    failedSetTries+=1
                    badPlantPos=True
                else:
                    p=Plant( tryPos[0], tryPos[1], pFGMWD,
                             pFGSPF, pFGSL, pFGMA, pFGFC, pFGDF, pFGFT, pFGTS,
                             "P-"+str(i), 1, month )
                    self.plantMap[ tryPos[0] ][ tryPos[1] ]=p
                    self.plantCounter+=1

        for pr in self.plantMap:
            for p in pr:
                if p is not None:
                    wd=self.landwatermap.getWaterDistance( p.position()[0], 
                                                           p.position()[1] ) # Y, X
                    p.setWaterDistance( wd )
                
        
    # hFGMA  := herbivoreFirstGenerationMaxAge
    # hFGMH  := herbivoreFirstGenerationMaxHunger
    # hFGDF  := herbivoreFirstGenerationDefenseFactor
    # hFGQOS := herbivoreFirstGenerationQualityOfSense
    # hFGFT  := herbivoreFirstGenerationFavoriteTemperature
    # hFGTS  := herbivoreFirstGenerationTemperatureSpan
    # hFGCPF := herbivoreFirstGenerationChildProductionFactor
    def spawnHerbivore( self, initialSpawnAmountHerbivore,
                        hFGMA, hFGMH, hFGDF, hFGQOS, hFGFT, hFGTS, hFGCPF, month ):
        for i in range( initialSpawnAmountHerbivore ):
             pos = self.landwatermap.searchFirstSpawn( 0.2 )
             self.herbivores.append( Herbivore( pos[0], pos[1], hFGMA, hFGMH,
                                     hFGDF, hFGQOS, hFGFT, hFGTS, hFGCPF,
                                     "H-"+str(i), #"phylonameTODO", 
                                     1, month ) ) #"phyloidTODO" ) )
        self.herbivoreSpawnStarted=True

            
            
    def showVid( self ):

        outputMap=deepcopy(self.landwatermap.getMap())
        
        for pr in self.plantMap:
            for p in pr:
                if p is not None:
                    pos=p.position()
                    outputMap[pos[0],pos[1]]=2|int(outputMap[pos[0],pos[1]])
                        
            for h in self.herbivores:
                pos=h.position()
                outputMap[pos[0],pos[1]]=4|int(outputMap[pos[0],pos[1]])
                
        myCmap = colors.ListedColormap(['#000080', # dark blue /    water
                                        '#008000', # dark green /   land
                                        '#4040A0', # blue /         water plant ( currently just as placeholder )
                                        '#00ff00', # light green /  land and plant
                                        
                                        '#000030', # darkest blue / water and herbivore
                                        '#0080ff', # teal blue /    land and herbivore
                                        '#00ff30', # teal /         water and plant and herbivore
                                        '#00ffff', # teal /         land and plant and herbivore
                                        
                                        '#ff0000'])# ? / spawn
                                 
        sP=self.landwatermap.getSpawn()
        outputMap[sP[0]][sP[1]]=8
 
        if self.screenOn==False:
            fig=plt.figure( figsize=(8.5, 4), dpi=80 )
        
        #fig, (mapPlt, tempPlot) = plt.subplots(1,2)
        gs1 = gridspec.GridSpec(1, 2, wspace=0.0, hspace=0.0)

        ax1 = plt.subplot(gs1[0, 0])     
        #plt.subplot(1, 2, 1)
        plt.imshow( outputMap, extent=(0, self.landwatermap.m.shape[1], 0, self.landwatermap.m.shape[0]),
                #aspect=1,
               cmap=myCmap, interpolation='nearest' )

        ax2 = plt.subplot(gs1[0,1])  
        ax2.get_xaxis().set_visible(False)
        l=len(self.temperatureList)
        step=l/5
        ax2.set_yticklabels( [str(round(self.temperatureList[int(step*5)-1],1)),
                          str(round(self.temperatureList[int(step*4)],1)),
                          str(round(self.temperatureList[int(step*3)],1)),
                          str(round(self.temperatureList[int(step*2)],1)),
                          str(round(self.temperatureList[int(step*1)],1)),
                      str(round(self.temperatureList[0] ,1))])

        plt.imshow( [list(i) for i in zip(*[self.temperatureList])], extent=(0, 1, 0, self.landwatermap.m.shape[0]))                       
        
        if self.screenOn==False:
            plt.ion()
            plt.show()
            self.screenOn=True
            
        plt.draw()
        
        
    def showDia( self ):

        outputMap=deepcopy(self.landwatermap.getMap())
        
        for pr in self.plantMap:
            for p in pr:
                if p is not None:
                    pos=p.position()
                    outputMap[pos[0],pos[1]]=2|int(outputMap[pos[0],pos[1]])
                        
            for h in self.herbivores:
                pos=h.position()
                outputMap[pos[0],pos[1]]=4|int(outputMap[pos[0],pos[1]])
                
        myCmap = colors.ListedColormap(['#000080', # dark blue /    water
                                        '#008000', # dark green /   land
                                        '#4040A0', # blue /         water plant ( currently just as placeholder )
                                        '#00ff00', # light green /  land and plant
                                        
                                        '#000030', # darkest blue / water and herbivore
                                        '#0080ff', # teal blue /    land and herbivore
                                        '#00ff30', # teal /         water and plant and herbivore
                                        '#00ffff', # teal /         land and plant and herbivore
                                        
                                        '#ff0000'])# ? / spawn
                                 
        sP=self.landwatermap.getSpawn()
        outputMap[sP[0]][sP[1]]=8
 
        fig=plt.figure( figsize=(8.5, 4), dpi=80 )
        
        #fig, (mapPlt, tempPlot) = plt.subplots(1,2)
        gs1 = gridspec.GridSpec(1, 2, wspace=0.0, hspace=0.0)

        ax1 = plt.subplot(gs1[0, 0])     
        #plt.subplot(1, 2, 1)
        plt.imshow( outputMap, extent=(0, self.landwatermap.m.shape[1], 0, self.landwatermap.m.shape[0]),
                #aspect=1,
               cmap=myCmap, interpolation='nearest' )

        ax2 = plt.subplot(gs1[0,1])  
        ax2.get_xaxis().set_visible(False)
        l=len(self.temperatureList)
        step=l/5
        ax2.set_yticklabels( [str(round(self.temperatureList[int(step*5)-1],1)),
                          str(round(self.temperatureList[int(step*4)],1)),
                          str(round(self.temperatureList[int(step*3)],1)),
                          str(round(self.temperatureList[int(step*2)],1)),
                          str(round(self.temperatureList[int(step*1)],1)),
                          str(round(self.temperatureList[0] ,1))])

        plt.imshow( [list(i) for i in zip(*[self.temperatureList])], extent=(0, 1, 0, self.landwatermap.m.shape[0]))                       
        
        plt.show()
            
        
    def setTemperature( self ):
        self.maxTemperature = random.normalvariate(40, 3)
        # hh ... height of map
        hh=self.landwatermap.m.shape[0]
        # hh/2 ... equator
        # hh*0.1*sin((self.tickCounter%12)/12*pi*2) ... change for month
        # mtp ... max temperature position
        mtp = numpy.sin((self.tickCounter%12)/12*numpy.pi*2)
        # d ... deviation
        d=random.normalvariate(0.1, 0.05)
        self.YOfMaxTemperature = hh/2+hh*d*mtp
        self.calcTemperature()
        
    def getTemperature( self, y ):
        if SimulationEngine.verbose:
            print( "getTemperature", y )
        return( self.temperatureList[int(y)] )
        
    def calcTemperature( self ):
        if SimulationEngine.verbose:
            print( "calcTemperature" )
        self.temperatureList=[]
        hh=self.landwatermap.m.shape[0]
        yOMT=self.YOfMaxTemperature
        tMax=self.maxTemperature
        self.temperatureList=[ tMax-2*(50)*abs( (yOMT-y)/hh ) for y in range(hh) ]
        
    def plantKidPlants( self, kids, seedlightnessfactor ):
        for k in kids:
            failedSetTries=0
            badPlantPos=True
            #Retry finding a space until the place is good.
            while badPlantPos==True and failedSetTries<10:
                # see Plant::getChildList() -> x,y is motherX and motherY until we found a proper X,Y
                motherX=k.position()[1]
                motherY=k.position()[0]
                #Check the map for a random space near the spawn on land.
                tryPos = self.landwatermap.searchSpawn( 10, seedlightnessfactor, motherY, motherX )
                badPlantPos=False
                if (tryPos is None):
                    badPlantPos=True
                else:
                    #Only place it there if no other plant has occupied this place.
                    if self.plantMap[ tryPos[0] ][ tryPos[1] ] is not None:
                        failedSetTries+=1
                        badPlantPos=True
                        
                    if badPlantPos==False:
                        k.setPosition( tryPos[0], tryPos[1] )
                        yp=k.position()[0]
                        xp=k.position()[1]
                        wd=self.landwatermap.getWaterDistance( yp, xp )
                        k.setWaterDistance( wd )
                        self.plantMap[ tryPos[0] ][ tryPos[1] ]=k # Y, X
                        self.plantCounter+=1
                    
                    
    def herbivoreSearchesFoodAndLocations( self, theHerbivore ):
        q=theHerbivore.getQualityOfSense()
        searchTries=q*q/numpy.sqrt(q)
        p=theHerbivore.position()
        foodSitesFound=0
        foodSites=[]
        normalSitesFound=0
        normalSites=[]
        for t in range( int( max(3,searchTries) ) ):
            tmpSite=self.landwatermap.searchSpawn( 3, q+2, p[0], p[1] )
            if tmpSite is not None:
                if self.plantMap[ tmpSite[0] ][ tmpSite[1] ] is not None:
                    foodSitesFound+=1
                    foodSites.append( tmpSite )
                else:
                    normalSitesFound+=1
                    normalSites.append( tmpSite )
                    
            if (foodSitesFound+normalSitesFound)>=10 and foodSitesFound>5 :
                return foodSites, normalSites
        return foodSites, normalSites
                
        
    def tick( self, month ):
        self.tickCounter+=1
        self.setTemperature()

        if SimulationEngine.verbose:
            print( "Engine: Remove dead herbivore." )
            
        tmpHMatrix=[ [ [] for i in range(self.landwatermap.m.shape[1])] for j in range(self.landwatermap.m.shape[0]) ]
        for i in self.herbivores:
            if len(tmpHMatrix[i.position()[1]][i.position()[0]])<3: # TODO - if more than 3 can stay on one field
                tmpHMatrix[i.position()[1]][i.position()[0]].append( i )
            else:
                iMerged=False
                for j in tmpHMatrix[i.position()[1]][i.position()[0]]:
                    if j.getDefense()<i.getDefense():
                        tmpHMatrix[i.position()[1]][i.position()[0]].remove( j ) 
                        tmpHMatrix[i.position()[1]][i.position()[0]].append( i )                       
                        j.deadDueRival( month )
                        self.deadHerbivore.append( j )
                        iMerged=True
                        break
                if not iMerged:                       
                    i.deadDueRival( month )
                    self.deadHerbivore.append( i )
            
        self.herbivores=[]
        for row in tmpHMatrix:
            for element in row:
                self.herbivores=self.herbivores+element
                
        for h in self.herbivores:
            if h.dead( self.getTemperature( h.position()[0] ) )==True:
                h.setDeathMonth( month )
                if self.CSVRequired:
                    self.deadHerbivore.append( h )
                self.herbivores.remove( h )

        if SimulationEngine.verbose:
            print( "Engine: Remove dead plants." )
        for pr in self.plantMap:
            for p in pr:
                if p is not None:
                    if p.dead( self.getTemperature( p.position()[0] ) )==True:
                        p.setDeathMonth( month )
                        if self.CSVRequired:
                            self.deadPlants.append( p )
                        self.plantMap[p.position()[0]][p.position()[1]]=None
                        self.plantCounter-=1
                    
        if self.CSVRequired:
            self.plantAbundanceHistory.append( self.plantCounter )
            self.herbivoreAbundanceHistory.append( len( self.herbivores ) )
        
        if( self.plantCounter==0 and self.plantsExtinct==False):
            
            self.plantsExtinct=True
            if SimulationEngine.verboseImportant:
                print( "All plants died!" )
            
        if( len( self.herbivores )==0 and self.herbivoreExtinct==False and self.herbivoreSpawnStarted==True ):
            self.herbivoreExtinct=True
            if SimulationEngine.verboseImportant:
                print( "All herbivore died!" )
            
        # HERBIVORE
        if SimulationEngine.verbose:
            print( "Engine: Handle herbivore." )
        for h in self.herbivores:
            if SimulationEngine.verbose:
                print( "Engine: Tick herbivore." )
            h.tick( self.getTemperature( h.position()[0] ) )
                
            if SimulationEngine.verbose:
                print( "Engine: Herbivore search food." )
            foundFoodLocations,foundNormalLocations = self.herbivoreSearchesFoodAndLocations( h )
            h.chooseSite( foundFoodLocations, foundNormalLocations, self.temperatureList )
            p=h.position()
            if self.plantMap[p[0]][p[1]] is not None:
                # This plant will get eaten (partitially) by the herbivore h
                # but only if the defense factor are ok
                # and the herbivore is hungry enough
                if h.isHungry() and self.plantMap[p[0]][p[1]].getDefense()<=h.getDefense():
                    self.plantMap[p[0]][p[1]].eaten()
                    h.ate()
            childHerbivoreList = h.getChildList( month )
            for childHerbivore in childHerbivoreList:
                self.herbivores.append( childHerbivore )
            
        # PLANTS
        if SimulationEngine.verbose:
            print( "Engine: Handle plants." )
        for pr in self.plantMap:
            for p in pr:
                if p is not None:
                    if SimulationEngine.verbose:
                        print( "Engine: Tick plants." )
                    p.tick( self.getTemperature( p.position()[0] ) )
                    if SimulationEngine.verbose:
                        print( "Engine: Place seeds of plants." )
                    self.plantKidPlants( p.getChildList( month ), p.getSeedLightness() )
                
        
        
    def makeAbundanceCsv( self ):
        o=open("plantHerbivoreAbundance.csv","w")
        out = csv.writer(o, delimiter=';')
        
        out.writerow(["Month","PlantAbundance","HerbivoreAbundance"])
        for i in range( len( self.plantAbundanceHistory ) ):
            out.writerow([i,self.plantAbundanceHistory[i],self.herbivoreAbundanceHistory[i]])
        o.close()
        
        
    def printPlantsHerbivors( self ):
        oh=open("herbivore.csv","w")
        outHerbivores = csv.writer(oh, delimiter=';')
        outHerbivores.writerow( Herbivore.getCSVHeader() )
        for dh in self.deadHerbivore:
            outHerbivores.writerow( dh.getCSVEntry() )      
        for h in self.herbivores: 
            outHerbivores.writerow( h.getCSVEntry() )
        oh.close()    
            
            
        op=open("plants.csv","w")
        outPlants = csv.writer(op, delimiter=';')
        outPlants.writerow( Plant.getCSVHeader() )
        for dp in self.deadPlants:
            outPlants.writerow( dp.getCSVEntry() )  
        for pr in self.plantMap:
            for p in pr:
                if p is not None:  
                    outPlants.writerow( p.getCSVEntry() )  
        op.close()   
        
        
        
        
        
        