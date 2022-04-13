#pragma once
#include "GJHContentsComponent.h"

class GJHActorTransform;
class GJHFbxAnimationRenderer;
class GJHBoneSlot : public GJHContentsComponent
{
private:
	std::shared_ptr<GJHFbxAnimationRenderer> m_AniRenderer;

	GJHGameEngineString TargetBoneName;
	std::shared_ptr<GJHActorTransform> m_TargetPtr;

public:
	void Setting(const GJHGameEngineString& _TargetBoneName, std::shared_ptr<GJHActorTransform> _TargetPtr);

public:
	bool IsBone(const GJHGameEngineString& _BoneName);

public:
	GJHBoneSlot();
	~GJHBoneSlot();

public:
	GJHBoneSlot(const GJHBoneSlot& _Other) = delete;
	GJHBoneSlot(const GJHBoneSlot&& _Other) = delete;

public:
	GJHBoneSlot& operator=(const GJHBoneSlot& _Other) = delete;
	GJHBoneSlot& operator=(const GJHBoneSlot&& _Other) = delete;

public:
	void Start() override;
	void Update() override;
	void RenderPrev() override;
};

