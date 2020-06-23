#ifndef _HERBIVORE_H_
#define _HERBIVORE_H_

#include "Point.h"
#include "MutatingPropertyStatic.h"
#include <vector>
#include <string>

class Herbivore
{
private:

	// Age at which the herbivore will die
	double _maxAge;
	static MutatingPropertyStatic _maxAge_MPS;

	// How long a herbivore will survive without food
	double _maxHunger;
	static MutatingPropertyStatic _maxHunger_MPS;

	// How strong a herbivore is. 
	// (relevant for rival fightsand eating plants)
	double _defenseFactor;
	static MutatingPropertyStatic _defenseFactor_MPS;

	// How far and much a herbivore can see
	// and how far it can move.
	double _qualityOfSense;
	static MutatingPropertyStatic _qualityOfSense_MPS;

	// Defines what the perfect temperature is for the being
	double _favoriteTemperature;
	static MutatingPropertyStatic _favoriteTemperature_MPS;

	// Defines how far away the temperature may be 
	// so the being can still bare it
	double _temperatureSpan;
	static MutatingPropertyStatic _temperatureSpan_MPS;

	// Defines how many children a being can have in one
	// 'pregnancy'
	double _childProductionFactor;
	static MutatingPropertyStatic _childProductionFactor_MPS;

	Point<size_t> _location;
	int _age;
	int _hunger;
	std::string _deathReason;
	size_t _deathMonth;
	size_t _birthMonth;
	double _reproductionProgress;
	std::string _phyloName;

	size_t _kidCount;


	static double _percentRequiredForReproduction;

	static double _minAgeForReproduction;
	static double _maxAgeForReproduction;

	static double _eatAtHungerPercent;

	// Will influence how the reproduction Progress will grow
	// each tick if the hunger is perfect (heuristic)
	static double _reproductionHungerPercent;

	// Will influence how the reproduction Progress will grow
	// each tick if the temperature is perfect (heuristic)
	static double _reproductionTemperaturePercent;

	bool _inTempSpan(double inTemperature);

	bool _markedForRivalDeath = false;

public:
	Herbivore();
	Herbivore(
		double inMaxAge,
		double inMaxHunger,
		double inDefenseFactor,
		double inQualityOfSense,
		double inFavoriteTemperature,
		double inTemperatureSpan,
		double inChildProdFactor,
		std::string inPhyloName);

	Herbivore(Herbivore inParent,
		Point<size_t> inPosition,
		size_t inCurrentMonth,
		std::string inPhyloName);

	void SetPosition(Point<size_t> inLocation);
	Point<size_t> GetPosition();

	bool CheckIfWillDie(double inCurrentTemperature);

	std::vector<Herbivore> GetChildList(int month);

	// Will calculate how unhungry and happy about the temperature 
	// a herbivore is and use it in its heuristic for the reproduction
	// progress
	void CalculateReproductionProgress(double inCurrentTemperature);

	bool IsTemperatureInSpan(double inTemperature);

	bool IsHungry();

	void Tick(double inCurrentTemperature);

	static MutatingPropertyStatic& GetMaxAge_MPS();
	static MutatingPropertyStatic& GetMaxHunger_MPS();
	static MutatingPropertyStatic& GetDefenseFactor_MPS();
	static MutatingPropertyStatic& GetQualityOfSense_MPS();
	static MutatingPropertyStatic& GetFavoriteTemperature_MPS();
	static MutatingPropertyStatic& GetTemperatureSpan_MPS();
	static MutatingPropertyStatic& GetChildProductionFactor_MPS();

	double GetMaxAge();
	double GetMaxHunger();
	double GetDefenseFactor();
	double GetQualityOfSense();
	double GetFavoriteTemperature();
	double GetTemperatureSpan();
	double GetChildProductionFactor();

	void DeadDueRival(size_t inMonth);

	static void SetReproductionInfluencePercents(double inHungerPercent, double inTemperaturePercent);
	static void SetNeededPercentForReproduction(double inRequiredProcentForReproduction);

	static void SetReproductionAgeRestrictions(double inMinAgeForReproduction, double inMaxAgeForReproduction);
	static void SetEatAtHungerPercent(double inEatAtHungerPercent);

	void SetDeathMonth(size_t inDeathMonth);

	void ChooseSite(
		std::vector<std::pair<Point<size_t>, bool>> inFoundLocationKandidates,
		std::vector<double> inTemperatureList);

	std::string GetPhyloName();

	void Ate();
	void SetMarkedForRivalDeath();
	bool IsMarkedForRivalDeath();


	std::string PrintAsCsv();
	static std::string GetCSVHeader();
};
#endif

