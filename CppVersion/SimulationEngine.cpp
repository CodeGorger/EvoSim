#include "SimulationEngine.h"
#include <algorithm>
#include <cmath>
#include "EvoSimRandom.h"
#include "CImg.h"
#include <fstream>

SimulationEngine::SimulationEngine() :
    _herbivoreExtinct(false),
    _plantsExtinct(false),
    _plantMap(PlantMap(0, 0)),
    _primalPlant(Plant()),
    _primalPlantCount(0),
    _primalHerbivore(Herbivore()),
    _primalHerbivoreCount(0),
    _abortIfHerbivoreDistint(true),
    _abortIfPlantsDistint(true),
    _isCsvFileRequired(true),
    _tickCounter(0),
    _primalHerbivoreSpawnMonth(10),
    _maxIterations(1000)
{
    _plantAbundanceHistory = std::vector<size_t>();
    _herbivoreAbundanceHistory = std::vector<size_t>();
}

void SimulationEngine::SetWorld(LandWaterMap inWorld)
{
    _world = inWorld;
    _plantMap = PlantMap(
        inWorld.GetWidth(),
        inWorld.GetHeight());
}

void SimulationEngine::SetCsvRequired(bool inCsvRequired)
{
    _isCsvFileRequired = inCsvRequired;
}

void SimulationEngine::SetVisualOutputRequired(bool inIsVisualOutputRequired)
{

}

void SimulationEngine::SetPrimalPlant(Plant inPrimalPlant, size_t inPrimalPlantCount)
{
    _primalPlant = inPrimalPlant;
    _primalPlantCount = inPrimalPlantCount;
}

void SimulationEngine::SetPrimalHerbivore(
    Herbivore inPrimalHerbivore,
    size_t inPrimalHerbivoreCount, 
    size_t inPrimalHerbivoreSpawnMonth)
{
    _primalHerbivore = inPrimalHerbivore;
    _primalHerbivoreCount = inPrimalHerbivoreCount; 
    _primalHerbivoreSpawnMonth = inPrimalHerbivoreSpawnMonth;
}

void SimulationEngine::SetAbortIfHerbivoreDistint(bool inAbortIfHerbivoreDistint)
{
    _abortIfHerbivoreDistint = inAbortIfHerbivoreDistint;
}

void SimulationEngine::SetAbortIfPlantsDistint(bool inAbortIfPlantsDistint)
{
    _abortIfPlantsDistint = inAbortIfPlantsDistint;
}

void SimulationEngine::SetMaxIterations(int inMaxIterations)
{
    _maxIterations = inMaxIterations;
}

// The simulation really really starts here
int SimulationEngine::Run()
{
    int ret = 0;
    _spawnPrimalPlants();

    for (size_t t = 0; t < _maxIterations; t++)
    {
        if (t == _primalHerbivoreSpawnMonth)
        {
            _spawnPrimalHerbivore();
        }

        if (t%10 == 0)
        {
            PrintMapImage();
        }

        _tick();

        if (_abortIfHerbivoreDistint && _areAllHerbivoreExtinct())
        {
            std::cout << "Herbivore Extinct" << std::endl;
            ret = 1;
            break;
        }

        if (_abortIfPlantsDistint && _areAllPlantsExtinct())
        {
            std::cout << "Plants Extinct" << std::endl;
            ret = 2;
            break;
        }
    }


    _makeAbundanceCsv();
    _printPlantsHerbivors();

    return ret;
}


void SimulationEngine::_makeAbundanceCsv()
{
    std::ofstream aFile;
    aFile.open("plantHerbivoreAbundance.csv");

    aFile << "Month,PlantAbundance,HerbivoreAbundance\n";

    for (size_t i = 0; i < _tickCounter ; i++)
    {
        aFile << std::to_string(i)+",";
        aFile << std::to_string(_plantAbundanceHistory[i]) + ",";
        aFile << std::to_string(_herbivoreAbundanceHistory[i]) + "\n";
    }

    aFile.close();
}

