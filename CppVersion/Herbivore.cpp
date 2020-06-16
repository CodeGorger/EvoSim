#include "Herbivore.h"
#include <algorithm>



MutatingPropertyStatic Herbivore::_maxAge_MPS = MutatingPropertyStatic();
MutatingPropertyStatic Herbivore::_maxHunger_MPS = MutatingPropertyStatic();
MutatingPropertyStatic Herbivore::_defenseFactor_MPS = MutatingPropertyStatic();
MutatingPropertyStatic Herbivore::_qualityOfSense_MPS = MutatingPropertyStatic();
MutatingPropertyStatic Herbivore::_favoriteTemperature_MPS = MutatingPropertyStatic();
MutatingPropertyStatic Herbivore::_temperatureSpan_MPS = MutatingPropertyStatic();
MutatingPropertyStatic Herbivore::_childProductionFactor_MPS = MutatingPropertyStatic();

double Herbivore::_percentRequiredForReproduction = 150;
double Herbivore::_minAgeForReproduction = 0.2;
double Herbivore::_maxAgeForReproduction = 0.7;
double Herbivore::_eatAtHungerPercent = 0.35;
double Herbivore::_reproductionHungerPercent = 4;
double Herbivore::_reproductionTemperaturePercent = 4;



void Herbivore::SetPosition(Point<size_t> inLocation)
{
    _location = inLocation;
}

Point<size_t> Herbivore::GetPosition()
{
    return _location;
}


Herbivore::Herbivore() :
    _location(Point<size_t>(0, 0)),
    _maxAge(0),
    _maxHunger(0),
    _defenseFactor(0),
    _qualityOfSense(0),
    _favoriteTemperature(0),
    _temperatureSpan(0),
    _childProductionFactor(0),
    _phyloName("XXX"),

    _age(0),
    _deathMonth(0),
    _birthMonth(0),
    _reproductionProgress(0),
    _hunger(0),
    _markedForRivalDeath(false),
    _kidCount(0)

{
}

Herbivore::Herbivore(
    double inMaxAge,
    double inMaxHunger,
    double inDefenseFactor,
    double inQualityOfSense,
    double inFavoriteTemperature,
    double inTemperatureSpan,
    double inChildProductionFactor,
    std::string inPhyloName) :
    _maxAge(inMaxAge),
    _maxHunger(inMaxHunger),
    _defenseFactor(inDefenseFactor),
    _qualityOfSense(inQualityOfSense),
    _favoriteTemperature(inFavoriteTemperature),
    _temperatureSpan(inTemperatureSpan),
    _childProductionFactor(inChildProductionFactor),
    _phyloName(inPhyloName),
    _location(Point<size_t>(0,0)),

    _age(0),
    _deathMonth(0),
    _birthMonth(0),
    _reproductionProgress(0),
    _hunger(0),
    _markedForRivalDeath(false),
    _kidCount(0)
{

}
Herbivore::Herbivore(Herbivore inParent,
    Point<size_t> inPosition,
    size_t inCurrentMonth,
    std::string inPhyloName):
    _maxAge(inParent.GetMaxAge()),
    _maxHunger(inParent.GetMaxHunger()),
    _defenseFactor(inParent.GetDefenseFactor()),
    _qualityOfSense(inParent.GetQualityOfSense()),
    _favoriteTemperature(inParent.GetFavoriteTemperature()),
    _temperatureSpan(inParent.GetTemperatureSpan()),
    _childProductionFactor(inParent.GetChildProductionFactor()),
    _location(inPosition),
    _phyloName(inPhyloName),

    _age(0),
    _deathMonth(0),
    _birthMonth(0),
    _reproductionProgress(0),
    _hunger(0),
    _markedForRivalDeath(false),
    _kidCount(0)
{

}

