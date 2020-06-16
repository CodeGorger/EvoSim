#include "LandWaterMap.h"
#include <math.h>
#include <iostream>
#include "EvoSimRandom.h"


LandWaterMap::LandWaterMap() :
    _globalSpawn(Point<size_t>(0, 0)),
    _map(Grid<TileType>(0, 0, TileType::Water))
{

}

LandWaterMap::LandWaterMap(
    size_t inMapHeight,
    size_t inMapWidth,
    double inLandProbability,
    double inLandClusteringAlpha):
    _globalSpawn(Point<size_t>(0,0)),
    _map(Grid<TileType>(inMapWidth, inMapHeight, TileType::Water))
{
    _landGeneration(inMapWidth, inMapHeight, inLandProbability, inLandClusteringAlpha);

    _searchForGlobalSpawn();
}

void LandWaterMap::_searchForGlobalSpawn()
{
    bool foundSpawn = false;
    size_t tries = 0;
    size_t maxTries = 500;

    Point<size_t> mapCenter = Point<size_t>( _map.GetWidth()/2, _map.GetHeight() / 2);
    // Forever, but only 500 times...
    while (!foundSpawn && tries!= maxTries)
    {
        // Search for a possible spawn around the center
        // with increasing radius 
        // std := standard deviation 
        // 1. try: 1/8 of the map width
        // 500. try: 1/2 of the map width
                
        double factorX = ((1 + 3 * ((double)tries) / maxTries) / 8);
        double stdX = ((double)mapCenter.GetX()) * factorX;
        double randX = EvoSimRandom::GetNormalDistRandom(mapCenter.GetX(), stdX);

        double factorY = ((1 + 3 * ((double)tries) / maxTries) / 8);
        double stdY = ((double)mapCenter.GetY()) * factorY;
        double randY = EvoSimRandom::GetNormalDistRandom(mapCenter.GetY(), stdY);
        
        _globalSpawn=Point<size_t>(
            (size_t)floor(randX),
            (size_t)floor(randY)
            );

        // Check spawn location if its within the map and Land ... 
        if (_map.IsLocationWithin(_globalSpawn) &&
            TileType::Land==_map.Get(_globalSpawn))
        {
            foundSpawn = true;
        }
        tries++;
    }
    if (tries >= maxTries)
    {
        std::cout << "Failed to find a spawn after " << tries << " tries." << std::endl;
    }
}

double _tileTypeToVal(TileType inT)
{
    if (TileType::Land == inT)
    {
        return 1;
    }
    if (TileType::Water == inT)
    {
        return 0;
    }
    return 0;
}

void LandWaterMap::_landGeneration(size_t inMapWidth, size_t inMapHeight, double inLandProbability, double inLandClusteringAlpha)
{
    _map = Grid<TileType>(inMapWidth, inMapHeight, TileType::Water);

    // First go over the entire map and completely randomly place land
    // but according to the probabiltiy given ... 
    for (size_t i = 0; i < inMapHeight; i++)
    {
        for (size_t j = 0; j < inMapWidth; j++)
        {
            double p=EvoSimRandom::GetEvenDistRandom(0, 1.0);
            _map.Set(j, i, ((inLandProbability > p) ? TileType::Land : TileType::Water));
        }
    }

    // Having just a random initialized map is strange and unnatural.
    // So take another iteration over the map and cluster land
    for (size_t i = 0; i < inMapHeight; i++)
    {
        for (size_t j = 0; j < inMapWidth; j++)
        {
            double tmpProb = 0;
            double tmpWeight = 0;
            if (i > 0 && j > 0)
            {
                tmpProb = tmpProb + 
                    _tileTypeToVal(_map.Get(i - 1, j - 1)) *
                    inLandClusteringAlpha;
                tmpWeight++;
            }
            if (j > 0)
            {
                tmpProb = tmpProb +
                    _tileTypeToVal(_map.Get(i, j - 1)) *
                    inLandClusteringAlpha;
                tmpWeight++;
            }
            if (i > 0)
            {
                tmpProb = tmpProb +
                    _tileTypeToVal(_map.Get(i - 1, j)) *
                    inLandClusteringAlpha;
                tmpWeight++;
            }
            if (i < inMapHeight - 1 && j < inMapWidth - 1)
            {
                tmpProb = tmpProb +
                    _tileTypeToVal(_map.Get(i + 1, j + 1)) *
                    inLandClusteringAlpha;
                tmpWeight++;
            }
            if (j < inMapWidth - 1)
            {
                tmpProb = tmpProb +
                    _tileTypeToVal(_map.Get(i, j + 1)) *
                    inLandClusteringAlpha;
                tmpWeight++;
            }
            if (i < inMapHeight - 1)
            {
                tmpProb = tmpProb +
                    _tileTypeToVal(_map.Get(i + 1, j)) *
                    inLandClusteringAlpha;
                tmpWeight++;
            }
            if (i < inMapHeight - 1 && j>0)
            {
                tmpProb = tmpProb +
                    _tileTypeToVal(_map.Get(i + 1, j - 1)) *
                    inLandClusteringAlpha;
                tmpWeight++;
            }
            if (i > 0 && j < inMapWidth - 1)
            {
                tmpProb = tmpProb +
                    _tileTypeToVal(_map.Get(i - 1, j + 1)) *
                    inLandClusteringAlpha;
                tmpWeight++;
            }
            tmpProb = tmpProb +
                _tileTypeToVal(_map.Get(i, j)) *
                inLandClusteringAlpha*1.5;
            tmpWeight++; 

            // if at the edge of the map, factor will correct that not all fields are used...
            double factor = 9.0 / tmpWeight;
            if (0.6 < tmpProb * factor)
            {
                _map.Set(i, j, TileType::Land);
            }
            if (tmpProb * factor < 0.4)
            {
                _map.Set(i, j, TileType::Water);
            }
        }
    }
}