void SimulationEngine::_printPlantsHerbivors()
{
    std::ofstream hFile;
    hFile.open("herbivore.csv");

    hFile << Herbivore::GetCSVHeader() + "\n";
    for (size_t i = 0; i <_deadHerbivore.size(); i++)
    {
        hFile << _deadHerbivore[i].PrintAsCsv() + "\n";
    }
    for (size_t i = 0; i < _allHerbivore.size(); i++)
    {
        hFile << _allHerbivore[i].PrintAsCsv() + "\n";
    }
    hFile.close();


    std::ofstream pFile;
    pFile.open("plants.csv");

    pFile << Plant::GetCSVHeader() + "\n";

    std::vector< std::shared_ptr<Plant> > allPlants;
    allPlants = _plantMap.GetAllPlants();
    for (size_t i = 0; i < allPlants.size(); i++)
    {
        pFile << allPlants[i]->PrintAsCsv() + "\n";
    }
    for (size_t i = 0; i < _deadPlants.size(); i++)
    {
        pFile << _deadPlants[i]->PrintAsCsv() + "\n";
    }

    pFile.close();
}

bool SimulationEngine::_areAllHerbivoreExtinct()
{
    return _herbivoreExtinct;
}


bool SimulationEngine::_areAllPlantsExtinct()
{
    return _plantsExtinct;
}


void SimulationEngine::_addPlant(
    Plant inParent,
    size_t inCurrentMonth,
    Point<size_t> inPosition,
    size_t id)
{
    std::shared_ptr<Plant> p;
    p = std::shared_ptr<Plant>( new Plant(
        inParent,
        inPosition,
        inCurrentMonth,
        inParent.GetPhyloName()+"-"+std::to_string(id)));

    double wd = _world.GetWaterDistance(inPosition);
    p->SetWaterDistance(wd);
    _plantMap.Set(inPosition, p);
}


void SimulationEngine::_addHerbivore(
    Herbivore inParent,
    size_t inCurrentMonth,
    Point<size_t> inPosition,
    size_t id)
{
    Herbivore tmp = 
        Herbivore(  inParent,
                    inPosition,
                    inCurrentMonth,
                    inParent.GetPhyloName() + "-" + std::to_string(id));
    _allHerbivore.push_back(tmp);
}


void SimulationEngine::_spawnPrimalPlants()
{    
    for (size_t p = 0; p < _primalPlantCount; p++)
    {
        int failedSetTries = 0;
        bool badPlantPos = true;

        //Retry finding a space until the place is good.
        // or until 30 tries have failed
        while (badPlantPos && failedSetTries < 30)
        {
            //Get a random land space near the spawn on land.
            Point<size_t> tryPos = _world.SearchPrimalSpawnCandidate(0.2);

            // If there is already a plant there
            if(_plantMap.HasPlantAt(tryPos))
            {
                failedSetTries++;
                badPlantPos = true;
            }
            else
            {// If there isnt already a plant there, plant the plant
                _addPlant(_primalPlant, 0, tryPos, p);
            }
        }
    }
}

void SimulationEngine::_spawnPrimalHerbivore()
{
    for (size_t p = 0; p < _primalHerbivoreCount; p++)
    {
        Point<size_t> tryPos = _world.SearchPrimalSpawnCandidate(0.2);

        _addHerbivore(_primalHerbivore, _tickCounter, tryPos, p);
    }
}

void SimulationEngine::PrintMapImage()
{
    _drawMap("img" + std::to_string(_tickCounter) + ".bmp");
}



