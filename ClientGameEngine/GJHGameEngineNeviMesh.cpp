#include "GJHGameEngineNeviMesh.h"
#include <GJHGameEngineThread.h>
#include <GJHGameEngineFile.h>
#include <GJHGameEngineCollision.h>

std::mutex GJHGameEngineNeviMesh::CurThreadLock;
std::vector<std::shared_ptr<NeviTri>>::iterator GJHGameEngineNeviMesh::CurThreadIter;
std::vector<std::shared_ptr<NeviTri>>::iterator GJHGameEngineNeviMesh::CurEndIter;

void GJHGameEngineNeviMesh::CalNeviThread(int Count)
{
	GJHGameEngineString Name = Count;
	Name += L"NeviCreateThread";

	HRESULT r;
	r = SetThreadDescription(GetCurrentThread(), Name.c_str());

	while (true)
	{
		CurThreadLock.lock();

		std::vector<std::shared_ptr<NeviTri>>::iterator BeginIter = CurThreadIter;
		if (CurThreadIter == CurEndIter)
		{
			CurThreadLock.unlock();
			break;
		}

		++CurThreadIter;

		CurThreadLock.unlock();

		std::vector<std::shared_ptr<NeviTri>>::iterator OtherIter = BeginIter;
		++OtherIter;

		for (; OtherIter != CurEndIter; ++OtherIter)
		{
			for (size_t i = 0; i < 3; i++)
			{
				bool Check = false;

				for (size_t j = 0; j < 3; j++)
				{
					if ((*BeginIter)->TriArr[i] == (*OtherIter)->TriArr[j])
					{
						Check = true;
						break;
					}
				}
				if (Check == true)
				{
					(*BeginIter)->PushLink((*OtherIter));
					(*OtherIter)->PushLink((*BeginIter));
					break;
				}
			}
		}
	}
}

GJHGameEngineNeviMesh::GJHGameEngineNeviMesh()
{
}

GJHGameEngineNeviMesh::~GJHGameEngineNeviMesh()
{
}

void GJHGameEngineNeviMesh::Create(const GJHGameEngineString& _Path, std::vector<float4> _PosList, std::vector<unsigned int> _IdxList)
{
	if (GJHGameEngineFile::IsExist(_Path) == true)
	{
		GJHGameEngineFile File;
		File.Open(_Path, L"rb");
		int Count = 0;
		File >> Count;

		m_ArrTri.resize(Count);

		for (int i = 0; i < Count; i++)
		{
			std::shared_ptr<NeviTri> NewTri = std::make_shared<NeviTri>();
			m_ArrTri[i] = NewTri;
		}

		for (int i = 0; i < Count; i++)
		{
			int Index = 0;
			File >> Index;
			m_ArrTri[Index]->Index = Index;
			File >> m_ArrTri[Index]->IsMoving;
			File >> m_ArrTri[Index]->TriArr[0];
			File >> m_ArrTri[Index]->TriArr[1];
			File >> m_ArrTri[Index]->TriArr[2];

			int LinkCount = 0;
			File >> LinkCount;

			for (int j = 0; j < LinkCount; j++)
			{
				int LinkIndex = 0;
				File >> LinkIndex;
				m_ArrTri[Index]->m_NeviListPtr.push_back(m_ArrTri[LinkIndex]);
			}
		}

		return;
	}

	int Count = 0;

	for (size_t i = 0; i < _IdxList.size(); i += 3)
	{
		std::shared_ptr<NeviTri> NewTri = std::make_shared<NeviTri>();
		NewTri->TriArr[0] = _PosList[_IdxList[i + 0]];
		NewTri->TriArr[1] = _PosList[_IdxList[i + 1]];
		NewTri->TriArr[2] = _PosList[_IdxList[i + 2]];
		NewTri->Index = Count++;
		m_ArrTri.push_back(NewTri);
	}

	std::vector<std::shared_ptr<NeviTri>>::iterator BeginIter = m_ArrTri.begin();
	std::vector<std::shared_ptr<NeviTri>>::iterator EndIter = m_ArrTri.end();

	SYSTEM_INFO Info;
	GetSystemInfo(&Info);
	int ThreadCount = (int)((float)Info.dwNumberOfProcessors * 0.7f);

	std::vector<std::thread*> ThreadList = std::vector<std::thread*>();

	CurThreadIter = m_ArrTri.begin();
	CurEndIter = m_ArrTri.end();
	--CurEndIter;

	for (int i = 0; i < ThreadCount; i++)
	{
		std::thread* Thread = new std::thread(&GJHGameEngineNeviMesh::CalNeviThread, i);
		ThreadList.push_back(Thread);
	}

	for (size_t i = 0; i < ThreadList.size(); i++)
	{
		ThreadList[i]->join();
	}

	for (size_t i = 0; i < ThreadList.size(); i++)
	{
		delete ThreadList[i];
	}

	GJHGameEngineFile File;
	File.Open(_Path, L"wb");
	File << (int)m_ArrTri.size();
	for (size_t i = 0; i < m_ArrTri.size(); i++)
	{
		File << m_ArrTri[i]->Index;
		File << m_ArrTri[i]->IsMoving;

		File << m_ArrTri[i]->TriArr[0];
		File << m_ArrTri[i]->TriArr[1];
		File << m_ArrTri[i]->TriArr[2];

		File << (int)m_ArrTri[i]->m_NeviListPtr.size();
		for (size_t j = 0; j < m_ArrTri[i]->m_NeviListPtr.size(); j++)
		{
			std::shared_ptr<NeviTri> Tri = m_ArrTri[i]->m_NeviListPtr[j].lock();

			File << Tri->Index;
		}
	}

	return;
}

