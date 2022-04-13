#pragma once
#include <GJHGameEngineGUI.h>
#include <GJHGameEngineThread.h>
#include <set>

class FbxLoading : std::enable_shared_from_this<FbxLoading>
{
public:
	float Per;
	std::shared_ptr<GJHGameEngineThread> m_Thread;
	bool IsEnd;
};

class FbxAniLoading : std::enable_shared_from_this<FbxAniLoading>
{
public:
	float Per;
	std::shared_ptr<GJHGameEngineThread> m_Thread;
	bool IsEnd;
	GJHGameEngineString MeshFbxName;
	GJHGameEngineString AniFbxName;
};

class GJHGameEngineScene;
class FBXWindow : public GJHGUIWindow
{
public:
	static std::mutex LoadingLock;
	GJHGameEngineScene* CurScene;

private:
	int m_CurFbxSelect;
	int m_CurLoadFbxSelect;
	bool IsOverLapMessage;
	bool IsLoadingMessage;

	int m_CurMeshSelect;
	int m_CurAniSelect;

	std::map<GJHGameEngineString, std::shared_ptr<FbxLoading>> LoadingList;
	std::map<GJHGameEngineString, std::shared_ptr<FbxAniLoading>> LoadingAniList;

	std::vector<GJHGameEngineString> LoadingEndList;
	std::vector<GJHGameEngineString> LoadingAniEndList;
	std::vector<GJHGameEngineString> UserFbxList;

	GJHGameEngineString MeshFbxName;
	GJHGameEngineString AniFbxName;

	GJHGameEngineString UserMeshFbxName;
	GJHGameEngineString UserAniFbxName;

public:
	FBXWindow();
	~FBXWindow();

public:
	FBXWindow(const FBXWindow& _Other) = delete;
	FBXWindow(const FBXWindow&& _Other) = delete;

public:
	FBXWindow& operator=(const FBXWindow& _Other) = delete;
	FBXWindow& operator=(const FBXWindow&& _Other) = delete;

public:
	virtual void OnGUI() override;
	static void LoadingFbx(GJHGameEngineString _FileName, FbxLoading* _Ptr);
	static void LoadingAniFbx(GJHGameEngineString _FileName, FbxAniLoading* _Ptr);
};