void SimulationEngine::_drawMap(std::string filename)
{
    const unsigned char redSpawn[] = { 255,0,0 };
    const unsigned char greenLand[] = { 0,100,0 };
    const unsigned char greenPlant[] = { 0,200,0 };
    const unsigned char blueWater[] = { 0,0,130 };
    const unsigned char tealPlantHerbivore[] = { 0,255,255 };
    const unsigned char blueHerbivore[] = { 0,128,255 };
    const unsigned char pinkHuh[] = { 255,00,130 };

    size_t w=_world.GetWidth();
    size_t h= _world.GetHeight();
    size_t pixelWidth = 4;

    unsigned int xSize = ((unsigned int)(w * pixelWidth));
    unsigned int ySize = ((unsigned int)(h * pixelWidth));
    unsigned int zSize = 1;
    unsigned int colorDepth = 3;
    unsigned char initValue = 0;
    cimg_library::CImg<unsigned char> output(
        xSize, ySize, zSize, colorDepth, initValue);

    for (size_t i = 0; i < h; i++)
    {
        for (size_t j = 0; j < w; j++)
        {
            unsigned char currentColor[] = { 0,0,0 };
            Point<size_t> p = Point<size_t>(j, i);
            if (_plantMap.HasPlantAt(p))
            {
                currentColor[0] = greenPlant[0];
                currentColor[1] = greenPlant[1];
                currentColor[2] = greenPlant[2];
            }
            else
            {
                switch (_world.GetMap().Get(j, i))
                {
                case TileType::Land:
                    currentColor[0] = greenLand[0];
                    currentColor[1] = greenLand[1];
                    currentColor[2] = greenLand[2];
                    break;

                case TileType::Water:
                    currentColor[0] = blueWater[0];
                    currentColor[1] = blueWater[1];
                    currentColor[2] = blueWater[2];
                    break;

                default:
                    currentColor[0] = pinkHuh[0];
                    currentColor[1] = pinkHuh[1];
                    currentColor[2] = pinkHuh[2];
                    std::cout << "Huh?! default..." << std::endl;
                    break;
                }
            }

            int x0 = ((int)(j * pixelWidth));
            int y0 = ((int)(i * pixelWidth));
            int x1 = ((int)((j + 1) * pixelWidth - 1));
            int y1 = ((int)((i + 1) * pixelWidth - 1));

            output.draw_rectangle(
                x0, y0, x1, y1,
                currentColor, 
                1.0f);
        }
    }

    for (size_t i = 0; i < _allHerbivore.size(); i++)
    {
        Point<size_t> p = _allHerbivore[i].GetPosition();
        size_t x = p.GetX();
        size_t y = p.GetY();
        if (_plantMap.HasPlantAt(p))
        {
            int x0 = ((int)(x * pixelWidth));
            int y0 = ((int)(y * pixelWidth));
            int x1 = ((int)((x + 1) * pixelWidth - 1));
            int y1 = ((int)((y + 1) * pixelWidth - 1));
             
            output.draw_rectangle(
                x0, y0, x1, y1,
                tealPlantHerbivore, 
                1.0f);
        }
        else
        {
            int x0 = ((int)(x * pixelWidth));
            int y0 = ((int)(y * pixelWidth));
            int x1 = ((int)((x + 1) * pixelWidth - 1));
            int y1 = ((int)((y + 1) * pixelWidth - 1));

            output.draw_rectangle(
                x0, y0, x1, y1,
                blueHerbivore,
                1.0f);
        }
    }

    output.save_bmp(filename.c_str());
}









void SimulationEngine::_tick()
{
    // It's the next month
    _tickCounter++;
    _calculateTemperatures();

    // Herbivore space constraints rivalery
    _herbivoreTickRivalery();

    // Herbivore temperature constraints rivalery
    _herbivoreTickTemperature();

    // Plant 
    _plantTickAgingTemperature();

    _plantAbundanceHistory.push_back(_plantMap.GetPlantCount());
    _herbivoreAbundanceHistory.push_back(_allHerbivore.size());

    _tickExtinction();

    // HERBIVORE
    _tickHandleLivingHerbivores();
   
    // PLANTS
    _tickHandleLivingPlants();
}


bool _compareHerbivore(std::pair <double, size_t> i1, std::pair <double, size_t> i2)
{
    return (i1.first < i2.first);
}

// Will return the id's of the herbivore must die
// will check the defense factor of all incoming and will let return all
// but the three strongest
std::vector<size_t> SimulationEngine::_determineStrongestHerbivores(
    std::vector< std::pair <double, size_t>> inDefenseAndId)
{
    sort(inDefenseAndId.begin(), inDefenseAndId.end(), _compareHerbivore);
    std::vector<size_t> ret;
    for(size_t i = 3; i < inDefenseAndId.size(); i++)
    {
        ret.push_back(inDefenseAndId[i].second);
    }
    return ret;
}