std::vector<Herbivore> Herbivore::GetChildList(int month)
{
    if (_minAgeForReproduction * _maxAge > _age)
    {
        //too young for offspring
        return std::vector<Herbivore>();
    }

    if (_maxAgeForReproduction * _maxAge < _age)
    {
        //too old for offspring
        return std::vector<Herbivore>();
    }

    if (_reproductionProgress < _percentRequiredForReproduction)
    {
        //didnt work/eat enough for offspring
        return std::vector<Herbivore>();
    }

    _reproductionProgress = 0;

    std::vector<Herbivore> tmpChildList = std::vector<Herbivore>();

    for (int i = 0; i < _childProductionFactor; i++)
    {
        double tmpKidMaxAge =
            _maxAge_MPS.EvtlMutate(_maxAge);
        double tmpKidMaxHunger =
            _maxHunger_MPS.EvtlMutate(_maxHunger);
        double tmpKidDefenseFactor =
            _defenseFactor_MPS.EvtlMutate(_defenseFactor);
        double tmpKidQualityOfSense =
            _qualityOfSense_MPS.EvtlMutate(_qualityOfSense);
        double tmpKidFavoriteTemperature =
            _favoriteTemperature_MPS.EvtlMutate(_favoriteTemperature);
        double tmpKidTemperatureSpan =
            _temperatureSpan_MPS.EvtlMutate(_temperatureSpan);
        double tmpKidChildProdFactor =
            _childProductionFactor_MPS.EvtlMutate(_childProductionFactor);
        
        Herbivore tmpKid = Herbivore(
            tmpKidMaxAge,
            tmpKidMaxHunger,
            tmpKidDefenseFactor,
            tmpKidQualityOfSense,
            tmpKidFavoriteTemperature,
            tmpKidTemperatureSpan,
            tmpKidChildProdFactor,
            (_phyloName+"-"+ std::to_string(i+1)));

        tmpChildList.push_back(tmpKid);
    }
    _kidCount += tmpChildList.size();
    return tmpChildList;
}


bool Herbivore::IsTemperatureInSpan(double inTemperature)
{
    double ft = _favoriteTemperature;
    double ts = _temperatureSpan;
    if (inTemperature > (ft + ts))
    {
        return false;
    }

    if (inTemperature < (ft - ts))
    {
        return false;
    }

    return true;
}


void Herbivore::CalculateReproductionProgress(double inCurrentTemperature)
{
    // at least :
    // + hp% if hunger==0; 0% if hunger==maxHunger
    // + tp% if perfect temperature
    double hp = _reproductionHungerPercent;
    double tp = _reproductionTemperaturePercent;

    // Will show how unhungry a herbivore is
    double tmpFullness = hp * (1 - _hunger / _maxHunger);

    double temperatureDeviation = abs(inCurrentTemperature - _favoriteTemperature);

    // Will show how happy a herbivore is about the temperature
    double tmpValTemp = tp * (1 - (temperatureDeviation / _temperatureSpan));


    double tmpSum = tmpFullness + tmpValTemp;
    double tmpProd = tmpFullness * tmpValTemp;
    _reproductionProgress += std::max(tmpSum, tmpProd);

}

bool Herbivore::CheckIfWillDie(double inCurrentTemperature)
{
    if (_age > _maxAge)
    {
        _deathReason = "age";
        return true;
    }

    if (inCurrentTemperature > _favoriteTemperature + _temperatureSpan)
    {
        _deathReason = "heat";
        return true;
    }

    if (inCurrentTemperature < _favoriteTemperature - _temperatureSpan)
    {
        _deathReason = "cold";
        return true;
    }

    if (_maxHunger<_hunger)
    {
        _deathReason = "hunger";
        return true;
    }

    return false;
}





void Herbivore::Tick(double inCurrentTemperature)
{
    _age += 1;
    _hunger += 1;
    CalculateReproductionProgress(inCurrentTemperature);
}

bool Herbivore::IsHungry()
{
    return(_eatAtHungerPercent < double(_hunger) / _maxHunger);
}


MutatingPropertyStatic& Herbivore::GetMaxAge_MPS()
{
    return _maxAge_MPS;
}

MutatingPropertyStatic& Herbivore::GetMaxHunger_MPS()
{
    return _maxHunger_MPS;
}

MutatingPropertyStatic& Herbivore::GetDefenseFactor_MPS()
{
    return _defenseFactor_MPS;
}

MutatingPropertyStatic& Herbivore::GetQualityOfSense_MPS()
{
    return _qualityOfSense_MPS;
}

MutatingPropertyStatic& Herbivore::GetFavoriteTemperature_MPS()
{
    return _favoriteTemperature_MPS;
}

MutatingPropertyStatic& Herbivore::GetTemperatureSpan_MPS()
{
    return _temperatureSpan_MPS;
}

MutatingPropertyStatic& Herbivore::GetChildProductionFactor_MPS()
{
    return _childProductionFactor_MPS;
}

double Herbivore::GetMaxAge()
{
    return _maxAge;
}

