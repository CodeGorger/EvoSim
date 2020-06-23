#include "PlantMap.h"

PlantMap::PlantMap(size_t inWidth, size_t inHeight):
	_plantGrids(Grid< std::shared_ptr<Plant> >(inWidth, inHeight, nullptr))
{
	_plantCount = 0;
}
std::vector< std::shared_ptr<Plant> > PlantMap::GetAllPlants()
{
	std::vector< std::shared_ptr<Plant> > ret;
	ret = std::vector< std::shared_ptr<Plant> >();
	for (size_t i = 0; i < _plantGrids.GetHeight(); i++)
	{
		for (size_t j = 0; j < _plantGrids.GetWidth(); j++)
		{
			if (nullptr != _plantGrids.Get(j, i))
			{
				ret.push_back(_plantGrids.Get(j, i));
			}
		}
	}
	return ret;
}

size_t PlantMap::_getWidth()
{
	return _plantGrids.GetWidth();
}

size_t PlantMap::_getHeight()
{
	return _plantGrids.GetHeight();
}


void PlantMap::Set(Point<size_t> inPosition, std::shared_ptr<Plant> inPlant)
{
	_plantGrids.Set(inPosition, inPlant);
	if (inPlant != nullptr)
	{
		_plantCount++;
	}
}



void PlantMap::Remove(Point<size_t> inPos)
{
	_plantGrids.Set(inPos, nullptr);
	_plantCount--;
}


bool PlantMap::HasPlantAt(Point<size_t> inPos)
{
	return (nullptr!=_plantGrids.Get(inPos));
}


void PlantMap::EatenFrom(Point<size_t> inPos) 
{
	if (!HasPlantAt(inPos))
	{
		std::cout << "Failed!" << std::endl;
		return;
	}
	_plantGrids.Get(inPos)->Eaten();
}


double PlantMap::GetDefenseFactor(Point<size_t> inPos)
{
	if (!HasPlantAt(inPos))
	{
		std::cout << "Failed!" << std::endl;
		return 0;
	}
	return _plantGrids.Get(inPos)->GetDefenseFactor();
}


size_t PlantMap::GetPlantCount()
{
	return _plantCount;
}