void SimulationEngine::_herbivoreTickRivalery()
{
    if (_tickCounter-1==_primalHerbivoreSpawnMonth)
    {
        // If this wouldnt be done, all herbivores but 3
        // of the primal herbivore would die due rivalery
        return;
    }
    std::vector< std::pair <double,size_t>> defenseAndId;
    
    // Foreach Herbivore
    for (size_t i = 0; i < _allHerbivore.size(); i++)
    {
        defenseAndId.clear();
        defenseAndId.push_back(std::pair <double, size_t>(_allHerbivore[i].GetDefenseFactor(),i));

        // find all other with the same locations
        for (size_t j = i+1; j < _allHerbivore.size(); j++)
        {
            if (_allHerbivore[j].GetPosition().Equals(_allHerbivore[i].GetPosition()))
            {
                defenseAndId.push_back(std::pair <double, size_t>(_allHerbivore[j].GetDefenseFactor(), j));
            }
        }

        // all but three herbivore with the strongest defense factor must die
        // due to complicated index shifting, every to-die herbivore will be marked
        // in a first run and in a second will be xxx-ed
        std::vector<size_t> mustDieIds=_determineStrongestHerbivores(defenseAndId);
        for (size_t d = 0; d < mustDieIds.size(); d++)
        {
            _allHerbivore[mustDieIds[d]].SetMarkedForRivalDeath();
        }
        for(size_t d = 0; d < _allHerbivore.size(); d++)
        {
            if (_allHerbivore[d].IsMarkedForRivalDeath())
            {
                _allHerbivore[d].DeadDueRival(_tickCounter);
                _deadHerbivore.push_back(_allHerbivore[d]);
                _allHerbivore.erase(_allHerbivore.begin() + d);
                d--;
            }
        }
    }
}

void SimulationEngine::_herbivoreTickTemperature()
{
    // Foreach Herbivore
    for (size_t i = 0; i < _allHerbivore.size(); i++)
    {
        size_t y_location=_allHerbivore[i].GetPosition().GetY();
        double tmpTemperature = _getTemperature(y_location);
        if (_allHerbivore[i].CheckIfWillDie(tmpTemperature))
        {
            _allHerbivore[i].SetDeathMonth(_tickCounter);
            if (_isCsvFileRequired)
            {
                _deadHerbivore.push_back(_allHerbivore[i]);
            }
            _allHerbivore.erase(_allHerbivore.begin() + i);
        }
    }
}


void SimulationEngine::_plantTickAgingTemperature()
{
    std::vector< std::shared_ptr<Plant> > allPlants;
    allPlants = _plantMap.GetAllPlants();
    for (size_t i = 0; i < allPlants.size(); i++)
    {
        size_t y_location = allPlants[i]->GetPosition().GetY();
        double tmpTemperature = _getTemperature(y_location);
        if (allPlants[i]->CheckIfWillDie(tmpTemperature))
        {
            allPlants[i]->SetDeathMonth(_tickCounter);
            if (_isCsvFileRequired)
            {
                _deadPlants.push_back(allPlants[i]);
            }
            Point<size_t> pos = allPlants[i]->GetPosition();
            _plantMap.Remove(pos); 
        }
    }
}


void SimulationEngine::_tickExtinction()
{
    if (_plantMap.GetPlantCount() == 0)
    {
        _plantsExtinct = true;
    }

    if (_allHerbivore.size() == 0 && _primalHerbivoreSpawnMonth < _tickCounter)
    {
        _herbivoreExtinct = true;
    }
}



// Will cycle through all herbivores and do:
//   h.Tick ... Calc individual progress (age, hunger, reproduction maturity)
//   search and choose a new site
//   an evtl let them eat
void SimulationEngine::_tickHandleLivingHerbivores()
{
    for (size_t i = 0; i < _allHerbivore.size(); i++)
    {
        Herbivore & h = _allHerbivore[i];
        size_t y_location = h.GetPosition().GetY();
        double tmpTemperature = _getTemperature(y_location);
        h.Tick(tmpTemperature);
        std::vector<std::pair<Point<size_t>, bool>> foundLocationKandidates;
        foundLocationKandidates = SearchFoodAndLocationsForHerbivore(h);
        h.ChooseSite(
            foundLocationKandidates,
            _temperatureList);

        if (_plantMap.HasPlantAt(h.GetPosition()))
        {
            // This plant will get eaten(partitially) by the herbivore h
            // but only if the defense factor are ok
            // and the herbivore is hungry enough
            if (h.IsHungry() && 
                _plantMap.GetDefenseFactor(h.GetPosition()) <= h.GetDefenseFactor())
            {
                _plantMap.EatenFrom(h.GetPosition());
                h.Ate();
            }
        }
        std::vector<Herbivore> childHerbivoreList = h.GetChildList(((int)_tickCounter));
        _allHerbivore.insert(_allHerbivore.end(), childHerbivoreList.begin(), childHerbivoreList.end());
    }
}

