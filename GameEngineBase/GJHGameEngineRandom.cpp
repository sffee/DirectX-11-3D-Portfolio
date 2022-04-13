#include "BasePre.h"
#include "GJHGameEngineRandom.h"

std::mt19937_64 GJHGameEngineRandom::gen;

void GJHGameEngineRandom::SeedChange(__int64 _Seed)
{
	gen = std::mt19937_64(_Seed);
}

float GJHGameEngineRandom::RandomFloat(float _Min, float _Max)
{
	std::uniform_real_distribution<float> uniformDist(_Min, _Max);
	return uniformDist(gen);
}

int GJHGameEngineRandom::RandomInt(int _Min, int _Max)
{
	std::uniform_int_distribution<int> uniformDist(_Min, _Max);
	return uniformDist(gen);
}