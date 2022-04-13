#include "FBXWindow.h"
#include <GJHGameEngineDirectory.h>
#include <GJHDirectFbx.h>
#include <GJHGameEngineFile.h>
#include <GJHGameEngineScene.h>
#include <GJHGameEngineActor.h>
#include <GJHActorTransform.h>
#include <GJHRenderer.h>
#include <GJHFbxAnimationRenderer.h>
#include <GJHGameEngineWindow.h>

std::mutex FBXWindow::LoadingLock;

FBXWindow::FBXWindow() :
	m_CurFbxSelect(-1),
	m_CurLoadFbxSelect(-1),
	m_CurMeshSelect(-1),
	m_CurAniSelect(-1)
{
	IsLoadingMessage = false;
	IsOverLapMessage = false;

	UserMeshFbxName = L"Monster3.FBX";
	UserAniFbxName = L"Monster3.FBX";
}

FBXWindow::~FBXWindow()
{
}

void FBXWindow::LoadingAniFbx(GJHGameEngineString _FileName, FbxAniLoading* _Ptr)
{
	std::shared_ptr<GJHDirectFbx> FindMeshFbx = GJHDirectFbx::FindAsync(_Ptr->MeshFbxName);
	std::shared_ptr<GJHDirectFbx> FindAniFbx = GJHDirectFbx::FindAsync(_Ptr->AniFbxName);

	FindAniFbx->CalFbxExBoneFrameTransMatrix(FindMeshFbx,
		[_Ptr](float _Value)
		{
			_Ptr->Per = _Value;
		}
	);


	GJHGameEngineString UserAniMeshName = _Ptr->AniFbxName;
	UserAniMeshName = GJHGameEnginePath::GetWithOutExtFileName(UserAniMeshName);
	UserAniMeshName += ".USERANI";

	GJHGameEngineDirectory NewDir;
	NewDir.MoveParent(L"GJH");
	NewDir.Move(L"Resources");
	NewDir.Move(L"Mesh");

	GJHGameEngineFile File = NewDir.FindFile(_FileName);

	FindAniFbx->SaveUserAni(File.Path() + UserAniMeshName);

	std::lock_guard L(FBXWindow::LoadingLock);
	_Ptr->IsEnd = true;

	GJHGameEngineWindow::Flash();
}

void FBXWindow::LoadingFbx(GJHGameEngineString _FileName, FbxLoading* _Ptr)
{
	if (_Ptr == nullptr)
	{
		return;
	}

	GJHGameEngineDirectory NewDir;
	NewDir.MoveParent(L"GJH");
	NewDir.Move(L"Resources");
	NewDir.Move(L"Mesh");

	GJHGameEngineFile File = NewDir.FindFile(_FileName);
	
	std::shared_ptr<GJHDirectFbx> Ptr = GJHDirectFbx::LoadAsync(File.FullPath(), [_Ptr](float _Value)
		{
			_Ptr->Per = _Value;
		}
	);

	GJHGameEngineString UserMeshName = _FileName;
	UserMeshName = GJHGameEnginePath::GetWithOutExtFileName(UserMeshName);
	UserMeshName += ".USERMESH";

	Ptr->SaveUserMesh(File.Path() + UserMeshName);

	std::lock_guard L(FBXWindow::LoadingLock);
	_Ptr->IsEnd = true;
}

