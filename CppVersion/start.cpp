
#include <iostream>
#include <string>
#include "LandWaterMap.h"
#include "Plant.h"
#include "Herbivore.h"
#include "EvoSimRandom.h"
#include <chrono>
#include "SimulationEngine.h"


void PrintArgumentList(char* mainArgvs[]);
void SetStaticPlantMutationProperties(char* mainArgvs[]);
void SetStaticHerbivoreMutationProperties(char* mainArgvs[]);
void SetStaticHerbivoreMiscProperties(char* mainArgvs[]);
void SetStaticPlantMiscProperties(char* mainArgvs[]);
Herbivore CreatePrimalHerbivore(char* mainArgvs[]);
Plant CreatePrimalPlant(char* mainArgvs[]);
void InitializeRandomGenerator(char* mainArgvs[]);

int main(int argc, char* argv[])
{
    if (argc < 100)
    {
        std::cout << "Wrong Parameter count. (" << argc << "/100)" << std::endl;
        return -1;
    }
    bool printArgv = false;
    if (printArgv)
    {
        PrintArgumentList(argv);
    }

     LandWaterMap world = LandWaterMap(
        std::stoi(argv[2]),
        std::stoi(argv[3]),
        std::stod(argv[4]),
        std::stod(argv[5]));

    SetStaticPlantMutationProperties(argv);
    SetStaticHerbivoreMutationProperties(argv);
    SetStaticPlantMiscProperties(argv);
    SetStaticHerbivoreMiscProperties(argv);

    InitializeRandomGenerator(argv);


    SimulationEngine sim;
    sim.SetWorld(world);
    sim.SetAbortIfHerbivoreDistint((0 != std::stoi(argv[88])));
    sim.SetAbortIfPlantsDistint((0 != std::stoi(argv[89])));
    sim.SetCsvRequired((0 != std::stoi(argv[96])));
    sim.SetVisualOutputRequired((0 != std::stoi(argv[97])));
    sim.SetMaxIterations(std::stoi(argv[1]));


    Plant primalPlant = CreatePrimalPlant(argv);
    int primalPlantCount = std::stoi(argv[6]);
    sim.SetPrimalPlant(primalPlant, primalPlantCount);

    Herbivore primalHerbivore = CreatePrimalHerbivore(argv);
    int primalHerbivoreCount = std::stoi(argv[7]);
    int primalHerbivoreSpawnMonth = std::stoi(argv[8]);
    sim.SetPrimalHerbivore(
        primalHerbivore, 
        primalHerbivoreCount, 
        primalHerbivoreSpawnMonth);

    //vid = (0 != std::stoi(argv[98]));
    //silent = (0 != std::stoi(argv[99]));

    int returnValue = sim.Run();
}

void InitializeRandomGenerator(char* mainArgvs[])
{
    unsigned int initialSeed = std::stoi(mainArgvs[95]);
    if (initialSeed != 0)
    {
        EvoSimRandom::SetSeed(initialSeed);
    }
    else
    {
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        unsigned int timelySeed = (unsigned int)now.time_since_epoch().count();
        EvoSimRandom::SetSeed(timelySeed);
    }
}

Plant CreatePrimalPlant(char* mainArgvs[])
{
    double pFGMWD = std::stod(mainArgvs[9]);  // plantFirstGenerationMaxWaterDistance
    double pFGSPF = std::stod(mainArgvs[10]); // plantFirstGenerationSeedProdFactor
    double pFGSL = std::stod(mainArgvs[11]);  // plantFirstGenerationSeedLightness
    double pFGMA = std::stod(mainArgvs[12]);  // plantFirstGenerationMaxAge
    double pFGFC = std::stod(mainArgvs[13]);  // plantFirstGenerationFoodChunks
    double pFGDF = std::stod(mainArgvs[14]);  // plantFirstGenerationDefenseFactor
    double pFGFT = std::stod(mainArgvs[15]);  // plantFirstGenerationFavoriteTemperature
    double pFGTS = std::stod(mainArgvs[16]);  // plantFirstGenerationTemperatureSpan
    return Plant( 
        pFGMWD,
        pFGSPF,
        pFGSL,
        pFGMA,
        pFGFC,
        pFGDF,
        pFGFT,
        pFGTS,
        "P",
        0);
}

