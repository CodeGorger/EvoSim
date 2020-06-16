# -*- coding: utf-8 -*-
"""
Created on Tue Dec 16 17:19:40 2014

@author: simon

This is a script that simulations ecology. Part of the system are carnivore,
herbivore and plants. Also a spatial system is part of the system, with water
and land sites.

"""

import numpy
import sys

sys.path.append("/home/simon/Desktop/StudiumTuebingen/CMEE/FinalProject/\
                 pythonSimulation/")
from landwatermap import LandWaterMap
from simulationengine import SimulationEngine
from herbivore import Herbivore
from plant import Plant


if __name__ == "__main__":
    if len(sys.argv) < 100:
        print( 'Missing parameter! (' , len(sys.argv) , '/100)' )
        sys.exit( 3 )
    printArgv=True
    if printArgv:
        print( "General PART I: " )
        print( "        no. of iteration: " + sys.argv[1] )
        print( "              map height: " + sys.argv[2] )
        print( "               map width: " + sys.argv[3] )
        print( "        land-probability: " + sys.argv[4] )
        print( "   land-clustering-alpha: " + sys.argv[5] )
        print( "   initial amount plants: " + sys.argv[6] )
        print( "initial amount herbivore: " + sys.argv[7] )
        print( "   spawn month herbivore: " + sys.argv[8] )
        print( "PLANTS: " )
        print( "         plantFirstGenerationMaxWaterDistance: " + sys.argv[9])
        print( "           plantFirstGenerationSeedProdFactor: " + sys.argv[10])
        print( "            plantFirstGenerationSeedLightness: " + sys.argv[11])
        print( "                   plantFirstGenerationMaxAge: " + sys.argv[12])
        print( "               plantFirstGenerationFoodChunks: " + sys.argv[13])
        print( "            plantFirstGenerationDefenseFactor: " + sys.argv[14])
        print( "      plantFirstGenerationFavoriteTemperature: " + sys.argv[15])
        print( "          plantFirstGenerationTemperatureSpan: " + sys.argv[16])
        
        print( "            plantMutationRateMaxWaterDistance: " + sys.argv[17])
        print( "              plantMutationRateSeedProdFactor: " + sys.argv[18])
        print( "               plantMutationRateSeedLightness: " + sys.argv[19])
        print( "                      plantMutationRateMaxAge: " + sys.argv[20])
        print( "                  plantMutationRateFoodChunks: " + sys.argv[21])
        print( "               plantMutationRateDefenseFactor: " + sys.argv[22])
        print( "         plantMutationRateFavoriteTemperature: " + sys.argv[23])
        print( "             plantMutationRateTemperatureSpan: " + sys.argv[24])
    
        print( "             plantMaxMutationMaxWaterDistance: " + sys.argv[25])
        print( "               plantMaxMutationSeedProdFactor: " + sys.argv[26])
        print( "                plantMaxMutationSeedLightness: " + sys.argv[27])
        print( "                       plantMaxMutationMaxAge: " + sys.argv[28])
        print( "                   plantMaxMutationFoodChunks: " + sys.argv[29])
        print( "                plantMaxMutationDefenseFactor: " + sys.argv[30])
        print( "          plantMaxMutationFavoriteTemperature: " + sys.argv[31])
        print( "              plantMaxMutationTemperatureSpan: " + sys.argv[32])
    
        print( "             plantMinMutationMaxWaterDistance: " + sys.argv[33])
        print( "               plantMinMutationSeedProdFactor: " + sys.argv[34])
        print( "                plantMinMutationSeedLightness: " + sys.argv[35])
        print( "                       plantMinMutationMaxAge: " + sys.argv[36])
        print( "                   plantMinMutationFoodChunks: " + sys.argv[37])
        print( "                plantMinMutationDefenseFactor: " + sys.argv[38])
        print( "          plantMinMutationFavoriteTemperature: " + sys.argv[39])
        print( "              plantMinMutationTemperatureSpan: " + sys.argv[40])
    
        print( "            plantStepMutationMaxWaterDistance: " + sys.argv[41])
        print( "              plantStepMutationSeedProdFactor: " + sys.argv[42])
        print( "               plantStepMutationSeedLightness: " + sys.argv[43])
        print( "                      plantStepMutationMaxAge: " + sys.argv[44])
        print( "                  plantStepMutationFoodChunks: " + sys.argv[45])
        print( "               plantStepMutationDefenseFactor: " + sys.argv[46])
        print( "         plantStepMutationFavoriteTemperature: " + sys.argv[47])
        print( "             plantStepMutationTemperatureSpan: " + sys.argv[48])
    
        print( "HERBIVORE: " )
        print( "               herbivoreFirstGenerationMaxAge: " + sys.argv[49])
        print( "            herbivoreFirstGenerationMaxHunger: " + sys.argv[50])
        print( "        herbivoreFirstGenerationDefenseFactor: " + sys.argv[51])
        print( "       herbivoreFirstGenerationQualityOfSense: " + sys.argv[52])
        print( "  herbivoreFirstGenerationFavoriteTemperature: " + sys.argv[53])
        print( "      herbivoreFirstGenerationTemperatureSpan: " + sys.argv[54])
        print( "herbivoreFirstGenerationChildProductionFactor: " + sys.argv[55])
    
        print( "                  herbivoreMutationRateMaxAge: " + sys.argv[56])
        print( "               herbivoreMutationRateMaxHunger: " + sys.argv[57])
        print( "           herbivoreMutationRateDefenseFactor: " + sys.argv[58])
        print( "          herbivoreMutationRateQualityOfSense: " + sys.argv[59])
        print( "     herbivoreMutationRateFavoriteTemperature: " + sys.argv[60])
        print( "         herbivoreMutationRateTemperatureSpan: " + sys.argv[61])
        print( "   herbivoreMutationRateChildProductionFactor: " + sys.argv[62])
    
        print( "                   herbivoreMaxMutationMaxAge: " + sys.argv[63])
        print( "                herbivoreMaxMutationMaxHunger: " + sys.argv[64])
        print( "            herbivoreMaxMutationDefenseFactor: " + sys.argv[65])
        print( "           herbivoreMaxMutationQualityOfSense: " + sys.argv[66])
        print( "      herbivoreMaxMutationFavoriteTemperature: " + sys.argv[67])
        print( "          herbivoreMaxMutationTemperatureSpan: " + sys.argv[68])
        print( "    herbivoreMaxMutationChildProductionFactor: " + sys.argv[69])
    
        print( "                   herbivoreMinMutationMaxAge: " + sys.argv[70])
        print( "                herbivoreMinMutationMaxHunger: " + sys.argv[71])
        print( "            herbivoreMinMutationDefenseFactor: " + sys.argv[72])
        print( "           herbivoreMinMutationQualityOfSense: " + sys.argv[73])
        print( "      herbivoreMinMutationFavoriteTemperature: " + sys.argv[74])
        print( "          herbivoreMinMutationTemperatureSpan: " + sys.argv[75])
        print( "    herbivoreMinMutationChildProductionFactor: " + sys.argv[76])
    
        print( "                  herbivoreStepMutationMaxAge: " + sys.argv[77])
        print( "               herbivoreStepMutationMaxHunger: " + sys.argv[78])
        print( "           herbivoreStepMutationDefenseFactor: " + sys.argv[79])
        print( "          herbivoreStepMutationQualityOfSense: " + sys.argv[80])
        print( "     herbivoreStepMutationFavoriteTemperature: " + sys.argv[81])
        print( "         herbivoreStepMutationTemperatureSpan: " + sys.argv[82])
        print( "   herbivoreStepMutationChildProductionFactor: " + sys.argv[83])
        
        print( "General PART II: " )
        
        print( "     herbivoreHungerPercentReproduction: " + sys.argv[84])
        print( "herbivoreTemperaturePercentReproduction: " + sys.argv[85])
        print( "      plantWaterDistPercentReproduction: " + sys.argv[86])
        print( "    plantTemperaturePercentReproduction: " + sys.argv[87])
        
        print( "               abortIfHerbivoreExtinct: " + sys.argv[88])
        print( "                  abortIfPlantsExtinct: " + sys.argv[89])
    
        print( "           reproductionPercentHerbivore: " + sys.argv[90])
        print( "              reproductionPercentPlants: " + sys.argv[91])
        
        
        print( "         MinAgeForReproductionHerbivore: " + sys.argv[92])
        print( "         MaxAgeForReproductionHerbivore: " + sys.argv[93])
        print( "                     hungerPercentToEat: " + sys.argv[94])
        print( "                  randomSeed(0=default): " + sys.argv[95])
        print( "                          withCSVOutput: " + sys.argv[96])
        print( "                          Visual output: " + sys.argv[97])
        print( "                 Visual output as video: " + sys.argv[98])
        print( "                                 Silent: " + sys.argv[99])
    
    #Create the world
    world = LandWaterMap( int( sys.argv[2] ), 
                          int( sys.argv[3] ), 
                          float( sys.argv[4] ), 
                          float( sys.argv[5] ) )    
    sim = SimulationEngine( world )
    
    # Plant
    # First settings
    pFGMWD = float( sys.argv[9] ) # plantFirstGenerationMaxWaterDistance   
    pFGSPF = float( sys.argv[10])  # plantFirstGenerationSeedProdFactor
    pFGSL  = float( sys.argv[11]) # plantFirstGenerationSeedLightness
    pFGMA  = float( sys.argv[12]) # plantFirstGenerationMaxAge
    pFGFC  = float( sys.argv[13]) # plantFirstGenerationFoodChunks
    pFGDF  = float( sys.argv[14]) # plantFirstGenerationDefenseFactor
    pFGFT  = float( sys.argv[15]) # plantFirstGenerationFavoriteTemperature   
    pFGTS  = float( sys.argv[16]) # plantFirstGenerationTemperatureSpan
    
    
    
    # Plant
    # Mutationrate    
    pMRMWD = float( sys.argv[17]) # plantMutationRateMaxWaterDistance   
    pMRSPF = float( sys.argv[18]) # plantMutationRateSeedProdFactor
    pMRSL  = float( sys.argv[19]) # plantMutationRateSeedLightness
    pMRMA  = float( sys.argv[20]) # plantMutationRateMaxAge
    pMRFC  = float( sys.argv[21]) # plantMutationRateFoodChunks
    pMRDF  = float( sys.argv[22]) # plantMutationRateDefenseFactor
    pMRFT  = float( sys.argv[23]) # plantMutationRateFavoriteTemperature   
    pMRTS  = float( sys.argv[24]) # plantMutationRateTemperatureSpan
    
    # Plant
    # Max Mutation values
    pMaxMMWD = float( sys.argv[25]) # plantMaxMutationMaxWaterDistance   
    pMaxMSPF = float( sys.argv[26]) # plantMaxMutationSeedProdFactor
    pMaxMSL  = float( sys.argv[27]) # plantMaxMutationSeedLightness
    pMaxMMA  = float( sys.argv[28]) # plantMaxMutationMaxAge
    pMaxMFC  = float( sys.argv[29]) # plantMaxMutationFoodChunks
    pMaxMDF  = float( sys.argv[30]) # plantMaxMutationDefenseFactor
    pMaxMFT  = float( sys.argv[31]) # plantMaxMutationFavoriteTemperature   
    pMaxMTS  = float( sys.argv[32]) # plantMaxMutationTemperatureSpan
    
    # Plant
    # Min Mutation values
    pMinMMWD = float( sys.argv[33]) # plantMinMutationMaxWaterDistance   
    pMinMSPF = float( sys.argv[34]) # plantMinMutationSeedProdFactor
    pMinMSL  = float( sys.argv[35]) # plantMinMutationSeedLightness
    pMinMMA  = float( sys.argv[36]) # plantMinMutationMaxAge
    pMinMFC  = float( sys.argv[37]) # plantMinMutationFoodChunks
    pMinMDF  = float( sys.argv[38]) # plantMinMutationDefenseFactor
    pMinMFT  = float( sys.argv[39]) # plantMinMutationFavoriteTemperature   
    pMinMTS  = float( sys.argv[40]) # plantMinMutationTemperatureSpan
    
    # Plant
    # Step of Mutation
    pStepMMWD = float( sys.argv[41]) # plantStepMutationMaxWaterDistance   
    pStepMSPF = float( sys.argv[42]) # plantStepMutationSeedProdFactor
    pStepMSL  = float( sys.argv[43]) # plantStepMutationSeedLightness
    pStepMMA  = float( sys.argv[44]) # plantStepMutationMaxAge
    pStepMFC  = float( sys.argv[45]) # plantStepMutationFoodChunks
    pStepMDF  = float( sys.argv[46]) # plantStepMutationDefenseFactor
    pStepMFT  = float( sys.argv[47]) # plantStepMutationFavoriteTemperature   
    pStepMTS  = float( sys.argv[48]) # plantStepMutationTemperatureSpan
    
    
    # Herbivore
    # First settings    
    hFGMA  = float( sys.argv[49]) # herbivoreFirstGenerationMaxAge
    hFGMH  = float( sys.argv[50]) # herbivoreFirstGenerationMaxHunger
    hFGDF  = float( sys.argv[51]) # herbivoreFirstGenerationDefenseFactor
    hFGQOS = float( sys.argv[52]) # herbivoreFirstGenerationQualityOfSense
    hFGFT  = float( sys.argv[53]) # herbivoreFirstGenerationFavoriteTemperature
    hFGTS  = float( sys.argv[54]) # herbivoreFirstGenerationTemperatureSpan
    hFGCPF = float( sys.argv[55]) # herbivoreFirstGenerationChildProductionFactor
    
    # Herbivore
    # Mutationrate
    hMRMA  = float( sys.argv[56]) # herbivoreMutationRateMaxAge
    hMRMH  = float( sys.argv[57]) # herbivoreMutationRateMaxHunger
    hMRDF  = float( sys.argv[58]) # herbivoreMutationRateDefenseFactor
    hMRQOS = float( sys.argv[59]) # herbivoreMutationRateQualityOfSense
    hMRFT  = float( sys.argv[60]) # herbivoreMutationRateFavoriteTemperature
    hMRTS  = float( sys.argv[61]) # herbivoreMutationRateTemperatureSpan
    hMRCPF = float( sys.argv[62]) # herbivoreMutationRateChildProductionFactor
    
    # Herbivore
    # Max Mutation values
    hMaxMMA  = float( sys.argv[63]) # herbivoreMaxMutationMaxAge
    hMaxMMH  = float( sys.argv[64]) # herbivoreMaxMutationMaxHunger
    hMaxMDF  = float( sys.argv[65]) # herbivoreMaxMutationDefenseFactor
    hMaxMQOS = float( sys.argv[66]) # herbivoreMaxMutationQualityOfSense
    hMaxMFT  = float( sys.argv[67]) # herbivoreMaxMutationFavoriteTemperature
    hMaxMTS  = float( sys.argv[68]) # herbivoreMaxMutationTemperatureSpan
    hMaxMCPF = float( sys.argv[69]) # herbivoreMaxMutationChildProductionFactor
    
    # Herbivore
    # Min Mutation values
    hMinMMA  = float( sys.argv[70]) # herbivoreMinMutationMaxAge
    hMinMMH  = float( sys.argv[71]) # herbivoreMinMutationMaxHunger
    hMinMDF  = float( sys.argv[72]) # herbivoreMinMutationDefenseFactor
    hMinMQOS = float( sys.argv[73]) # herbivoreMinMutationQualityOfSense
    hMinMFT  = float( sys.argv[74]) # herbivoreMinMutationFavoriteTemperature
    hMinMTS  = float( sys.argv[75]) # herbivoreMinMutationTemperatureSpan
    hMinMCPF = float( sys.argv[76]) # herbivoreMinMutationChildProductionFactor
    
    # Herbivore
    # Step of Mutation
    hStepMMA  = float( sys.argv[77] )  # herbivoreStepMutationMaxAge
    hStepMMH  = float( sys.argv[78] )  # herbivoreStepMutationMaxHunger
    hStepMDF  = float( sys.argv[79] )  # herbivoreStepMutationDefenseFactor
    hStepMQOS = float( sys.argv[80] )  # herbivoreStepMutationQualityOfSense
    hStepMFT  = float( sys.argv[81] )  # herbivoreStepMutationFavoriteTemperature
    hStepMTS  = float( sys.argv[82] )  # herbivoreStepMutationTemperatureSpan
    hStepMCPF = float( sys.argv[83] )  # herbivoreStepMutationChildProductionFactor
    
    
    Plant.setRateOfMutation(pMRMWD,pMRSPF,pMRSL,pMRMA,pMRFC,pMRDF,pMRFT,pMRTS)
    Plant.setMaxMutationVals(pMaxMMWD,pMaxMSPF,pMaxMSL,pMaxMMA,pMaxMFC,pMaxMDF,pMaxMFT,pMaxMTS)
    Plant.setMinMutationVals(pMinMMWD,pMinMSPF,pMinMSL,pMinMMA,pMinMFC,pMinMDF,pMinMFT,pMinMTS)
    Plant.setStepMutationVals(pStepMMWD,pStepMSPF,pStepMSL,pStepMMA,pStepMFC,pStepMDF,pStepMFT,pStepMTS)
    
    
    Herbivore.setRateOfMutation(hMRMA,hMRMH,hMRDF,hMRQOS,hMRFT,hMRTS,hMRCPF)
    Herbivore.setMaxMutationVals(hMaxMMA,hMaxMMH,hMaxMDF,hMaxMQOS,hMaxMFT,hMaxMTS,hMaxMCPF)
    Herbivore.setMinMutationVals(hMinMMA,hMinMMH,hMinMDF,hMinMQOS,hMinMFT,hMinMTS,hMinMCPF)
    Herbivore.setStepMutationVals(hStepMMA,hStepMMH,hStepMDF,hStepMQOS,hStepMFT,hStepMTS,hStepMCPF)
    
    Herbivore.setReproductionInfluencePercents( float( sys.argv[84] ), float( sys.argv[85] ) )
    Plant.setReproductionInfluencePercents( float( sys.argv[86] ), float( sys.argv[87] ) )
    
    abortIfHerbivoreDistint=bool( int( sys.argv[88] ) )
    abortIfPlantsDistint=bool( int( sys.argv[89] ) )
    
    Herbivore.setNeededPercentForReproduction( float( sys.argv[90] ) )
    Plant.setNeededPercentForReproduction( float( sys.argv[91] ) )
    Herbivore.setReproductionAgeRestrictions( float( sys.argv[92] ), float( sys.argv[93] ) )
    Herbivore.setEatAtHungerPercent( float( sys.argv[94] ) )
    
    if( float(sys.argv[95]) != 0 ):
        random.seed( float(sys.argv[95]) )
    
    CSVRequired=bool( int( sys.argv[96] ) )
    sim.setCSVRequired( CSVRequired )
    
    visualOutput=bool( int( sys.argv[97] ) )
    vid=bool( int( sys.argv[98] ) )
    silent=int( sys.argv[99] )
    SimulationEngine.setSilent( silent )
    ret = 0
    #Start the simulation!
    
    if silent>0:
        print( "SPAWN PLANTS (month:0)" )
    sim.spawnPlants( int( sys.argv[6] ),
                     pFGMWD, pFGSPF, pFGSL, pFGMA, pFGFC, pFGDF, pFGFT, pFGTS, 0 )
    
    for i in range(int(sys.argv[1])):
        if i%5==0:
            if silent>0:
                print( "Month:", i)
            if visualOutput:
                if vid:
                    sim.showVid()
                else:
                    sim.showDia()
        if i%250==0:
            if silent==0:
                print( "Month:", i )
                
                    
                
        if( i==int( sys.argv[8] ) ):
            if silent>0:
                print( "SPAWN HERBIVORE (month:", i, ")" )
            sim.spawnHerbivore( int( sys.argv[7] ),
                               hFGMA, hFGMH, hFGDF, hFGQOS, hFGFT, hFGTS, hFGCPF, i )
        sim.tick( i )
        if abortIfHerbivoreDistint and sim.getAllHerbivoreExtinct():
            ret=1
            break
        if abortIfPlantsDistint and sim.getAllPlantsExtinct():
            ret=2        
            break
        
    #    fileName= "plants"+str(i)+".txt"
    #    output=[]
    #    for pl in sim.plants:
    #        output.append(pl.position())
    #    numpy.savetxt( fileName, output )
        #fileName= "m"+i+".txt"
        #numpy.savetxt( fileName, sim.showMap() )
    
    
    if CSVRequired :
        sim.makeAbundanceCsv()
        sim.printPlantsHerbivors()
    
    
    #sim.showMap()
    sys.exit( ret )
    
    
    
