#ifndef _EVOSIMRANDOM_H_
#define _EVOSIMRANDOM_H_

#include <random>
class EvoSimRandom
{

private:
	static std::mt19937 _gen;

public:
	static void SetSeed( unsigned int inSeed );
	static double GetEvenDistRandom(double inMin, double inMax);
	static double GetNormalDistRandom(double inCenter, double inStandardDeviation);

};

#endif
