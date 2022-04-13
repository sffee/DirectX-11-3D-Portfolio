#pragma once
#include <GJHGameEngineMath.h>

class GJHRenderingTransformData
{
public:
	union
	{
		struct
		{
			float4x4 Scale;
			float4x4 Rotation;
			float4x4 Position;
			float4x4 Revolve;
			float4x4 Parent;
			float4x4 LocalWorld;
			float4x4 World;
			float4x4 View;
			float4x4 Projection;
			float4x4 WorldView;
			float4x4 ViewProjection;
			float4x4 WorldViewProjection;
			float4   CamPos;
			float4   CamPos1;
			float4   CamPos2;
			float4   CamPos3;
		};
	};

	void CalWorld()
	{
		LocalWorld = Scale * Rotation * Position * Revolve;
		World = LocalWorld * Parent;
	}

	void CalRenderingMatrix()
	{
		WorldView = World * View;
		ViewProjection = View * Projection;

		WorldViewProjection = WorldView * Projection;
	}

	void Iden()
	{
		Position.Iden();
		Scale.Iden();
		Rotation.Iden();
		Revolve.Iden();
		Parent.Iden();
		LocalWorld.Iden();
		World.Iden();
		View.Iden();
		Projection.Iden();
		WorldView.Iden();
		ViewProjection.Iden();
		WorldViewProjection.Iden();
	}

	GJHRenderingTransformData& operator=(const GJHRenderingTransformData& _Other)
	{
		Scale = _Other.Scale;
		Rotation = _Other.Rotation;
		Position = _Other.Position;
		Revolve = _Other.Revolve;
		Parent = _Other.Parent;
		LocalWorld = _Other.LocalWorld;
		World = _Other.World;
		View = _Other.View;
		Projection = _Other.Projection;
		WorldView = _Other.WorldView;
		ViewProjection = _Other.ViewProjection;
		WorldViewProjection = _Other.WorldViewProjection;

		return *this;
	}

public:
	GJHRenderingTransformData() :
		Position(),
		Scale(),
		Rotation(),
		Revolve(),
		Parent(),
		LocalWorld(),
		World(),
		View(),
		Projection(),
		WorldView(),
		ViewProjection(),
		WorldViewProjection()
	{
		Iden();
	}

	~GJHRenderingTransformData()
	{

	}
};