void FBXWindow::OnGUI()
{
	GJHGameEngineDirectory FbxDir;
	FbxDir.MoveParent(L"GJH");
	FbxDir.Move(L"Resources");
	FbxDir.Move(L"Mesh");

	{
		UserFbxList.clear();
		std::vector<GJHGameEngineFile> GetFiles = FbxDir.AllFile(L"MESHFBX");

		for (auto File : GetFiles)
		{
			UserFbxList.push_back(File.FileName());
		}

		GetFiles.clear();
		GetFiles = FbxDir.AllFile(L"*.USERANI");

		for (auto File : GetFiles)
		{
			UserFbxList.push_back(File.FileName());
		}
	}

	std::vector<GJHGameEngineFile> GetFiles = FbxDir.AllFile(L"FBX");
	std::vector<GJHGameEngineString> GetFileNames;

	for (auto File : GetFiles)
	{
		GetFileNames.push_back(File.FileName());
	}

	GJHGameEngineGUI::TextList(L"FBX FILE LIST", m_CurFbxSelect, GetFileNames, 500.f, 10);
	ImGui::SameLine();
	GJHGameEngineGUI::TextList(L"USER MESH LIST", m_CurLoadFbxSelect, UserFbxList, 500.f, 10);

	if (m_CurFbxSelect != -1)
	{
		GJHGameEngineString Name = GetFileNames[m_CurFbxSelect];

		std::shared_ptr<GJHDirectFbx> Mesh = GJHDirectFbx::Find(GetFileNames[m_CurFbxSelect]);

		if (Mesh != nullptr)
		{
			if (Mesh->m_mapRenderSet.size() != 0
				&& Mesh->m_UserAniData.size() != 0)
			{
				MeshFbxName = GetFileNames[m_CurFbxSelect];
				AniFbxName = GetFileNames[m_CurFbxSelect];
			}
			else if (Mesh->m_mapRenderSet.size() != 0)
			{
				MeshFbxName = GetFileNames[m_CurFbxSelect];
			}
			else if (Mesh->m_UserAniData.size() != 0)
			{
				AniFbxName = GetFileNames[m_CurFbxSelect];
			}
		}
	}

	if (m_CurLoadFbxSelect != -1)
	{
		GJHGameEngineString Name = UserFbxList[m_CurLoadFbxSelect];

		if (Name.Find(L".USERANI") != GJHGameEngineString::FindError)
		{
			UserAniFbxName = Name;
		}
		else if (Name.Find(L".USERMESH") != GJHGameEngineString::FindError)
		{
			UserMeshFbxName = Name;
		}
	}

	if (IsOverLapMessage == true)
	{
		GJHGameEngineGUI::Text(L"이미 로딩중인 Fbx입니다.");
	}

	if (IsLoadingMessage == true)
	{
		GJHGameEngineGUI::Text(L"로딩이 끝난 Fbx입니다.");
	}

	if (m_CurFbxSelect != -1)
	{
		std::lock_guard L(FBXWindow::LoadingLock);

		if (GJHDirectFbx::Find(GetFileNames[m_CurFbxSelect]) != nullptr)
		{
			GJHGameEngineGUI::Text(L"로딩 완료 메쉬");
		}
		else if (GJHGameEngineGUI::Button(L"Loading"))
		{
			if (LoadingList.find(GetFileNames[m_CurFbxSelect]) != LoadingList.end())
			{
				IsOverLapMessage = true;
			}
			else if (GJHDirectFbx::FindAsync(GetFileNames[m_CurFbxSelect]) == nullptr)
			{
				IsLoadingMessage = false;
				IsOverLapMessage = false;
				std::shared_ptr<FbxLoading> NewThread = std::make_shared<FbxLoading>();
				NewThread->m_Thread = std::make_shared<GJHGameEngineThread>();
				NewThread->m_Thread->SetName(GetFileNames[m_CurFbxSelect]);
				NewThread->m_Thread->Start(&FBXWindow::LoadingFbx, GetFileNames[m_CurFbxSelect], NewThread.get());
				LoadingList.insert(std::map<GJHGameEngineString, std::shared_ptr<FbxLoading>>::value_type(GetFileNames[m_CurFbxSelect], NewThread));
			}
			else
			{
				IsLoadingMessage = true;
			}
		}
	}

	GJHGameEngineGUI::InputText(L"Mesh", MeshFbxName);
	GJHGameEngineGUI::SameLine();
	GJHGameEngineGUI::InputText(L"Ani", AniFbxName);
	GJHGameEngineGUI::SameLine();

	if (GJHGameEngineGUI::Button(L"UserAniCreate"))
	{
		std::shared_ptr<FbxAniLoading> NewThread = std::make_shared<FbxAniLoading>();
		NewThread->m_Thread = std::make_shared<GJHGameEngineThread>();
		NewThread->MeshFbxName = MeshFbxName;
		NewThread->AniFbxName = AniFbxName;

		if (GJHDirectFbx::FindAsync(AniFbxName) == nullptr)
		{
			GJHGameEngineDirectory NewDir;
			NewDir.MoveParent(L"GJH");
			NewDir.Move(L"Resources");
			NewDir.Move(L"Mesh");

			GJHGameEngineFile File = NewDir.FindFile(AniFbxName);
			GJHDirectFbx::LoadAsync(File.Path() + AniFbxName);
		}

		NewThread->m_Thread->SetName(AniFbxName);
		NewThread->m_Thread->Start(&FBXWindow::LoadingAniFbx, AniFbxName, NewThread.get());
		LoadingAniList.insert(std::map<GJHGameEngineString, std::shared_ptr<FbxAniLoading>>::value_type(AniFbxName, NewThread));
	}

	GJHGameEngineGUI::InputText(L"UserMesh", UserMeshFbxName);
	GJHGameEngineGUI::SameLine();
	GJHGameEngineGUI::InputText(L"UserAni", UserAniFbxName);
	GJHGameEngineGUI::SameLine();

	if (GJHGameEngineGUI::Button(L"UserAniCreateObject"))
	{
		{
			GJHGameEngineFile File = FbxDir.FindFile(UserMeshFbxName);
			GJHDirectFbx::LoadUserMesh(File.FullPath());
		}

		{
			GJHGameEngineFile File = FbxDir.FindFile(UserAniFbxName);
			GJHDirectFbx::LoadUserAnimation(File.FullPath());
		}

		ActorPtr Actor = CurScene->CreateActor();
		std::shared_ptr<GJHActorTransform> Ptr = Actor->FindComponent<GJHActorTransform>();
		Ptr->SetWorldPosition(float4(0.f, 0.f, 20.f));
		Ptr->SetWorldScale(float4({ 0.1f, 0.1f, 0.1f }));
		Ptr->SetWorldScale(float4({ 0.1f, 0.1f, 0.1f }));

		std::shared_ptr<GJHFbxAnimationRenderer> NewRenderer = Actor->CreateComponent<GJHFbxAnimationRenderer>(0);
		NewRenderer->CreateUserAnimation(L"All", UserMeshFbxName, UserAniFbxName, L"DefferdFbxAniShader3D", 0, true, 1.f / 40.f);
		NewRenderer->ChangeAnimation(L"All");
	}

	{
		std::lock_guard L(FBXWindow::LoadingLock);

		for (auto& Thread : LoadingList)
		{
			GJHGameEngineGUI::Text(Thread.second->m_Thread->GetName());
			ImGui::SameLine();
			GJHGameEngineGUI::ProgressBar(Thread.second->Per * 0.01f);
		}

		auto Start = LoadingList.begin();
		auto End = LoadingList.end();
		for (; End != Start;)
		{
			if (Start->second->IsEnd == true)
			{
				LoadingEndList.push_back(Start->second->m_Thread->GetName());
				Start = LoadingList.erase(Start);
			}
			else
			{
				++Start;
			}
		}
	}

	{
		std::lock_guard L(FBXWindow::LoadingLock);

		for (auto& Thread : LoadingAniList)
		{
			GJHGameEngineGUI::Text(Thread.second->m_Thread->GetName());
			ImGui::SameLine();
			GJHGameEngineGUI::ProgressBar(Thread.second->Per * 0.01f);
		}

		auto Start = LoadingAniList.begin();
		auto End = LoadingAniList.end();
		for (; End != Start;)
		{
			if (Start->second->IsEnd == true)
			{
				LoadingAniEndList.push_back(Start->second->m_Thread->GetName());
				Start = LoadingAniList.erase(Start);
			}
			else
			{
				++Start;
			}
		}
	}
}