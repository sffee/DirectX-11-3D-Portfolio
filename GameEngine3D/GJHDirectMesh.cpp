#include "GJHDirectMesh.h"
#include "GJHGameEngineDevice.h"

GJHDirectMesh::GJHDirectMesh()
{

}

GJHDirectMesh::~GJHDirectMesh()
{

}

bool GJHDirectMesh::VertexBufferPush(std::shared_ptr<GJHDirectVertexBuffer> _VB)
{
	if (_VB == nullptr)
	{
		MsgAssert(!"if (nullptr == VB)");
		return false;
	}

	m_VertexBuffers.push_back(_VB);
	return true;
}

bool GJHDirectMesh::IndexBufferPush(std::shared_ptr<GJHDirectIndexBuffer> _IB)
{
	if (_IB == nullptr)
	{
		MsgAssert(!"if (nullptr == VB)");
		return false;
	}

	m_IndexBuffers.push_back(_IB);

	return true;
}

bool GJHDirectMesh::VertexBufferPush(const GJHGameEngineString& _Name)
{
	return VertexBufferPush(GJHDirectVertexBuffer::Find(_Name));
}

bool GJHDirectMesh::IndexBufferPush(const GJHGameEngineString& _Name)
{
	return IndexBufferPush(GJHDirectIndexBuffer::Find(_Name));
}

bool GJHDirectMesh::CreateMesh(const GJHGameEngineString& _VBName, const GJHGameEngineString& _IBName)
{
	if (VertexBufferPush(_VBName) == false)
	{
		return false;
	}

	if (IndexBufferPush(_IBName) == false)
	{
		return false;
	}

	return true;
}

void GJHDirectMesh::Setting()
{
	if (m_VertexBuffers.size() != 0 && m_VertexBuffers[0] != nullptr)
	{
		m_VertexBuffers[0]->Setting();
	}

	if (m_IndexBuffers.size() != 0 && m_IndexBuffers[0] != nullptr)
	{
		m_IndexBuffers[0]->Setting();
	}

	return;
}

void GJHDirectMesh::RenderIndexed()
{
	if (m_IndexBuffers.size() != 0 && m_IndexBuffers[0] != nullptr)
	{
		GJHGameEngineDevice::GetContext()->DrawIndexed(m_IndexBuffers[0]->GetIndexCount(), 0, 0);
	}
}

bool GJHDirectMesh::CreateMesh(std::shared_ptr<GJHDirectVertexBuffer>& _VB, std::shared_ptr<GJHDirectIndexBuffer>& _IB)
{
	VertexBufferPush(_VB);
	IndexBufferPush(_IB);
	return true;
}

void GJHDirectMesh::RenderIndexedInstanced(int _Index)
{
	if (m_IndexBuffers.size() != 0 && m_IndexBuffers[0] != nullptr)
	{
		GJHGameEngineDevice::GetContext()->DrawIndexedInstanced(m_IndexBuffers[0]->GetIndexCount(), _Index, 0, 0, 0);
	}
}