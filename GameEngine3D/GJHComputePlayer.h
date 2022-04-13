#pragma once
#include "GJHPlayerBase.h"
#include "GJHDirectComputeShader.h"

class GJHComputePlayer : public GJHPlayerBase
{
private:
public:
private:
	static GJHDirectComputeShader* gComputeShader;

private:
	std::shared_ptr<GJHDirectComputeShader> m_ComputeShader;

	int ThreadX;
	int ThreadY;
	int ThreadZ;

public:
	void SetComputeThreadSetting(int _X, int _Y, int _Z);

public:
	void SetComputeShader(const GJHGameEngineString& _Name, int _X = 0, int _Y = 0, int _Z = 0);

public:
	GJHComputePlayer();
	~GJHComputePlayer();

public:
	GJHComputePlayer(const GJHComputePlayer& _Other) = delete;
	GJHComputePlayer(const GJHComputePlayer&& _Other) = delete;

public:
	GJHComputePlayer& operator=(const GJHComputePlayer& _Other) = delete;
	GJHComputePlayer& operator=(const GJHComputePlayer&& _Other) = delete;

public:
	void Compute();
};

