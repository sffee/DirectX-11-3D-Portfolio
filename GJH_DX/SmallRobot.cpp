#include "SmallRobot.h"
#include <GJHFbxRenderer.h>
#include "define.h"

SmallRobot::SmallRobot() :
	m_MoveSpeed(200.f),
	m_RotSpeed(0.2f)
{

}

SmallRobot::~SmallRobot()
{

}

void SmallRobot::Start()
{
	m_Render = FindComponent<GJHFbxRenderer>();
}

void SmallRobot::Update()
{
	TrackingPlayer();
}

void SmallRobot::TrackingPlayer()
{
	float4 MeshForward = m_Render->GetWorldForward();
	float RenderAngle = m_Render->GetWorldRotation().y;

	float4 LookForward = (PlayerTrans->GetWorldPosition() - GetActorTrans()->GetWorldPosition()).GetNormalize();

	float MoveRot = RenderAngle - float4::CalVectorAngleDegY360(MeshForward, LookForward);

	float Angle = float4::Lerp(m_Render->GetWorldRotation().y, MoveRot, m_RotSpeed);
	if (isnan(Angle) == false)
	{
		m_Render->SetLocalRotationY(Angle);
	}

	GetActorTrans()->SetLocalMove(LookForward * DELTATIME(m_MoveSpeed));
}