#ifndef _SIMULATIONENGINE_H_
#define _SIMULATIONENGINE_H_

#include "Plant.h"
#include "Herbivore.h"
#include "LandWaterMap.h"
#include <vector>
#include <memory>

class SimulationEngine
{
private:
	std::vector<Herbivore> _allHerbivore;
	std::vector<Herbivore> _deadHerbivore;

	std::vector<size_t> _plantAbundanceHistory;
	std::vector<size_t> _herbivoreAbundanceHistory;

	// std::vector<Plant> _allPlants;
	Grid<Plant> _plantMap;
	std::vector<Plant> _deadPlants;

	size_t _plantCounter;
	LandWaterMap _world;
	bool _isCsvFileRequired;
	size_t _maxIterations;
	size_t _tickCounter;

	Plant _primalPlant;
	size_t _primalPlantCount;

	Herbivore _primalHerbivore;
	size_t _primalHerbivoreCount;
	size_t _primalHerbivoreSpawnMonth;

	bool _abortIfHerbivoreDistint;
	bool _abortIfPlantsDistint;

	bool _herbivoreExtinct;
	bool _plantsExtinct;

	std::vector<double> _temperatureList;


	void _makeAbundanceCsv();
	void _printPlantsHerbivors();

	bool _areAllHerbivoreExtinct();
	bool _areAllPlantsExtinct();



	void _addPlant(
		Plant inParent,
		size_t inCurrentMonth,
		Point<size_t> inPosition,
		int id);
	void _addHerbivore(
		Herbivore inParent,
		size_t inCurrentMonth,
		Point<size_t> inPosition,
		int id);




	void _spawnPrimalPlants();
	void _spawnPrimalHerbivore();


	void _drawMap(std::string filename);

	void _tick();

	std::vector<size_t> _determineStrongestHerbivores(
		std::vector< std::pair <double, size_t>> inDefenseAndId);

	void _herbivoreTickRivalery();
	void _herbivoreTickTemperature();
	void _plantTickAgingTemperature();
	void _tickExtinction();
	void _tickHandleLivingHerbivores();
	void _tickHandleLivingPlants();
	void _plantKidPlants(
		std::vector<Plant> inKids,
		double seedlightnessfactor, 
		Point<size_t> inMotherLocation);

	void _calculateTemperatures();
	double _getTemperature(size_t y_location);

public:
	SimulationEngine();

	void PrintMapImage();

	int Run();

	void SetWorld(LandWaterMap inWorld);
	void SetCsvRequired(bool inCsvRequired);

	void SetVisualOutputRequired(bool inIsVisualOutputRequired);
	void SetPrimalPlant(Plant inPrimalPlant, size_t inPrimalPlantCount );
	void SetPrimalHerbivore(Herbivore inPrimalHerbivore, size_t inPrimalHerbivoreCount, size_t inPrimalHerbivoreSpawnMonth);
	void SetAbortIfHerbivoreDistint(bool inAbortIfHerbivoreDistint);
	void SetAbortIfPlantsDistint(bool inAbortIfPlantsDistint);

	void SetMaxIterations(int inMaxIterations);

	std::vector < std::pair<Point<size_t>, bool> > SearchFoodAndLocationsForHerbivore(Herbivore inHerbivore);
};


#endif