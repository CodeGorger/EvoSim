#include "MutatingPropertyStatic.h"
#include "EvoSimRandom.h"


MutatingPropertyStatic::MutatingPropertyStatic():
    _mutationStep(1),
    _mutationMax(10),
    _mutationMin(1),
    _mutationRate(0.5)
{

}

void MutatingPropertyStatic::SetMutationStep(double inMutationStep)
{
    _mutationStep = inMutationStep;
}

void MutatingPropertyStatic::SetMutationMax(double inMutationMax)
{
    _mutationMax = inMutationMax;
}

void MutatingPropertyStatic::SetMutationMin(double inMutationMin)
{
    _mutationMin = inMutationMin;
}

void MutatingPropertyStatic::SetMutationRate(double inMutationRate)
{
    _mutationRate = inMutationRate;
}


double MutatingPropertyStatic::EvtlMutate(double inCurrentValue)
{
    
    if ((EvoSimRandom::GetEvenDistRandom(0, 1000) / 1000) > _mutationRate)
    {
        // Randomness didn't roll low enough, don't mutate
        return inCurrentValue;
    }
    // else Randomness rolled low enough, mutate

    if (EvoSimRandom::GetEvenDistRandom(0, 1000) <= 499)
    {
        // Mutate up
        if ((inCurrentValue + _mutationStep) > _mutationMax)
        {
            // But only mutate up, if the max is not exceeded
            inCurrentValue += _mutationStep;
        }
    }
    else
    {
        // Mutate down
        if ((inCurrentValue - _mutationStep) > _mutationMin)
        {
            // But only mutate down, if the min is not exceeded
            inCurrentValue -= _mutationStep;
        }
    }
    return inCurrentValue;
}