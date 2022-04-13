#pragma once
#include "GJHRenderer.h"
#include <GJHDirectFbx.h>

class GJHFbxRenderer : public GJHRenderer
{
public:
	GJHFbxRenderer();
	~GJHFbxRenderer();

public:
	GJHFbxRenderer(const GJHFbxRenderer& _Other) = delete;
	GJHFbxRenderer(const GJHFbxRenderer&& _Other) = delete;

public:
	GJHFbxRenderer& operator=(const GJHFbxRenderer& _Other) = delete;
	GJHFbxRenderer& operator=(const GJHFbxRenderer&& _Other) = delete;

public:
	void SetMesh(const GJHGameEngineString& _MeshName);

public:
	void RenderingSetting() override;
};

