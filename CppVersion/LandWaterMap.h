#ifndef _LAND_WATER_MAP_
#define _LAND_WATER_MAP_

#include "Point.h"
#include "Grid.h"
#include "TileType.h"


class LandWaterMap
{
public:
	LandWaterMap();
	LandWaterMap(
		size_t inMapHeight,
		size_t inMapWidth,
		double inLandProbability,
		double inLandClusteringAlpha);

	// Will search in the search-window for water,
	// and if water was found, give back the distance
	double checkSearchWindowForWater(
		Point<size_t> botLeft,
		Point<size_t> topRight,
		Point<size_t> inDistanceTo);

	// Will search for water and give back the distance
	double GetWaterDistance(Point<size_t> inDistanceTo);

	bool IsLand(Point<size_t> inLocation);

	// Returns the found location and if it was possible to find a place
	std::pair<Point<size_t>, bool> SearchSpawn(
		int inTries, 
		double inMaxDist, 
		Point<size_t> inSearchCenter);

	Point<size_t> GetGlobalSpawn();

	Point<size_t> SearchPrimalSpawnCandidate(double inStandardDevFactor);

	size_t GetWidth();
	size_t GetHeight();

	Grid<TileType> GetMap();

private:
	Point<size_t> _globalSpawn;
	Grid<TileType> _map;

	void _landGeneration(size_t inMapWidth, size_t inMapHeight, double inLandProbability, double inLandClusteringAlpha);
	
	void _searchForGlobalSpawn();

	Point<size_t> _moveTowardButtomLeft(Point<size_t> inPoint);
	Point<size_t> _moveTowardUpRight(Point<size_t> inPoint);
};

#endif


