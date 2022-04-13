#include "GJHContentsSceneComponent.h"
#include <GJHGameEngineScene.h>
#include "GJHActorTransform.h"
#include "GJHSpriteRenderer.h"
#include "GJHSpriteAnimation.h"
#include "GJHWorldComponent.h"
#include "GJHRenderManagerComponent.h"
#include "GJHGUIComponent.h"
#include <GJHDirectTexture2D.h>

float GJHContentsSceneComponent::m_StageTime = 0.f;
float4 GJHContentsSceneComponent::m_CamSize;

GJHContentsSceneComponent::GJHContentsSceneComponent()
{

}

GJHContentsSceneComponent::~GJHContentsSceneComponent()
{

}

void GJHContentsSceneComponent::Begin()
{
	World = GetScene()->FindComponent<GJHWorldComponent>(L"World");
	GUI = GetScene()->FindComponent<GJHGUIComponent>(L"GUI");
}