std::shared_ptr<NeviWorker> GJHGameEngineNeviMesh::GetWorker(std::shared_ptr<GJHActorTransform> _Target, float4 _RayDir)
{
	std::shared_ptr<NeviWorker> NewWorker = std::make_shared<NeviWorker>(this);
	NewWorker->Init(_Target, _RayDir);

	return NewWorker;
}


float NeviTri::CheckY(const float4& _Position, const float4& _RayDir, const float4x4& _World)
{
	float4 Pos = _Position;
	float4 Value0 = TriArr[0] * _World;
	float4 Value1 = TriArr[1] * _World;
	float4 Value2 = TriArr[2] * _World;

	float Dis0 = 0.0f;
	GJHGameEngineCollision::TriToRayDis(Value0, Value1, Value2, Pos, _RayDir, Dis0);

	float Dis1 = 0.0f;
	GJHGameEngineCollision::TriToRayDis(Value0, Value1, Value2, Pos, -_RayDir, Dis1);

	if (Dis0 == 0.0f && Dis1 == 0.0f)
	{
		return 0.0f;
	}

	if (Dis0 != 0.0f)
	{
		return Dis0;
	}

	if (Dis1 != 0.0f)
	{
		return Dis1;
	}

	return 0.f;
}

bool NeviTri::MoveCheck(const float4& _StartPostion, const float4& _MoveScale, const float4& _RayDir, const float4x4& _World)
{
	float4 Value0 = TriArr[0] * _World;
	float4 Value1 = TriArr[1] * _World;
	float4 Value2 = TriArr[2] * _World;

	return GJHGameEngineCollision::TriToRayContaine(Value0, Value1, Value2, _StartPostion + _MoveScale, _RayDir);
}

float NeviWorker::CheckY(float4 _Pos)
{
	float4x4 World = m_Parent->GetActorTrans()->GetWorldMatrix();

	return m_CurTri->CheckY(_Pos, RayDir, World);
}

float NeviWorker::CheckY()
{
	return CheckY(m_Target->GetWorldPosition());
}

bool NeviWorker::MoveCheck(float4 _MoveScale)
{
	float4x4 World = m_Parent->GetActorTrans()->GetWorldMatrix();

	if (true == m_CurTri->MoveCheck(m_Target->GetWorldPosition(), _MoveScale, RayDir, World))
	{
		return true;
	}

	for (auto& _OtherTri : m_CurTri->m_NeviListPtr)
	{
		std::shared_ptr<NeviTri> Tri = _OtherTri.lock();

		if (Tri->MoveCheck(m_Target->GetWorldPosition(), _MoveScale, RayDir, World) == true)
		{
			m_CurTri = Tri;
			return true;
		}
	}

	return false;
}


std::shared_ptr<NeviTri> GJHGameEngineNeviMesh::FindTri(std::shared_ptr<GJHActorTransform> _Target, float4 _RayDir)
{
	float4x4 World = GetActorTrans()->GetWorldMatrix();

	for (size_t i = 0; i < m_ArrTri.size(); i++)
	{
		float4 Value0 = m_ArrTri[i]->TriArr[0] * World;
		float4 Value1 = m_ArrTri[i]->TriArr[1] * World;
		float4 Value2 = m_ArrTri[i]->TriArr[2] * World;

		if (GJHGameEngineCollision::TriToRayContaine(Value0, Value1, Value2, _Target->GetWorldPosition(), _RayDir) == true)
		{
			return m_ArrTri[i];
		}
	}

	return nullptr;
}

void NeviWorker::Init(std::shared_ptr<GJHActorTransform> _Target, float4 _RayDir)
{
	m_Target = _Target;
	RayDir = _RayDir;
	RayDirReverse = -_RayDir;

	m_CurTri = m_Parent->FindTri(m_Target, RayDir);
}