Herbivore CreatePrimalHerbivore(char* mainArgvs[])
{
    double hFGMA  = std::stod(mainArgvs[49]);  // herbivoreFirstGenerationMaxAge
    double hFGMH  = std::stod(mainArgvs[50]);  // herbivoreFirstGenerationMaxHunger
    double hFGDF  = std::stod(mainArgvs[51]);  // herbivoreFirstGenerationDefenseFactor
    double hFGQOS = std::stod(mainArgvs[52]);  // herbivoreFirstGenerationQualityOfSense
    double hFGFT  = std::stod(mainArgvs[53]);  // herbivoreFirstGenerationFavoriteTemperature
    double hFGTS  = std::stod(mainArgvs[54]);  // herbivoreFirstGenerationTemperatureSpan
    double hFGCPF = std::stod(mainArgvs[55]);  // herbivoreFirstGenerationChildProductionFactor
    return Herbivore(
        hFGMA,
        hFGMH,
        hFGDF,
        hFGQOS,
        hFGFT,
        hFGTS,
        hFGCPF,
        "H");
}

void SetStaticPlantMiscProperties(char* mainArgvs[])
{
    Plant::SetReproductionInfluencePercents(std::stod(mainArgvs[86]), std::stod(mainArgvs[87]));
    Plant::SetNeededPercentForReproduction(std::stod(mainArgvs[91]));
}

void SetStaticHerbivoreMiscProperties(char* mainArgvs[])
{
    Herbivore::SetReproductionInfluencePercents(std::stod(mainArgvs[84]), std::stod(mainArgvs[85]));
    Herbivore::SetNeededPercentForReproduction(std::stod(mainArgvs[90]));
    Herbivore::SetReproductionAgeRestrictions(std::stod(mainArgvs[92]), std::stod(mainArgvs[93]));
    Herbivore::SetEatAtHungerPercent(std::stod(mainArgvs[94]));
}

void SetStaticHerbivoreMutationProperties(char* mainArgvs[])
{
    Herbivore::GetMaxAge_MPS().SetMutationRate(std::stod(mainArgvs[56]));
    Herbivore::GetMaxHunger_MPS().SetMutationRate(std::stod(mainArgvs[57]));
    Herbivore::GetDefenseFactor_MPS().SetMutationRate(std::stod(mainArgvs[58]));
    Herbivore::GetQualityOfSense_MPS().SetMutationRate(std::stod(mainArgvs[59]));
    Herbivore::GetFavoriteTemperature_MPS().SetMutationRate(std::stod(mainArgvs[60]));
    Herbivore::GetTemperatureSpan_MPS().SetMutationRate(std::stod(mainArgvs[61]));
    Herbivore::GetChildProductionFactor_MPS().SetMutationRate(std::stod(mainArgvs[62]));

    Herbivore::GetMaxAge_MPS().SetMutationMax(std::stod(mainArgvs[63]));
    Herbivore::GetMaxHunger_MPS().SetMutationMax(std::stod(mainArgvs[64]));
    Herbivore::GetDefenseFactor_MPS().SetMutationMax(std::stod(mainArgvs[65]));
    Herbivore::GetQualityOfSense_MPS().SetMutationMax(std::stod(mainArgvs[66]));
    Herbivore::GetFavoriteTemperature_MPS().SetMutationMax(std::stod(mainArgvs[67]));
    Herbivore::GetTemperatureSpan_MPS().SetMutationMax(std::stod(mainArgvs[68]));
    Herbivore::GetChildProductionFactor_MPS().SetMutationMax(std::stod(mainArgvs[69]));

    Herbivore::GetMaxAge_MPS().SetMutationMin(std::stod(mainArgvs[70]));
    Herbivore::GetMaxHunger_MPS().SetMutationMin(std::stod(mainArgvs[71]));
    Herbivore::GetDefenseFactor_MPS().SetMutationMin(std::stod(mainArgvs[72]));
    Herbivore::GetQualityOfSense_MPS().SetMutationMin(std::stod(mainArgvs[73]));
    Herbivore::GetFavoriteTemperature_MPS().SetMutationMin(std::stod(mainArgvs[74]));
    Herbivore::GetTemperatureSpan_MPS().SetMutationMin(std::stod(mainArgvs[75]));
    Herbivore::GetChildProductionFactor_MPS().SetMutationMin(std::stod(mainArgvs[76]));

    Herbivore::GetMaxAge_MPS().SetMutationStep(std::stod(mainArgvs[77]));
    Herbivore::GetMaxHunger_MPS().SetMutationStep(std::stod(mainArgvs[78]));
    Herbivore::GetDefenseFactor_MPS().SetMutationStep(std::stod(mainArgvs[79]));
    Herbivore::GetQualityOfSense_MPS().SetMutationStep(std::stod(mainArgvs[80]));
    Herbivore::GetFavoriteTemperature_MPS().SetMutationStep(std::stod(mainArgvs[81]));
    Herbivore::GetTemperatureSpan_MPS().SetMutationStep(std::stod(mainArgvs[82]));
    Herbivore::GetChildProductionFactor_MPS().SetMutationStep(std::stod(mainArgvs[83]));

}

