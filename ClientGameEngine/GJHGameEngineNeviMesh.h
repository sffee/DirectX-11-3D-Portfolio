#pragma once
#include <GJHGameEngineMath.h>
#include <vector>
#include <list>
#include <GJHGameEngineComponent.h>
#include <mutex>
#include "GJHContentsComponent.h"

class NeviTri : std::enable_shared_from_this<NeviTri>
{
public:
	float4 TriArr[3];
	bool IsMoving;
	int Index;

	std::mutex OtherLock;

	std::vector<std::weak_ptr<NeviTri>> m_NeviListPtr;

public:
	void PushLink(std::shared_ptr<NeviTri> _Nevi)
	{
		OtherLock.lock();
		m_NeviListPtr.push_back(_Nevi);
		OtherLock.unlock();
	}

public:
	bool MoveCheck(const float4& _StartPostion, const float4& _MoveScale, const float4& _RayDir, const float4x4& _World);
	float CheckY(const float4& _Position, const float4& _RayDir, const float4x4& _World);
};

class GJHGameEngineNeviMesh;
class NeviWorker : std::enable_shared_from_this<NeviWorker>
{
private:
	friend GJHGameEngineNeviMesh;

private:
	GJHGameEngineNeviMesh* m_Parent;
	std::shared_ptr<NeviTri>            m_CurTri;
	std::shared_ptr<GJHActorTransform>     m_Target;
	float4                              RayDir;
	float4                              RayDirReverse;

private:
	void Init(std::shared_ptr<GJHActorTransform> _Target, float4 _RayDir);

public:
	bool MoveCheck(float4 _MoveScale);
	float CheckY();
	float CheckY(float4 _Pos);

public:
	NeviWorker(GJHGameEngineNeviMesh* _Parent) :
		m_Parent(_Parent),
		m_CurTri(nullptr),
		m_Target(nullptr),
		RayDir(),
		RayDirReverse()
	{
	}
};

class GJHActorTransform;
class GJHGameEngineNeviMesh : public GJHContentsComponent
{
private:
	static std::mutex CurThreadLock;
	static std::vector<std::shared_ptr<NeviTri>>::iterator CurThreadIter;
	static std::vector<std::shared_ptr<NeviTri>>::iterator CurEndIter;

	std::vector<std::shared_ptr<NeviTri>> m_ArrTri;

public:
	std::shared_ptr<NeviTri>  FindTri(std::shared_ptr<GJHActorTransform> _Target, float4 _RayDir);
	std::shared_ptr<NeviWorker>  GetWorker(std::shared_ptr<GJHActorTransform> _Target, float4 _RayDir);

public:
	void Create(const GJHGameEngineString& _Path, std::vector<float4> _PosList, std::vector<unsigned int> _IdxList);

	static void CalNeviThread(int Count);

public:
	GJHGameEngineNeviMesh();
	~GJHGameEngineNeviMesh();

public:
	GJHGameEngineNeviMesh(const GJHGameEngineNeviMesh& _Other) = delete;
	GJHGameEngineNeviMesh(const GJHGameEngineNeviMesh&& _Other) = delete;

public:
	GJHGameEngineNeviMesh& operator=(const GJHGameEngineNeviMesh& _Other) = delete;
	GJHGameEngineNeviMesh& operator=(const GJHGameEngineNeviMesh&& _Other) = delete;
};