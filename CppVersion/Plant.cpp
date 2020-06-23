#include "Plant.h"
#include <algorithm>


MutatingPropertyStatic Plant::_maxWaterDistance_MPS = MutatingPropertyStatic();
MutatingPropertyStatic Plant::_seedProdFactor_MPS = MutatingPropertyStatic();
MutatingPropertyStatic Plant::_seedLightness_MPS = MutatingPropertyStatic();
MutatingPropertyStatic Plant::_maxAge_MPS = MutatingPropertyStatic();
MutatingPropertyStatic Plant::_foodChunks_MPS = MutatingPropertyStatic();
MutatingPropertyStatic Plant::_defenseFactor_MPS = MutatingPropertyStatic();
MutatingPropertyStatic Plant::_favoriteTemperature_MPS = MutatingPropertyStatic();
MutatingPropertyStatic Plant::_temperatureSpan_MPS = MutatingPropertyStatic();

double Plant::_percentRequiredForReproduction=100;
double Plant::_reproductionWaterDistancePercent = 5;
double Plant::_reproductionTemperaturePercent = 5;


Plant::Plant() :
    _position(Point<size_t>(0, 0)),

    _defenseFactor(0),
    _deathMonth(0),
    _birthMonth(0),
    _favoriteTemperature(0),
    _foodChunks(0),
    _foodChunksRemaining(0),
    _age(0),
    _isWaterDistance(0),
    _kidCount(0),
    _maxAge(0),
    _maxWaterDistance(0),
    _seedLightness(0),
    _seedProdFactor(0),
    _reproductionProgress(0),
    _temperatureSpan(0)

{

}


Plant::Plant(
    double inMaxWaterDistance,
    double inSeedProdFactor,
    double inSeedLightness,
    double inKidMaxAge,
    double inFoodChunks,
    double inDefenseFactor,
    double inFavoriteTemperature,
    double inTemperatureSpan,
    std::string inPhyloName,
    size_t inBirthMonth):
    _maxWaterDistance(inMaxWaterDistance),
    _seedProdFactor(inSeedProdFactor),
    _seedLightness(inSeedLightness),
    _maxAge(inKidMaxAge),
    _foodChunks(inFoodChunks),
    _defenseFactor(inDefenseFactor),
    _favoriteTemperature(inFavoriteTemperature),
    _temperatureSpan(inTemperatureSpan),
    _phyloName(inPhyloName),
    _position(Point<size_t>(0,0)),

    _isWaterDistance(0),
    _deathMonth(0),
    _birthMonth(inBirthMonth),
    _reproductionProgress(0),
    _foodChunksRemaining(inFoodChunks),
    _kidCount(0),
    _age(0)
{

}

Plant::Plant(Plant inParent,
    Point<size_t> inPosition,
    size_t inCurrentMonth,
    std::string inPhyloName):
    _maxWaterDistance(inParent.GetMaxWaterDistance()),
    _seedProdFactor(inParent.GetSeedProdFactor()),
    _seedLightness(inParent.GetSeedLightness()),
    _maxAge(inParent.GetMaxAge()),
    _foodChunks(inParent.GetFoodChunks()),
    _defenseFactor(inParent.GetDefenseFactor()),
    _favoriteTemperature(inParent.GetFavoriteTemperature()),
    _temperatureSpan(inParent.GetTemperatureSpan()),
    _position(inPosition),
    _phyloName(inPhyloName),
    _foodChunksRemaining(inParent.GetFoodChunks()),
    _kidCount(0),
    _age(0),
    _deathMonth(0),
    _birthMonth(0),
    _reproductionProgress(0),
    _isWaterDistance(0)
{
}

void Plant::SetPosition(Point<size_t> inLocation)
{
    _position = inLocation;
}

Point<size_t> Plant::GetPosition()
{
    return _position;
}




std::vector< std::shared_ptr<Plant> > Plant::CreateChildList(size_t inCurrentMonth)
{
    std::vector< std::shared_ptr<Plant> > tmpChildList;
    tmpChildList = std::vector< std::shared_ptr<Plant> >();

    // If the plant has not yet worked enough for kids
    if (_reproductionProgress < _percentRequiredForReproduction)
    {
        // Empty list
        return tmpChildList;
    }
    // else ... the plant has worked enough

    // reset the _reproductionProgress counter
    _reproductionProgress -= _percentRequiredForReproduction;

    int tmpKidsCount = (int)_seedProdFactor;

    for (int i = 0; i < tmpKidsCount; i++)
    {
        double tmpKidMaxWaterDistance =
            _maxWaterDistance_MPS.EvtlMutate(_maxWaterDistance);
        double tmpKidSeedProdFactor =
            _seedProdFactor_MPS.EvtlMutate(_seedProdFactor);
        double tmpKidSeedLightness =
            _seedLightness_MPS.EvtlMutate(_seedLightness);
        double tmpKidMaxAge =
            _maxAge_MPS.EvtlMutate(_maxAge);
        double tmpKidFoodChunks =
            _foodChunks_MPS.EvtlMutate(_foodChunks);
        double tmpKidDefenseFactor =
            _defenseFactor_MPS.EvtlMutate(_defenseFactor);
        double tmpKidFavoriteTemperature =
            _favoriteTemperature_MPS.EvtlMutate(_favoriteTemperature);
        double tmpKidTemperatureSpan =
            _temperatureSpan_MPS.EvtlMutate(_temperatureSpan);

        std::shared_ptr<Plant> tmpKid = 
            std::shared_ptr<Plant>( new Plant(
            tmpKidMaxWaterDistance,
            tmpKidSeedProdFactor,
            tmpKidSeedLightness,
            tmpKidMaxAge,
            tmpKidFoodChunks,
            tmpKidDefenseFactor,
            tmpKidFavoriteTemperature,
            tmpKidTemperatureSpan,
            _phyloName+"-"+std::to_string(i+1),
            inCurrentMonth) );

        tmpChildList.push_back(tmpKid);
    }
    _kidCount += tmpChildList.size();
    return tmpChildList;
}