Point<size_t> LandWaterMap::_moveTowardButtomLeft(Point<size_t> inPoint)
{
    Point<size_t> ret = inPoint;

    if (ret.GetX() >= 1)
    {
        ret.SetX(ret.GetX() - 1);
    }
    if (ret.GetY() >= 1)
    {
        ret.SetY(ret.GetY() - 1);
    }

    return ret;
}

Point<size_t> LandWaterMap::_moveTowardUpRight(Point<size_t> inPoint)
{
    Point<size_t> ret = inPoint;

    if (ret.GetX() < _map.GetWidth())
    {
        ret.SetX(ret.GetX() + 1);
    }
    if (ret.GetY() < _map.GetHeight())
    {
        ret.SetY(ret.GetY() + 1);
    }

    return ret;
}

double LandWaterMap::checkSearchWindowForWater(
    Point<size_t> botLeft, 
    Point<size_t> topRight, 
    Point<size_t> inDistanceTo)
{
    double ret = -1;
    for (size_t i = botLeft.GetX(); i < topRight.GetX(); i++)
    {
        for (size_t j = botLeft.GetY(); j < topRight.GetY(); j++)
        {
            Point<size_t> tmpPotentialWaterSource = Point<size_t>(i, j);
            if (TileType::Water == _map.Get(tmpPotentialWaterSource))
            {
                double tmpDist = inDistanceTo.GetDistance(tmpPotentialWaterSource);
                if (tmpDist < ret || ret == -1)
                {
                    ret = tmpDist;
                }
            }
        }
    }
    return ret;
}

double LandWaterMap::GetWaterDistance(Point<size_t> inDistanceTo)
{
    Point<size_t> botLeft = inDistanceTo;
    Point<size_t> topRight = inDistanceTo;

    double waterDistance = -1;
    // Run 10 iterations, searching with the window increased everytime
    for (size_t i = 0; i<10 ; i++)
    {
        botLeft = _moveTowardButtomLeft(botLeft);
        topRight = _moveTowardUpRight(topRight);

        waterDistance=checkSearchWindowForWater(botLeft, topRight, inDistanceTo);

        if (waterDistance >= 0)
        {
            return waterDistance;
        }
    }
    std::cout << "Fail to find water!" <<  std::endl;
    return 0.0;
}

bool LandWaterMap::IsLand(Point<size_t> inLocation)
{
    return (_map.Get(inLocation) == TileType::Land);
}

// Will return a position on the map
// (but evtl will also say it failed and be 0, 0)
// if it didnt fail, the returned position is in the map and
// within the given max distance
std::pair<Point<size_t>, bool> LandWaterMap::SearchSpawn(
    int inTries, 
    double inMaxDist, 
    Point<size_t> inSearchCenter )
{
    while (inTries > 0)
    {
        inTries--;
        double tryX = EvoSimRandom::GetEvenDistRandom(
            inSearchCenter.GetX() - inMaxDist,
            inSearchCenter.GetX() + inMaxDist);
        double tryY = EvoSimRandom::GetEvenDistRandom(
            inSearchCenter.GetY() - inMaxDist,
            inSearchCenter.GetY() + inMaxDist);
        Point<size_t> tryLocation = Point<size_t>(
            (size_t) floor(tryX),
            (size_t) floor(tryY));

        // If it is a legal position inside the maps boundaries
        if (_map.IsLocationWithin(tryLocation))
        {
            // If it is not further away then allowed
            if(inMaxDist >= tryLocation.GetDistance(inSearchCenter))
            {
                // If its land
                if (TileType::Land == _map.Get(tryLocation))
                {
                    return std::pair<Point<size_t>, bool> (tryLocation,true);
                }
            }
        }
    }
    return std::pair<Point<size_t>, bool>(Point<size_t>(0, 0), false);
}

Point<size_t> LandWaterMap::GetGlobalSpawn()
{
    return _globalSpawn;
}

Point<size_t> LandWaterMap::SearchPrimalSpawnCandidate(double inStandardDevFactor)
{
    if (inStandardDevFactor < 0.2)
    {
        inStandardDevFactor = 0.2;
    }
    if (inStandardDevFactor > 0.8)
    {
        inStandardDevFactor = 0.8;
    }

    Point<size_t> globalSpawn=GetGlobalSpawn();
    
    size_t maxTries = 30;
    for (size_t tries = 0; tries < maxTries; tries++)
    {
        double factor = ((1 + 7 * ((double)tries) / maxTries) / 8);
        double stdX = 
            ((double)_map.GetWidth()) * 
            factor *
            inStandardDevFactor;
        double stdY = 
            ((double)_map.GetHeight()) *
            factor *
            inStandardDevFactor;
        double tryX = EvoSimRandom::GetNormalDistRandom( globalSpawn.GetX(), stdX );
        double tryY = EvoSimRandom::GetNormalDistRandom( globalSpawn.GetY(), stdY );

        Point<size_t> _spawnCandidate = Point<size_t>(
            (size_t) floor(tryX),
            (size_t) floor(tryY) );
        if ( _map.IsLocationWithin(_spawnCandidate) && 
            TileType::Land == _map.Get(_spawnCandidate))
        {
            return _spawnCandidate;
        }
    }
    std::cout << "Failed to find a spawn." << std::endl;
    return Point<size_t>(0, 0);
}




size_t LandWaterMap::GetWidth()
{
    return _map.GetWidth();
}
size_t LandWaterMap::GetHeight()
{
    return _map.GetHeight();
}
Grid<TileType> LandWaterMap::GetMap()
{
    return _map;
}