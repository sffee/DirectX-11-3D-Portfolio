#pragma once
#include "GJHRenderer.h"
#include <GJHDirectFbx.h>

class BoneFrameData
{
public:
	float4x4 AniMat;
	float4x4 AniRotMat;
	float4x4 AniScaleMat;
	float4x4 AniPosMat;
	float4x4 AniWorldMat;

	float4 Scale;
	float4 Pos;
	float4 Euler;
	float4 Q;
};

class BoneFrameData;
class GJHFbxAnimationRenderer;
class GJHFbxAnimation : std::enable_shared_from_this<GJHFbxAnimation>
{
public:
	GJHFbxAnimationRenderer* m_Parent;
	std::shared_ptr<GJHDirectFbx> m_MeshFbx;
	std::shared_ptr<GJHDirectFbx> m_AniFbx;
	FbxExAniData* m_AniData;
	int m_Start;
	int m_End;

	GJHGameEngineString m_Name;

	int m_CurFrame;
	float m_FrameTime;
	float m_CurFrameTime;
	bool m_Loop;

	bool m_AnimationEnd;

	std::vector<BoneFrameData> FrameBoneData;

	std::vector<float4x4> AnimationMatrix;

	std::vector<std::shared_ptr<GJHRenderPlayer>> AniRenderPlayer;

	std::shared_ptr<GJHDirectTexture2D> m_BoneTex;

public:
	void CalAnimationMatrix();
	void Reset();

public:
	GJHFbxAnimation();
};

class GJHFbxAnimationRenderer : public GJHRenderer
{
private:
	std::map<GJHGameEngineString, std::shared_ptr<GJHFbxAnimation>> AllAnimation;
	std::shared_ptr<GJHFbxAnimation> m_CurAnimation;

public:
	std::shared_ptr<GJHFbxAnimation> FindAnimation(const GJHGameEngineString& _Name);
	void ChangeAnimation(const GJHGameEngineString& _AniName);

public:
	std::shared_ptr<GJHFbxAnimation> GetCurrentAnimation();

public:
	void CreateAnimation(
		GJHGameEngineString _AniName,
		GJHGameEngineString _MeshFbx,
		GJHGameEngineString _AniFbx,
		GJHGameEngineString _MatName,
		int _AnimationIndex,
		bool _Loop,
		int _Start,
		int _End,
		float _FrameTime
	);

	void CreateAnimation(
		GJHGameEngineString _AniName,
		GJHGameEngineString _MeshFbx,
		GJHGameEngineString _AniFbx,
		GJHGameEngineString _MatName,
		int _AnimationIndex,
		bool _Loop,
		float _FrameTime
	);

	void CreateUserAnimation(
		GJHGameEngineString _AniName,
		GJHGameEngineString _MeshFbx,
		GJHGameEngineString _AniFbx,
		GJHGameEngineString _MatName,
		int _AnimationIndex,
		bool _Loop,
		float _FrameTime
	);

	void CreateUserAnimation(
		GJHGameEngineString _AniName,
		GJHGameEngineString _MeshFbx,
		GJHGameEngineString _AniFbx,
		GJHGameEngineString _MatName,
		int _AnimationIndex,
		bool _Loop,
		int _Start,
		int _End,
		float _FrameTime
	);

public:
	bool AnimationEnd();

public:
	void RenderingSetting() override;
	void Update() override;

public:
	GJHFbxAnimationRenderer();
	~GJHFbxAnimationRenderer();

public:
	GJHFbxAnimationRenderer(const GJHFbxAnimationRenderer& _Other) = delete;
	GJHFbxAnimationRenderer(const GJHFbxAnimationRenderer&& _Other) = delete;

public:
	GJHFbxAnimationRenderer& operator=(const GJHFbxAnimationRenderer& _Other) = delete;
	GJHFbxAnimationRenderer& operator=(const GJHFbxAnimationRenderer&& _Other) = delete;
};