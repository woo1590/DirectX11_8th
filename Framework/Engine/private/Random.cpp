#include "EnginePCH.h"
#include "Random.h"

Random::Random():rng(std::random_device{}())
{
}

Random* Random::Create()
{
	Random* Instance = new Random;

	return Instance;
}

