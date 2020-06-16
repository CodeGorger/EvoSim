#ifndef _MUTATINGPROPERTYSTATIC_H_
#define _MUTATINGPROPERTYSTATIC_H_

class MutatingPropertyStatic
{
public:
	MutatingPropertyStatic();
	
	void SetMutationStep(double inMutationStep);
	void SetMutationMax(double inMutationMax);
	void SetMutationMin(double inMutationMin);
	void SetMutationRate(double inMutationRate);

	double EvtlMutate(double inCurrentValue);
private:

	double _mutationStep;
	double _mutationMax;
	double _mutationMin;
	double _mutationRate;
};


#endif
