#ifndef _PLANT_H_
#define _PLANT_H_

#include "MutatingPropertyStatic.h"
#include "Point.h"
#include <vector>
#include <string>

class Plant
{

private:
	double _maxWaterDistance;
	static MutatingPropertyStatic _maxWaterDistance_MPS;

	double _seedProdFactor;
	static MutatingPropertyStatic _seedProdFactor_MPS;

	double _seedLightness;
	static MutatingPropertyStatic _seedLightness_MPS;

	double _maxAge;
	static MutatingPropertyStatic _maxAge_MPS;

	double _foodChunks;
	static MutatingPropertyStatic _foodChunks_MPS;

	double _defenseFactor;
	static MutatingPropertyStatic _defenseFactor_MPS;

	double _favoriteTemperature;
	static MutatingPropertyStatic _favoriteTemperature_MPS;

	double _temperatureSpan;
	static MutatingPropertyStatic _temperatureSpan_MPS;

	Point<size_t> _position;
	int _age;

	int _deathMonth;
	int _birthMonth;
	std::string _deathReason;
	int _kidCount;
	int _foodChunksRemaining;
	std::string _phyloName;
	double _isWaterDistance;

	// How much work the plant has put into kids already
	double _reproductionProgress;

	// How much work the plant needs to do to get kids
	static double _percentRequiredForReproduction;

	// Will influence how the reproduction Progress will grow
	// each tick if the water distance is perfect (heuristic)
	static double _reproductionWaterDistancePercent;

	// Will influence how the reproduction Progress will grow
	// each tick if the temperature is perfect (heuristic)
	static double _reproductionTemperaturePercent;

	void _calcReproductionProgress(double inCurrentTemperature);

	bool _isNullPlant;

public:
	// ctor for the primal plant
	Plant();
	Plant(
		double inMaxWaterDistance,
		double inSeedProdFactor,
		double inSeedLightness,
		double inKidMaxAge,
		double inFoodChunks,
		double inDefenseFactor,
		double inFavoriteTemperature,
		double inTemperatureSpan,
		std::string inPhyloName,
		size_t inBirthMonth);
	
	Plant(  Plant inParent,
			Point<size_t> inPosition,
			size_t inCurrentMonth,
			std::string inPhyloName);

	void SetPosition(Point<size_t> inLocation);
	Point<size_t> GetPosition();

	// Will calculate how unthirsty and happy about the temperature 
	// a plant is and use it in its heuristic for the reproduction
	// progress
	void CalculateReproductionProgress(double inCurrentTemperature);

	std::vector<Plant> CreateChildList(size_t inCurrentMonth);

	static MutatingPropertyStatic& GetMaxWaterDistance_MPS();
	static MutatingPropertyStatic& GetSeedProdFactor_MPS();
	static MutatingPropertyStatic& GetSeedLightness_MPS();
	static MutatingPropertyStatic& GetMaxAge_MPS();
	static MutatingPropertyStatic& GetFoodChunks_MPS();
	static MutatingPropertyStatic& GetDefenseFactor_MPS();
	static MutatingPropertyStatic& GetFavoriteTemperature_MPS();
	static MutatingPropertyStatic& GetTemperatureSpan_MPS();

	double GetMaxWaterDistance();
	double GetSeedProdFactor();
	double GetSeedLightness();
	double GetMaxAge();
	double GetFoodChunks();
	double GetDefenseFactor();
	double GetFavoriteTemperature();
	double GetTemperatureSpan();

	static void SetReproductionInfluencePercents(
		double inWaterDistancePercent, 
		double inTemperaturePercent);
	
	static void SetNeededPercentForReproduction(
		double inRequiredProcent);

	void SetDeathMonth(size_t inDeathMonth);

	void Eaten();

	void Tick(double inCurrentTemperature);

	std::string GetPhyloName();

	bool CheckIfWillDie(double inCurrentTemperature);

	void SetWaterDistance(double inWaterDistance);

	bool IsNotNullPlant();
	 
	static std::string GetCSVHeader();
	
	std::string PrintAsCsv();
};

#endif