double Herbivore::GetMaxHunger()
{
    return _maxHunger;
}

double Herbivore::GetDefenseFactor()
{
    return _defenseFactor;
}

double Herbivore::GetQualityOfSense()
{
    return _qualityOfSense;
}

double Herbivore::GetFavoriteTemperature()
{
    return _favoriteTemperature;
}

double Herbivore::GetTemperatureSpan()
{
    return _temperatureSpan;
}

double Herbivore::GetChildProductionFactor()
{
    return _childProductionFactor;
}

void Herbivore::DeadDueRival(size_t inMonth)
{
    _deathReason = "Rivalery";
    _deathMonth = inMonth;
}

void Herbivore::SetReproductionInfluencePercents(double inHungerPercent, double inTemperaturePercent)
{
    _reproductionHungerPercent = inHungerPercent;
    _reproductionTemperaturePercent = inTemperaturePercent;
}

void Herbivore::SetNeededPercentForReproduction(double inRequiredProcentForReproduction)
{
    _percentRequiredForReproduction = inRequiredProcentForReproduction;
}

void Herbivore::SetReproductionAgeRestrictions(double inMinAgeForReproduction, double inMaxAgeForReproduction)
{
    _minAgeForReproduction = inMinAgeForReproduction;
    _maxAgeForReproduction = inMaxAgeForReproduction;
}


void Herbivore::SetEatAtHungerPercent(double inEatAtHungerPercent)
{
    _eatAtHungerPercent = inEatAtHungerPercent;
}

void Herbivore::SetDeathMonth(size_t inDeathMonth)
{
    _deathMonth = inDeathMonth;
}

void Herbivore::ChooseSite(
    std::vector<std::pair<Point<size_t>, bool>> inFoundLocationKandidates,
    std::vector<double> inTemperatureList) 
{

    // Jump to first not too cold food site
    for (size_t i = 0; i < inFoundLocationKandidates.size(); i++)
    {
        int h = inFoundLocationKandidates[i].first.GetY();
        if(inFoundLocationKandidates[i].second &&
            _inTempSpan(inTemperatureList[h]) )
        {
            _location = inFoundLocationKandidates[i].first;
            return;
        }
    }

    //No food found
    // Jump to first not too cold other site
    for (size_t i = 0; i < inFoundLocationKandidates.size(); i++)
    {
        int h = inFoundLocationKandidates[i].first.GetY();
        if(!inFoundLocationKandidates[i].second &&
            _inTempSpan(inTemperatureList[h]))
        {
            _location = inFoundLocationKandidates[i].first;
            return;
        }
    }

    // No location found at all, no position changing
}


std::string Herbivore::GetPhyloName()
{
    return _phyloName;
}

void Herbivore::Ate()
{
    _hunger = 0;
}

void Herbivore::SetMarkedForRivalDeath()
{
    _markedForRivalDeath = true;
}

bool Herbivore::IsMarkedForRivalDeath()
{
    return _markedForRivalDeath;
}

std::string Herbivore::PrintAsCsv()
{
    std::string ret="";
    ret += _phyloName + ",";
    ret += std::to_string(_age) + ",";
    ret += std::to_string(_maxAge) + ",";
    ret += std::to_string(_hunger) + ",";
    ret += std::to_string(_maxHunger) + ",";
    ret += std::to_string(_defenseFactor) + ",";
    ret += std::to_string(_qualityOfSense) + ",";
    ret += std::to_string(_favoriteTemperature) + ",";
    ret += std::to_string(_temperatureSpan) + ",";
    ret += std::to_string(_childProductionFactor) + ",";
    ret += std::to_string(_kidCount) + ",";
    ret += std::to_string(_birthMonth) + ",";
    ret += std::to_string(_deathMonth) + ",";
    ret += _deathReason;

    return ret;
}

std::string Herbivore::GetCSVHeader()
{
    return "Phylo Name,Age,Max Age,Hunger,Max Hunger,Defense Factor,Quality Of Sense,Favorite Temperature,Temperature Span,Child Production Factor,Kid Count,Birth Month,Death Month,Death Reason";
}


bool Herbivore::_inTempSpan(double inTemperature)
{
    if (inTemperature > _favoriteTemperature + _temperatureSpan)
    {
        return false;
    }


    if (inTemperature < _favoriteTemperature - _temperatureSpan)
    {
        return false;
    }


    return true;
}