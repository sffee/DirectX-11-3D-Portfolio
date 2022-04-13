#include "GJHFreeCamera.h"
#include <GJHGameEngineActor.h>
#include "GJHCamera.h"
#include <GJHGameEngineInput.h>
#include <GJHGameEngineWindow.h>
#include "GJHGUIComponent.h"

GJHFreeCamera::GJHFreeCamera() :
	m_Speed(300.0f),
	m_RotSpeed(90.0f),
	m_BoostRatio(30.0f),
	m_FreeCamMode(true)
{
}

GJHFreeCamera::~GJHFreeCamera()
{
}

void GJHFreeCamera::Start()
{
	std::shared_ptr<GJHCamera> Cam = GetActor()->FindComponent<GJHCamera>();

	if (Cam == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("캠이 존재하지 않는 곳에 프리카메라 기능을 추가하려고 했습니다.");
	}
}

void GJHFreeCamera::Update()
{
	return;

	if (m_FreeCamMode == false)
	{
		return;
	}

	if (GJHGameEngineInput::GetInst().Press(L"RClick") == false)
	{
		return;
	}

	float LocalSpeed = m_Speed;

	if (GJHGameEngineInput::GetInst().Press(L"LShift"))
	{
		LocalSpeed *= m_BoostRatio;
	}

	if (GJHGameEngineInput::GetInst().Press(L"W"))
	{
		GetActorTrans()->SetWorldMove(GetActorTrans()->GetWorldForward() * GJHGameEngineTime::FDeltaTime(LocalSpeed));
	}
	if (GJHGameEngineInput::GetInst().Press(L"S"))
	{
		GetActorTrans()->SetWorldMove(GetActorTrans()->GetWorldBack() * GJHGameEngineTime::FDeltaTime(LocalSpeed));
	}
	if (GJHGameEngineInput::GetInst().Press(L"D"))
	{
		GetActorTrans()->SetWorldMove(GetActorTrans()->GetWorldRight() * GJHGameEngineTime::FDeltaTime(LocalSpeed));
	}
	if (GJHGameEngineInput::GetInst().Press(L"A"))
	{
		GetActorTrans()->SetWorldMove(GetActorTrans()->GetWorldLeft() * GJHGameEngineTime::FDeltaTime(LocalSpeed));
	}
	if (GJHGameEngineInput::GetInst().Press(L"Q"))
	{
		GetActorTrans()->SetWorldMove(GetActorTrans()->GetWorldUp() * GJHGameEngineTime::FDeltaTime(LocalSpeed));
	}
	if (GJHGameEngineInput::GetInst().Press(L"E"))
	{
		GetActorTrans()->SetWorldMove(GetActorTrans()->GetWorldDown() * GJHGameEngineTime::FDeltaTime(LocalSpeed));
	}

	GetActorTrans()->AddWorldRotationY(GJHGameEngineInput::GetMouse3DDir().x * GJHGameEngineTime::FDeltaTime(m_RotSpeed));
	GetActorTrans()->AddWorldRotationX(-GJHGameEngineInput::GetMouse3DDir().y * GJHGameEngineTime::FDeltaTime(m_RotSpeed));
}