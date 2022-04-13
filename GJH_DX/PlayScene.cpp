#include "PlayScene.h"
#include <GJHGameEngineScene.h>
#include "FBXWindow.h"
#include "PlaySceneWindow.h"
#include <GJHCamera.h>
#include <GJHGameEngineNeviMesh.h>
#include <GJHLight.h>
#include <GJHDirectFbx.h>
#include <GJHFbxAnimationRenderer.h>
#include <GJHFbxRenderer.h>
#include <GJHGameEngineDirectory.h>
#include <GJHWorldComponent.h>
#include "Player.h"
#include "SmallRobot.h"
#include "ColTestActor.h"
#include "define.h"

PlayScene::PlayScene()
{

}

PlayScene::~PlayScene()
{

}

void PlayScene::Start()
{
	GetWorld()->SetCollisionLink((int)CollisionType::Player, (int)CollisionType::Monster);

	std::shared_ptr<FBXWindow> FindFbxWindow = GJHGameEngineGUI::FindGUIWindow<FBXWindow>(L"FBXWindow");

	if (FindFbxWindow != nullptr)
	{
		FindFbxWindow->CurScene = GetScene();
	}

	m_PlaySceneWindow = GJHGameEngineGUI::CreateGUIWindow<PlaySceneWindow>(L"PlaySceneWindow");
	
	CreateCam();
	CreateLight();
	CreateMap();
	CreatePlayer();
	//CreateMonster();

	{
		ActorPtr Actor = GetScene()->CreateActor();

		std::shared_ptr<GJHRenderer> Render = Actor->CreateComponent<GJHRenderer>();
		Render->CreateRenderPlayer(L"3DMeshColor", L"Box");
		Render->CBufferSettingNew("OutPutColor", float4::BLUE);
		Render->SetWorldScale({ 1000.f, 500.f, 300.f });

		std::shared_ptr<ColTestActor> TestActor = Actor->CreateComponent<ColTestActor>();
		TestActor->GetActorTrans()->SetWorldPosition({ -4800.f, 0.f, 8800.f });
		TestActor->SetCollisionScale({ 1000.f, 500.f, 300.f });
	}

	{
		ActorPtr Actor = GetScene()->CreateActor();

		std::shared_ptr<GJHRenderer> Render = Actor->CreateComponent<GJHRenderer>();
		Render->CreateRenderPlayer(L"3DMeshColor", L"Box");
		Render->CBufferSettingNew("OutPutColor", float4::BLUE);
		Render->SetWorldScale({ 1000.f, 500.f, 300.f });
		Render->SetWorldRotation({ 0.f, 45.f, 0.f });

		std::shared_ptr<ColTestActor> TestActor = Actor->CreateComponent<ColTestActor>();
		TestActor->GetActorTrans()->SetWorldPosition({ -3200.f, 0.f, 8800.f });
		TestActor->SetCollisionScale({ 1000.f, 500.f, 300.f });
		TestActor->SetCollisionRotation({ 0.f, 45.f, 0.f });
	}
}

void PlayScene::Update()
{
	std::shared_ptr<GJHActorTransform> FreeCamTrans = m_FreeCam->FindComponent<GJHActorTransform>();
	PlaySceneWindow::FreeCamPos = FreeCamTrans->GetWorldPosition();
	PlaySceneWindow::FreeCamRot = FreeCamTrans->GetWorldRotation();

	if (KEYDOWN("F2"))
	{
		m_CollisionDebugCam->ToggleOnOff();
	}
}

void PlayScene::CreateCam()
{
	std::shared_ptr<GJHCamera> Cam;
	{
		ActorPtr Actor = GetScene()->CreateActor();
		Cam = Actor->CreateComponent<GJHCamera>(0, OutPutPathType::Forward, OutPutPathType::Defferd, 0, 1, 2);
		m_FreeCam = Actor->CreateComponent<GJHFreeCamera>();
		m_FreeCam->GetActorTrans()->SetWorldPosition({ -4800.f, 135.f, 6290.f });
		m_FreeCam->GetActorTrans()->SetWorldRotation({ 13.f, 3.f, 0.f });
		m_FreeCam->FreeCamModeOn();

		Cam->SetCamProjectionSize({ CAMSIZEX, CAMSIZEY });
		Cam->SetCamProjectionMode(CamProjectionMode::Perspective);
	}

	{
		ActorPtr Actor = GetScene()->CreateActor();
		m_CollisionDebugCam = Actor->CreateComponent<GJHCamera>(0, OutPutPathType::Forward, 10);

		m_CollisionDebugCam->SetCamProjectionSize({ CAMSIZEX, CAMSIZEY });
		m_CollisionDebugCam->SetCamProjectionMode(CamProjectionMode::Perspective);

		m_CollisionDebugCam->GetActorTrans()->AttachParent(Cam->GetActorTrans());
	}
}

