
#include "EvoSimRandom.h"

std::mt19937 EvoSimRandom::_gen = std::mt19937(0);

void EvoSimRandom::SetSeed(unsigned int inSeed)
{
	_gen= std::mt19937(inSeed);
}


double EvoSimRandom::GetEvenDistRandom(double inMin, double inMax)
{
	std::uniform_real_distribution<> distribution(inMin, inMax);
	return distribution(_gen);
}


double EvoSimRandom::GetNormalDistRandom(double inCenter, double inStandardDeviation)
{
	if(inStandardDeviation<=0.1)
	{ 
		inStandardDeviation = 0.1;
	}
	std::normal_distribution<double> distribution(inCenter, inStandardDeviation);
	return distribution(_gen);
}