MutatingPropertyStatic& Plant::GetMaxWaterDistance_MPS()
{
    return _maxWaterDistance_MPS;
}

MutatingPropertyStatic& Plant::GetSeedProdFactor_MPS()
{
    return _seedProdFactor_MPS;
}


MutatingPropertyStatic& Plant::GetSeedLightness_MPS()
{
    return _seedLightness_MPS;
}

MutatingPropertyStatic& Plant::GetMaxAge_MPS()
{
    return _maxAge_MPS;
}


MutatingPropertyStatic& Plant::GetFoodChunks_MPS()
{
    return _foodChunks_MPS;
}


MutatingPropertyStatic& Plant::GetDefenseFactor_MPS()
{
    return _defenseFactor_MPS;
}


MutatingPropertyStatic& Plant::GetFavoriteTemperature_MPS()
{
    return _favoriteTemperature_MPS;
}


MutatingPropertyStatic& Plant::GetTemperatureSpan_MPS()
{
    return _temperatureSpan_MPS;
}



double Plant::GetMaxWaterDistance()
{
    return _maxWaterDistance;
}
double Plant::GetSeedProdFactor()
{
    return _seedProdFactor;
}
double Plant::GetSeedLightness()
{
    return _seedLightness;
}
double Plant::GetMaxAge()
{
    return _maxAge;
}
double Plant::GetFoodChunks()
{
    return _foodChunks;
}
double Plant::GetDefenseFactor()
{
    return _defenseFactor;
}
double Plant::GetFavoriteTemperature()
{
    return _favoriteTemperature;
}

double Plant::GetTemperatureSpan()
{
    return _temperatureSpan;
}

void Plant::SetReproductionInfluencePercents(double inWaterDistancePercent, double inTemperaturePercent)
{
    _reproductionWaterDistancePercent = inWaterDistancePercent;
    _reproductionTemperaturePercent = inTemperaturePercent;
}

void Plant::SetNeededPercentForReproduction(double inRequiredProcent)
{
    _percentRequiredForReproduction = inRequiredProcent;
}

void Plant::SetDeathMonth(size_t inDeathMonth)
{
    _deathMonth = inDeathMonth;
}

void Plant::Eaten()
{
    _foodChunksRemaining--;
}

void Plant::Tick(double inCurrentTemperature)
{
    _age++;
    _calcReproductionProgress(inCurrentTemperature);
}


std::string Plant::GetPhyloName()
{
    return _phyloName;
}

bool Plant::CheckIfWillDie(double inCurrentTemperature)
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

    if (_maxWaterDistance < _isWaterDistance)
    {
        _deathReason = "aridification";
        return true;
    }

    if (_foodChunksRemaining <= 0)
    {
        _deathReason = "eaten";
        return true;
    }


    return false;
}

void Plant::SetWaterDistance(double inWaterDistance)
{
    _isWaterDistance = inWaterDistance;
}


std::string Plant::GetCSVHeader()
{
    return "Phylo Name,Age,Max Age,Is Water Distance,Max Water Distance,Defense Factor,Food Chunks,Favorite Temperature,Temperature Span,Seed Lightness,Seed Prod Factor,Kid Count,Birth Month,Death Month,Death Reason";
}

std::string Plant::PrintAsCsv()
{
    std::string ret = "";
    ret += _phyloName + ",";
    ret += std::to_string(_age) + ",";
    ret += std::to_string(_maxAge) + ",";
    ret += std::to_string(_isWaterDistance) + ",";
    ret += std::to_string(_maxWaterDistance) + ",";
    ret += std::to_string(_defenseFactor) + ",";
    ret += std::to_string(_foodChunks) + ",";
    ret += std::to_string(_favoriteTemperature) + ",";
    ret += std::to_string(_temperatureSpan) + ",";
    ret += std::to_string(_seedLightness) + ",";
    ret += std::to_string(_seedProdFactor) + ",";
    ret += std::to_string(_kidCount) + ",";
    ret += std::to_string(_birthMonth) + ",";
    ret += std::to_string(_deathMonth) + ",";
    ret += _deathReason;

    return ret;
}

void Plant::_calcReproductionProgress(double inCurrentTemperature)
{
    // at least :
    // + wp% if water is distance 1 +0% if water is at maxWaterDistance
    // + tp% if perfect temperature
    double wp = _reproductionWaterDistancePercent;
    double tp = _reproductionTemperaturePercent;
    if (_maxWaterDistance == 1)
    {
        _maxWaterDistance += 0.1;
    }
    double wnom = (_isWaterDistance - 1);
    double wdenom = (_maxWaterDistance - 1);
    double tmpValWater = wp * (1 - wnom/ wdenom);

    double temperatureDeviation = abs(inCurrentTemperature - _favoriteTemperature);
    double tfactor = (temperatureDeviation / _temperatureSpan);
    double tmpValTemp = tp * (1 - tfactor);

    double tmpSum = tmpValWater + tmpValTemp;
    double tmpProd = tmpValWater * tmpValTemp;
    _reproductionProgress += std::max(tmpSum, tmpProd);
}