void PlayScene::CreateLight()
{
	{
		ActorPtr Actor = GetScene()->CreateActor();
		std::shared_ptr<GJHLight> pLight = Actor->CreateComponent<GJHLight>(0, 0);
		std::shared_ptr<GJHActorTransform> Ptr = Actor->FindComponent<GJHActorTransform>();
		Ptr->SetWorldPosition(float4(-4800.f, 3360.f, 7300.f));
		Ptr->AddWorldRotationX(90.0f);
	}

	{
		ActorPtr Actor = GetScene()->CreateActor();
		std::shared_ptr<GJHLight> pLight = Actor->CreateComponent<GJHLight>(0, 0);
		std::shared_ptr<GJHActorTransform> Ptr = Actor->FindComponent<GJHActorTransform>();
		Ptr->SetWorldPosition(float4(-4800.f, 3360.f, 8800.f));
		Ptr->AddWorldRotationX(90.0f);
	}
}

void PlayScene::CreateMap()
{
	{
		GJHGameEngineDirectory NewDir;
		NewDir.MoveParent(L"GJH");
		NewDir.Move(L"Resources");
		NewDir.Move(L"Mesh");
		NewDir.Move(L"Map");
		NewDir.Move(L"Stage1");

		GJHDirectFbx::LoadUserMesh(NewDir.PlusFilePath(L"Tower.USERMESH"));

		ActorPtr Actor = GetScene()->CreateActor();
		std::shared_ptr<GJHFbxRenderer> NewRenderer = Actor->CreateComponent<GJHFbxRenderer>();
		NewRenderer->SetMesh(L"Tower.USERMESH");
	}
}

void PlayScene::CreatePlayer()
{
	{
		GJHGameEngineDirectory NewDir;
		NewDir.MoveParent(L"GJH");
		NewDir.Move(L"Resources");
		NewDir.Move(L"Mesh");
		NewDir.Move(L"Player");

		GJHDirectFbx::LoadUserMesh(NewDir.PlusFilePath(L"Player.USERMESH"));

		NewDir.Move(L"Animation");
		auto Arr = NewDir.AllFile(L"USERANI");
		for (size_t i = 0; i < Arr.size(); i++)
		{
			GJHDirectFbx::LoadUserAnimation(Arr[i].FullPath());
		}

		ActorPtr Actor = GetScene()->CreateActor();
		Actor->CreateComponent<GJHFbxAnimationRenderer>();

		m_Player = Actor->CreateComponent<Player>();
		m_Player->PlayerInit(m_FreeCam->GetActorTrans(), { 0.0f, 200.0f, -500.0f }, { 0.0f, 0.0f, 0.0f });
		m_Player->GetActorTrans()->SetWorldPosition({ -4800.f, 0.f, 6600.f });
		//m_Player->GetActorTrans()->SetWorldScale({ 100.f, 100.f, 100.f });
	}
}

void PlayScene::CreateMonster()
{
	{
		GJHGameEngineDirectory NewDir;
		NewDir.MoveParent(L"GJH");
		NewDir.Move(L"Resources");
		NewDir.Move(L"Mesh");
		NewDir.Move(L"Monster");
		NewDir.Move(L"SmallRobot");

		GJHDirectFbx::LoadUserMesh(NewDir.PlusFilePath(L"SmallRobot.USERMESH"));

		NewDir.Move(L"Animation");

		ActorPtr Actor = GetScene()->CreateActor();
		std::shared_ptr<GJHFbxRenderer> NewRenderer = Actor->CreateComponent<GJHFbxRenderer>();
		NewRenderer->SetMesh(L"SmallRobot.USERMESH");

		std::shared_ptr<SmallRobot> Robot = Actor->CreateComponent<SmallRobot>();
		Robot->GetActorTrans()->SetWorldPosition({ -4800.f, 0.f, 8800.f });
	}
}