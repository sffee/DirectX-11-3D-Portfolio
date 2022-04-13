#pragma once
#include <GJHGameEngineString.h>
#include "GJHResBase.h"
#include "GJHDirectVertexBuffer.h"
#include "GJHDirectIndexBuffer.h"

class GJHDirectMesh : public GJHResBase, public GJHDirectResourcesManager<GJHDirectMesh>
{
public:
	static bool Create(const GJHGameEngineString& _Name)
	{
		GJHDirectMesh* NewRes = new GJHDirectMesh();
		if (NewRes->CreateMesh(_Name, _Name) == false)
		{
			return false;
		}

		InsertRes(_Name, NewRes);

		return true;
	}

	static bool Create(const GJHGameEngineString& _MeshName, const GJHGameEngineString& _VName, const GJHGameEngineString& _IName)
	{
		GJHDirectMesh* NewRes = new GJHDirectMesh();
		if (false == NewRes->CreateMesh(_VName, _IName))
		{
			return false;
		}
		InsertRes(_MeshName, NewRes);

		return true;
	}

	static bool CreateVertexOnly(const GJHGameEngineString& _Name)
	{
		GJHDirectMesh* NewRes = new GJHDirectMesh;

		if (NewRes->VertexBufferPush(_Name) == false)
		{
			return false;
		}

		InsertRes(_Name, NewRes);

		return true;
	}

private:
	std::vector<std::shared_ptr<GJHDirectVertexBuffer>> m_VertexBuffers;
	std::vector<std::shared_ptr<GJHDirectIndexBuffer>> m_IndexBuffers;

public:
	bool VertexBufferPush(const GJHGameEngineString& _Name);
	bool IndexBufferPush(const GJHGameEngineString& _Name);
	bool VertexBufferPush(std::shared_ptr<GJHDirectVertexBuffer> _VB);
	bool IndexBufferPush(std::shared_ptr<GJHDirectIndexBuffer> _IB);

public:
	GJHDirectMesh();
	~GJHDirectMesh();
	GJHDirectMesh(const GJHDirectMesh& _Other) = delete;
	GJHDirectMesh(const GJHDirectMesh&& _Other) = delete;

public:
	GJHDirectMesh operator=(const GJHDirectMesh& _Other) = delete;
	GJHDirectMesh operator=(const GJHDirectMesh&& _Other) = delete;

public:
	bool CreateMesh(std::shared_ptr<GJHDirectVertexBuffer>& _VB, std::shared_ptr<GJHDirectIndexBuffer>& _IB);
	bool CreateMesh(const GJHGameEngineString& _VBName, const GJHGameEngineString& _IBName);
	void Setting() override;
	void RenderIndexed();
	void RenderIndexedInstanced(int _Index);
};