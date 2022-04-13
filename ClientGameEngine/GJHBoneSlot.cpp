#include "GJHBoneSlot.h"
#include <GJHGameEngineActor.h>
#include "GJHFbxAnimationRenderer.h"
#include "GJHActorTransform.h"

GJHBoneSlot::GJHBoneSlot()
	: m_TargetPtr(nullptr)
{
}

GJHBoneSlot::~GJHBoneSlot()
{
}

void GJHBoneSlot::Start()
{
	m_AniRenderer = GetActor()->FindComponent<GJHFbxAnimationRenderer>();

	if (m_AniRenderer == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (m_AniRenderer == nullptr)");
	}
}

void GJHBoneSlot::Update()
{
	
}

void GJHBoneSlot::RenderPrev()
{
	if (m_TargetPtr == nullptr)
	{
		return;
	}

	std::shared_ptr<GJHFbxAnimation> Ani = m_AniRenderer->GetCurrentAnimation();
	Bone* BonePtr = Ani->m_MeshFbx->FindBone(TargetBoneName);

	float4x4 AniRendererMatrix = m_AniRenderer->GetRenderingData().World;

	float4x4 ScaleMat;
	ScaleMat.Scale({ 100.0f, 100.0f, 100.0f });

	float4x4 AniScale = Ani->FrameBoneData[BonePtr->Index].AniScaleMat;
	float4x4 AniRot = Ani->FrameBoneData[BonePtr->Index].AniRotMat;
	float4x4 AniPos = Ani->FrameBoneData[BonePtr->Index].AniPosMat;

	m_TargetPtr->GetTransformData().World = ScaleMat * AniRot * AniPos * AniRendererMatrix;
}

void GJHBoneSlot::Setting(const GJHGameEngineString& _TargetBoneName, std::shared_ptr<GJHActorTransform> _TargetPtr)
{
	TargetBoneName = _TargetBoneName;
	m_TargetPtr = _TargetPtr;
	m_TargetPtr->CalMatrixOff();
	// m_TargetPtr->DebugOn();
}

bool GJHBoneSlot::IsBone(const GJHGameEngineString& _BoneName)
{
	std::shared_ptr<GJHFbxAnimation> Ani = m_AniRenderer->GetCurrentAnimation();
	Bone* BonePtr = Ani->m_MeshFbx->FindBone(_BoneName);
	
	if (BonePtr == nullptr)
	{
		return false;
	}

	return true;
}