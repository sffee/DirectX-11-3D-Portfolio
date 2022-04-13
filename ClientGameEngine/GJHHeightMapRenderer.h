#pragma once
#include "GJHRenderer.h"
#include <GJHVertexHeader.h>

struct MapData
{
	float4 MapCount;
	int IsSplatting[4];
};

class GJHHeightMapRenderer : public GJHRenderer
{
private:
	std::shared_ptr<GJHDirectMesh> m_HeightMesh;

	std::shared_ptr<GJHDirectTexture2D> HTex;

public:
	std::vector<VertexBase> VBVector;
	std::vector<float4> VBVectorPos;
	std::vector<UINT> IBVector;

public:
	MapData Data;
	void CreateMap(int _X, int _Z, float YRatio, const GJHGameEngineString& _HTexPath);

public:
	void CreateMap(int _X, int _Z);

public:
	GJHHeightMapRenderer();
	~GJHHeightMapRenderer();

public:
	GJHHeightMapRenderer(const GJHHeightMapRenderer& _Other) = delete;
	GJHHeightMapRenderer(const GJHHeightMapRenderer&& _Other) = delete;

public:
	GJHHeightMapRenderer& operator=(const GJHHeightMapRenderer& _Other) = delete;
	GJHHeightMapRenderer& operator=(const GJHHeightMapRenderer&& _Other) = delete;

public:
	void SetFloor(int _FloorCount, const GJHGameEngineString& _DiffuseTexture, const GJHGameEngineString& _SplettingTexture);
};

