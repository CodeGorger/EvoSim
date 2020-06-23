#ifndef _PLANTMAP_H_
#define _PLANTMAP_H_

#include "Grid.h"
#include "Plant.h"
#include <memory>

class PlantMap
{
public:
	PlantMap(size_t inWidth, size_t inHeight);
	std::vector< std::shared_ptr<Plant> > GetAllPlants();
	void Set(Point<size_t> inPosition, std::shared_ptr<Plant> inPlant);
	void Remove(Point<size_t> inPos);
	bool HasPlantAt(Point<size_t> inPos);
	void EatenFrom(Point<size_t> inPos);
	double GetDefenseFactor(Point<size_t> inPos);

	size_t GetPlantCount();
private:
	Grid< std::shared_ptr<Plant> > _plantGrids;
	size_t _plantCount;

	size_t _getWidth();
	size_t _getHeight();
};


#endif