void SetStaticPlantMutationProperties(char* mainArgvs[])
{
    Plant::GetMaxWaterDistance_MPS().SetMutationRate(std::stod(mainArgvs[17]));
    Plant::GetSeedProdFactor_MPS().SetMutationRate(std::stod(mainArgvs[18]));
    Plant::GetSeedLightness_MPS().SetMutationRate(std::stod(mainArgvs[19]));
    Plant::GetMaxAge_MPS().SetMutationRate(std::stod(mainArgvs[20]));
    Plant::GetFoodChunks_MPS().SetMutationRate(std::stod(mainArgvs[21]));
    Plant::GetDefenseFactor_MPS().SetMutationRate(std::stod(mainArgvs[22]));
    Plant::GetFavoriteTemperature_MPS().SetMutationRate(std::stod(mainArgvs[23]));
    Plant::GetTemperatureSpan_MPS().SetMutationRate(std::stod(mainArgvs[24]));

    Plant::GetMaxWaterDistance_MPS().SetMutationMax(std::stod(mainArgvs[25]));
    Plant::GetSeedProdFactor_MPS().SetMutationMax(std::stod(mainArgvs[26]));
    Plant::GetSeedLightness_MPS().SetMutationMax(std::stod(mainArgvs[27]));
    Plant::GetMaxAge_MPS().SetMutationMax(std::stod(mainArgvs[28]));
    Plant::GetFoodChunks_MPS().SetMutationMax(std::stod(mainArgvs[29]));
    Plant::GetDefenseFactor_MPS().SetMutationMax(std::stod(mainArgvs[30]));
    Plant::GetFavoriteTemperature_MPS().SetMutationMax(std::stod(mainArgvs[31]));
    Plant::GetTemperatureSpan_MPS().SetMutationMax(std::stod(mainArgvs[32]));

    Plant::GetMaxWaterDistance_MPS().SetMutationMin(std::stod(mainArgvs[33]));
    Plant::GetSeedProdFactor_MPS().SetMutationMin(std::stod(mainArgvs[34]));
    Plant::GetSeedLightness_MPS().SetMutationMin(std::stod(mainArgvs[35]));
    Plant::GetMaxAge_MPS().SetMutationMin(std::stod(mainArgvs[36]));
    Plant::GetFoodChunks_MPS().SetMutationMin(std::stod(mainArgvs[37]));
    Plant::GetDefenseFactor_MPS().SetMutationMin(std::stod(mainArgvs[38]));
    Plant::GetFavoriteTemperature_MPS().SetMutationMin(std::stod(mainArgvs[39]));
    Plant::GetTemperatureSpan_MPS().SetMutationMin(std::stod(mainArgvs[40]));

    Plant::GetMaxWaterDistance_MPS().SetMutationStep(std::stod(mainArgvs[41]));
    Plant::GetSeedProdFactor_MPS().SetMutationStep(std::stod(mainArgvs[42]));
    Plant::GetSeedLightness_MPS().SetMutationStep(std::stod(mainArgvs[43]));
    Plant::GetMaxAge_MPS().SetMutationStep(std::stod(mainArgvs[44]));
    Plant::GetFoodChunks_MPS().SetMutationStep(std::stod(mainArgvs[45]));
    Plant::GetDefenseFactor_MPS().SetMutationStep(std::stod(mainArgvs[46]));
    Plant::GetFavoriteTemperature_MPS().SetMutationStep(std::stod(mainArgvs[47]));
    Plant::GetTemperatureSpan_MPS().SetMutationStep(std::stod(mainArgvs[48]));
}

