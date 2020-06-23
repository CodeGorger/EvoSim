#ifndef _EVOSIMRANDOM_H_
#define _EVOSIMRANDOM_H_


#include <random>


// A class to handle all randomness of the evolution simulation
class EvoSimRandom
{

private:
	static std::mt19937 _gen;

public:
	// Will either set the seed
	// Or if inSeed==0 will take the time
	static void SetSeed( unsigned int inSeed );

	// Will return a double between inMin and inMax, Even distribution
	static double GetEvenDistRandom(double inMin, double inMax);

	// Will return a value around inCenter, with standard deviation; Normal distribution
	static double GetNormalDistRandom(double inCenter, double inStandardDeviation);

};

#endif
