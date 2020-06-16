import numpy as np
import matplotlib.pyplot as plt
from pylab import *

#def graphAbundance():
#    month,p,h=np.loadtxt('/home/simon/Desktop/StudiumTuebingen/CMEE/FinalProject/pythonSimulation/plantHerbivoreAbundance.csv', 
#                         delimiter=';', 
#                         unpack=True,
#                         skiprows=1)
#    fig = plt.figure()
#    ax1 = fig.add_subplot( 1, 1, 1, axisbg='white' )
#   
#    ax1.plot( month, h, lw=1.0, color='r', label='herbivore' )
#    ax1.plot( month, p, lw=1.0, color='g', label='plants' )
#    ax1.legend(['Herbivores', 'Plants'])
#    
#    plt.title('Plant-Herbivore-Abundance')
#    plt.ylabel('Abundance')
#    plt.xlabel('Month')
#    plt.show()
#    
#graphAbundance()

#
#def graphPieOfKids():
##    p_PhyloName,p_Age,p_MaxAge,p_IsWaterDistance,p_MaxWaterDistance,
##    p_DefenseFactor,p_FoodChunks,p_FavoriteTemperature,p_TemperatureSpan,p_SeedLightness,
##    p_SeedProdFactor,p_KidCount,p_BirthMonth,p_DeathMonth,p_DeathReason
#    p_KidCount=np.loadtxt('/home/simon/Desktop/StudiumTuebingen/CMEE/FinalProject/pythonSimulation/plants.csv', 
#                         delimiter=';', 
#                         unpack=True,
#                         skiprows=1,
#                         usecols=[11])
#    
##    h_PhyloName,h_Age,h_MaxAge,h_Hunger,h_MaxHunger,
##    h_DefenseFactor,h_QualityOfSense,h_FavoriteTemperature,h_TemperatureSpan,h_ChildProductionFactor,
##    h_KidCount,h_BirthMonth,h_DeathMonth,h_DeathReason
#    h_KidCount=np.loadtxt('/home/simon/Desktop/StudiumTuebingen/CMEE/FinalProject/pythonSimulation/herbivore.csv', 
#                         delimiter=';', 
#                         unpack=True,
#                         skiprows=1,
#                         usecols=[10])
#    print( h_KidCount )
#    plantKids=[0,0,0,0,0,0,0,0]             
#    for p in p_KidCount:
#        if p > 6:
#            plantKids[7]+=1
#        else:
#            plantKids[int(p)]+=1
#        
#    labelsPlants=["Zero kids","One kid","Two kids","Three kids","Four kids","Five kids","Six kids","7 or more kids"]
#    pie(plantKids, labels=labelsPlants, autopct='%1.1f%%', shadow=True)
#    title('Kids of Plants')    
#    show()
#    
#    herbivoreKids=[0,0,0,0,0] 
#    for h in h_KidCount:
#        if h == 0:
#            herbivoreKids[0]+=1
#        if h == 1 or h == 2:
#            herbivoreKids[1]+=1
#        if h == 3:
#            herbivoreKids[2]+=1
#        if h == 4:
#            herbivoreKids[3]+=1
#        if h > 4:
#            herbivoreKids[4]+=1
#    labelsHerbivores=["Zero kids","One or two kids","Three kids","Four kids","More than 4 kids"]
#    pie(herbivoreKids, labels=labelsHerbivores, autopct='%1.1f%%', shadow=True, startangle=180)
#    title('Kids of Herbivore')    
#    show()
#
#    
#graphPieOfKids()


def graphDefenseMutation():
##    h_PhyloName,h_Age,h_MaxAge,h_Hunger,h_MaxHunger,
##    h_DefenseFactor,h_QualityOfSense,h_FavoriteTemperature,h_TemperatureSpan,h_ChildProductionFactor,
##    h_KidCount,h_BirthMonth,h_DeathMonth,h_DeathReason
    h_BirthMonth_h_DefenseFactor=np.loadtxt('/home/simon/Desktop/StudiumTuebingen/CMEE/FinalProject/pythonSimulation/herbivore.csv', 
                         delimiter=';', 
                         unpack=True,
                         skiprows=1,
                         usecols=[11,6])
    fig = plt.figure()
    ax1 = fig.add_subplot( 1, 1, 1, axisbg='white' )
   
    ax1.plot( month, h, lw=1.0, color='r', label='herbivore' )
    ax1.plot( month, p, lw=1.0, color='g', label='plants' )
    ax1.legend(['Herbivores', 'Plants'])
    
    plt.title('Defense - Mutation')
    plt.ylabel('Defense - Average')
    plt.xlabel('Month')
    plt.show()
    
graphDefenseMutation()