// Will return a valid list of fields and a bool if it has food of not
std::vector<std::pair<Point<size_t>, bool>>
    SimulationEngine::SearchFoodAndLocationsForHerbivore(
    Herbivore inHerbivore)
{
    double q = inHerbivore.GetQualityOfSense();
    int searchTries = (int)floor(q * q / sqrt(q));

    // will return a location and if it was successful
    Point<size_t> p = inHerbivore.GetPosition();

    std::vector < std::pair<Point<size_t>, bool>> returnValue= std::vector < std::pair<Point<size_t>, bool>>();

    for (int t = 0; t< int(std::max(3, searchTries)); t++)
    {
        // will return a location and the bool if it was successful
        std::pair<Point<size_t>, bool> tmpSite = _world.SearchSpawn(3, q + 2, p);
        if (tmpSite.second)
        {
            // tmpSite is just being reused. Yet, the meaning of the boolean 
            // changes from "success" to "is there food"
            tmpSite.second = (_plantMap.HasPlantAt(tmpSite.first));
            returnValue.push_back(tmpSite);
        }
    }
    return returnValue;
}

double SimulationEngine::_getTemperature(size_t y_location)
{
    return _temperatureList[y_location];
}  


void SimulationEngine::_calculateTemperatures()
{
    // hh ... height of map
    size_t hh = _world.GetHeight();
    // hh / 2 ... equator
    // hh * 0.1 * sin((self.tickCounter % 12) / 12 * pi * 2) ... change for month
    // mtp ... max temperature position
    double mtp = sin(((_tickCounter % 12) / 12) * 3.14159265 * 2);

    // d ... deviation
    double d = EvoSimRandom::GetNormalDistRandom(0.1, 0.05);

    double tmpYOfMaxTemperature = hh / 2 + hh * d * mtp;
    

    std::vector<double> tmpTemperatureList = std::vector<double>();

    double yOMT = tmpYOfMaxTemperature;
    double tMax = EvoSimRandom::GetNormalDistRandom(40, 3);

    for (size_t i = 0; i < hh; i++)
    {
        tmpTemperatureList.push_back(tMax - ((double)2 * (50)) * abs((yOMT - ((double)i)) / ((double)hh)));
    }
    _temperatureList = tmpTemperatureList;

}




void SimulationEngine::_tickHandleLivingPlants()
{
    std::vector< std::shared_ptr<Plant> > allPlants = _plantMap.GetAllPlants();

    for (size_t i = 0; i < allPlants.size(); i++)
    {
        size_t h=allPlants[i]->GetPosition().GetY();
        allPlants[i]->Tick(_getTemperature(h));
        _plantKidPlants(
            allPlants[i]->CreateChildList(_tickCounter),
            allPlants[i]->GetSeedLightness(),
            allPlants[i]->GetPosition());
    }
}


void SimulationEngine::_plantKidPlants(
    std::vector<std::shared_ptr<Plant> > inKids,
    double inSeedlightnessfactor,
    Point<size_t> inMotherLocation)
{
    for (size_t i=0; i < inKids.size(); i++)
    {
        //k in kids :
        int failedSetTries = 0;
        bool hasBeenPlanted = false;
        // Retry finding a space until the place is good.
        while (!hasBeenPlanted && failedSetTries++ < 10)
        {
            // see Plant::getChildList()->x, y is motherXand motherY until we found a proper X, Y
            Point<size_t> motherPosition = inMotherLocation;

            // Check the map for a random space near the spawn on land. (will also return successful find)
            std::pair<Point<size_t>,bool> tryPos = _world.SearchSpawn(10, inSeedlightnessfactor, motherPosition);

            if (tryPos.second)
            {
                Point<size_t> pos = tryPos.first;
                // Only place it there if no other plant has occupied this place.
                if(!_plantMap.HasPlantAt(pos))
                {
                    // The first time this happens a place was found
                    inKids[i]->SetPosition(pos);
                    double wd = _world.GetWaterDistance(pos);
                    inKids[i]->SetWaterDistance(wd);
                    _plantMap.Set(pos, inKids[i]);
                    hasBeenPlanted = true;
                }
            }
        }
        if (failedSetTries >= 10)
        {
            //std::cout << "Failed to set kid plants" << std::endl;
        }
    }
}