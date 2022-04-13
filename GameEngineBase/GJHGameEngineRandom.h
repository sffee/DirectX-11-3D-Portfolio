#pragma once
#include <random>

class GJHGameEngineRandom
{
private:
	static std::mt19937_64 gen;

public:
	static void SeedChange(__int64 _Seed);

public:
	static float RandomFloat(float _Min, float _Max);
	static int RandomInt(int _Min, int _Max);

public:
	GJHGameEngineRandom();
	~GJHGameEngineRandom();

public:
	GJHGameEngineRandom(const GJHGameEngineRandom& _Other) = delete;
	GJHGameEngineRandom(const GJHGameEngineRandom&& _Other) = delete;

public:
	GJHGameEngineRandom operator=(const GJHGameEngineRandom& _Other) = delete;
	GJHGameEngineRandom operator=(const GJHGameEngineRandom&& _Other) = delete;
};