void PrintArgumentList(char* mainArgvs[])
{
    std::cout << "General PART I: " << std::endl;
    std::cout << "        no. of iteration: " << mainArgvs[1] << std::endl;
    std::cout << "              map height: " << mainArgvs[2] << std::endl;
    std::cout << "               map width: " << mainArgvs[3] << std::endl;
    std::cout << "        land-probability: " << mainArgvs[4] << std::endl;
    std::cout << "   land-clustering-alpha: " << mainArgvs[5] << std::endl;
    std::cout << "   initial amount plants: " << mainArgvs[6] << std::endl;
    std::cout << "initial amount herbivore: " << mainArgvs[7] << std::endl;
    std::cout << "   spawn month herbivore: " << mainArgvs[8] << std::endl;
    std::cout << "PLANTS: " << std::endl;
    std::cout << "         plantFirstGenerationMaxWaterDistance: " << mainArgvs[9] << std::endl;
    std::cout << "           plantFirstGenerationSeedProdFactor: " << mainArgvs[10] << std::endl;
    std::cout << "            plantFirstGenerationSeedLightness: " << mainArgvs[11] << std::endl;
    std::cout << "                   plantFirstGenerationMaxAge: " << mainArgvs[12] << std::endl;
    std::cout << "               plantFirstGenerationFoodChunks: " << mainArgvs[13] << std::endl;
    std::cout << "            plantFirstGenerationDefenseFactor: " << mainArgvs[14] << std::endl;
    std::cout << "      plantFirstGenerationFavoriteTemperature: " << mainArgvs[15] << std::endl;
    std::cout << "          plantFirstGenerationTemperatureSpan: " << mainArgvs[16] << std::endl;

    std::cout << "            plantMutationRateMaxWaterDistance: " << mainArgvs[17] << std::endl;
    std::cout << "              plantMutationRateSeedProdFactor: " << mainArgvs[18] << std::endl;
    std::cout << "               plantMutationRateSeedLightness: " << mainArgvs[19] << std::endl;
    std::cout << "                      plantMutationRateMaxAge: " << mainArgvs[20] << std::endl;
    std::cout << "                  plantMutationRateFoodChunks: " << mainArgvs[21] << std::endl;
    std::cout << "               plantMutationRateDefenseFactor: " << mainArgvs[22] << std::endl;
    std::cout << "         plantMutationRateFavoriteTemperature: " << mainArgvs[23] << std::endl;
    std::cout << "             plantMutationRateTemperatureSpan: " << mainArgvs[24] << std::endl;

    std::cout << "             plantMaxMutationMaxWaterDistance: " << mainArgvs[25] << std::endl;
    std::cout << "               plantMaxMutationSeedProdFactor: " << mainArgvs[26] << std::endl;
    std::cout << "                plantMaxMutationSeedLightness: " << mainArgvs[27] << std::endl;
    std::cout << "                       plantMaxMutationMaxAge: " << mainArgvs[28] << std::endl;
    std::cout << "                   plantMaxMutationFoodChunks: " << mainArgvs[29] << std::endl;
    std::cout << "                plantMaxMutationDefenseFactor: " << mainArgvs[30] << std::endl;
    std::cout << "          plantMaxMutationFavoriteTemperature: " << mainArgvs[31] << std::endl;
    std::cout << "              plantMaxMutationTemperatureSpan: " << mainArgvs[32] << std::endl;

    std::cout << "             plantMinMutationMaxWaterDistance: " << mainArgvs[33] << std::endl;
    std::cout << "               plantMinMutationSeedProdFactor: " << mainArgvs[34] << std::endl;
    std::cout << "                plantMinMutationSeedLightness: " << mainArgvs[35] << std::endl;
    std::cout << "                       plantMinMutationMaxAge: " << mainArgvs[36] << std::endl;
    std::cout << "                   plantMinMutationFoodChunks: " << mainArgvs[37] << std::endl;
    std::cout << "                plantMinMutationDefenseFactor: " << mainArgvs[38] << std::endl;
    std::cout << "          plantMinMutationFavoriteTemperature: " << mainArgvs[39] << std::endl;
    std::cout << "              plantMinMutationTemperatureSpan: " << mainArgvs[40] << std::endl;

    std::cout << "            plantStepMutationMaxWaterDistance: " << mainArgvs[41] << std::endl;
    std::cout << "              plantStepMutationSeedProdFactor: " << mainArgvs[42] << std::endl;
    std::cout << "               plantStepMutationSeedLightness: " << mainArgvs[43] << std::endl;
    std::cout << "                      plantStepMutationMaxAge: " << mainArgvs[44] << std::endl;
    std::cout << "                  plantStepMutationFoodChunks: " << mainArgvs[45] << std::endl;
    std::cout << "               plantStepMutationDefenseFactor: " << mainArgvs[46] << std::endl;
    std::cout << "         plantStepMutationFavoriteTemperature: " << mainArgvs[47] << std::endl;
    std::cout << "             plantStepMutationTemperatureSpan: " << mainArgvs[48] << std::endl;

    std::cout << "HERBIVORE: " << std::endl;
    std::cout << "               herbivoreFirstGenerationMaxAge: " << mainArgvs[49] << std::endl;
    std::cout << "            herbivoreFirstGenerationMaxHunger: " << mainArgvs[50] << std::endl;
    std::cout << "        herbivoreFirstGenerationDefenseFactor: " << mainArgvs[51] << std::endl;
    std::cout << "       herbivoreFirstGenerationQualityOfSense: " << mainArgvs[52] << std::endl;
    std::cout << "  herbivoreFirstGenerationFavoriteTemperature: " << mainArgvs[53] << std::endl;
    std::cout << "      herbivoreFirstGenerationTemperatureSpan: " << mainArgvs[54] << std::endl;
    std::cout << "herbivoreFirstGenerationChildProductionFactor: " << mainArgvs[55] << std::endl;

    std::cout << "                  herbivoreMutationRateMaxAge: " << mainArgvs[56] << std::endl;
    std::cout << "               herbivoreMutationRateMaxHunger: " << mainArgvs[57] << std::endl;
    std::cout << "           herbivoreMutationRateDefenseFactor: " << mainArgvs[58] << std::endl;
    std::cout << "          herbivoreMutationRateQualityOfSense: " << mainArgvs[59] << std::endl;
    std::cout << "     herbivoreMutationRateFavoriteTemperature: " << mainArgvs[60] << std::endl;
    std::cout << "         herbivoreMutationRateTemperatureSpan: " << mainArgvs[61] << std::endl;
    std::cout << "   herbivoreMutationRateChildProductionFactor: " << mainArgvs[62] << std::endl;

    std::cout << "                   herbivoreMaxMutationMaxAge: " << mainArgvs[63] << std::endl;
    std::cout << "                herbivoreMaxMutationMaxHunger: " << mainArgvs[64] << std::endl;
    std::cout << "            herbivoreMaxMutationDefenseFactor: " << mainArgvs[65] << std::endl;
    std::cout << "           herbivoreMaxMutationQualityOfSense: " << mainArgvs[66] << std::endl;
    std::cout << "      herbivoreMaxMutationFavoriteTemperature: " << mainArgvs[67] << std::endl;
    std::cout << "          herbivoreMaxMutationTemperatureSpan: " << mainArgvs[68] << std::endl;
    std::cout << "    herbivoreMaxMutationChildProductionFactor: " << mainArgvs[69] << std::endl;

    std::cout << "                   herbivoreMinMutationMaxAge: " << mainArgvs[70] << std::endl;
    std::cout << "                herbivoreMinMutationMaxHunger: " << mainArgvs[71] << std::endl;
    std::cout << "            herbivoreMinMutationDefenseFactor: " << mainArgvs[72] << std::endl;
    std::cout << "           herbivoreMinMutationQualityOfSense: " << mainArgvs[73] << std::endl;
    std::cout << "      herbivoreMinMutationFavoriteTemperature: " << mainArgvs[74] << std::endl;
    std::cout << "          herbivoreMinMutationTemperatureSpan: " << mainArgvs[75] << std::endl;
    std::cout << "    herbivoreMinMutationChildProductionFactor: " << mainArgvs[76] << std::endl;

    std::cout << "                  herbivoreStepMutationMaxAge: " << mainArgvs[77] << std::endl;
    std::cout << "               herbivoreStepMutationMaxHunger: " << mainArgvs[78] << std::endl;
    std::cout << "           herbivoreStepMutationDefenseFactor: " << mainArgvs[79] << std::endl;
    std::cout << "          herbivoreStepMutationQualityOfSense: " << mainArgvs[80] << std::endl;
    std::cout << "     herbivoreStepMutationFavoriteTemperature: " << mainArgvs[81] << std::endl;
    std::cout << "         herbivoreStepMutationTemperatureSpan: " << mainArgvs[82] << std::endl;
    std::cout << "   herbivoreStepMutationChildProductionFactor: " << mainArgvs[83] << std::endl;

    std::cout << "General PART II: " << std::endl;

    std::cout << "     herbivoreHungerPercentReproduction: " << mainArgvs[84] << std::endl;
    std::cout << "herbivoreTemperaturePercentReproduction: " << mainArgvs[85] << std::endl;
    std::cout << "      plantWaterDistPercentReproduction: " << mainArgvs[86] << std::endl;
    std::cout << "    plantTemperaturePercentReproduction: " << mainArgvs[87] << std::endl;

    std::cout << "               abortIfHerbivoreExtinct: " << mainArgvs[88] << std::endl;
    std::cout << "                  abortIfPlantsExtinct: " << mainArgvs[89] << std::endl;

    std::cout << "           reproductionPercentHerbivore: " << mainArgvs[90] << std::endl;
    std::cout << "              reproductionPercentPlants: " << mainArgvs[91] << std::endl;


    std::cout << "         MinAgeForReproductionHerbivore: " << mainArgvs[92] << std::endl;
    std::cout << "         MaxAgeForReproductionHerbivore: " << mainArgvs[93] << std::endl;
    std::cout << "                     hungerPercentToEat: " << mainArgvs[94] << std::endl;
    std::cout << "                  randomSeed(0=default): " << mainArgvs[95] << std::endl;
    std::cout << "                          withCSVOutput: " << mainArgvs[96] << std::endl;
    std::cout << "                          Visual output: " << mainArgvs[97] << std::endl;
    std::cout << "                 Visual output as video: " << mainArgvs[98] << std::endl;
    std::cout << "                                 Silent: " << mainArgvs[99] << std::endl;

}


