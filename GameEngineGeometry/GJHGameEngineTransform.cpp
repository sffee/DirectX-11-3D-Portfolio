#include "GeoPre.h"
#include "GJHGameEngineTransform.h"
#include "GJHGameEngineWorld.h"

GJHGameEngineTransform::GJHGameEngineTransform() :
	m_LocalScale(float4::ONE),
	m_WorldScale(float4::ONE),
	CheckTransUpdate{ false },
	m_ParentTrans(),
	m_ParentWorld(nullptr),
	CalOn(true)
{
	for (bool& Check : CheckTransUpdate)
	{
		Check = true;
	}
}

GJHGameEngineTransform::GJHGameEngineTransform(GJHGameEngineWorld* _ParentWorld) :
	m_LocalScale(float4::ONE),
	m_WorldScale(float4::ONE),
	CheckTransUpdate{ false },
	m_ParentTrans(),
	m_ParentWorld(_ParentWorld),
	CalOn(true)
{

}

GJHGameEngineTransform::~GJHGameEngineTransform()
{

}

void GJHGameEngineTransform::AttachParent(std::shared_ptr<GJHGameEngineTransform> _Parent)
{
	m_ParentWorld->ParentTransCheckOn();
	m_ParentTrans = _Parent;

	_Parent->m_ChildTransList.push_back(this);

	CalWorldScale();
	CalWorldRotation();
	CalWorldPosition();
	CheckTransUpdate[(int)MatrixType::Matrix_World] = true;
}

void GJHGameEngineTransform::DetachParent() 
{
	if (m_ParentTrans.lock() == nullptr)
	{
		return;
	}

	std::shared_ptr<GJHGameEngineTransform> Parent = m_ParentTrans.lock();

	Parent->m_ChildTransList.remove(this);

	m_ParentTrans.reset();

	std::shared_ptr<GJHGameEngineTransform> Ptr = this->shared_from_this();

	m_ParentWorld->PushTrans(Ptr);

	float4 WP = Ptr->GetWorldPosition();
	float4 LP = Ptr->GetLocalPosition();

	float4 WR = Ptr->GetWorldRotation();
	float4 LR = Ptr->GetLocalRotation();

	float4 WS = Ptr->GetWorldScale();
	float4 LS = Ptr->GetLocalScale();

	Ptr->SetLocalPosition(Ptr->GetWorldPosition());
	Ptr->SetLocalRotation(Ptr->GetWorldRotation());
	Ptr->SetLocalScale(Ptr->GetWorldScale());
	TransData.Parent.Iden();
}

void GJHGameEngineTransform::CalTransData()
{
	std::shared_ptr<GJHGameEngineTransform> a = this->shared_from_this();

	DebugCheck();

	if (CalOn == false)
	{
		for (auto& _Child : m_ChildTransList)
		{
			_Child->CheckTransUpdate[(int)MatrixType::Matrix_World] = true;

			_Child->CalTransData();
		}

		return;
	}

	if (CheckTransUpdate[(int)MatrixType::Matrix_Scale] == true)
	{
		TransData.Scale.Scale(m_LocalScale);
		CheckTransUpdate[(int)MatrixType::Matrix_Scale] = false;
		CheckTransUpdate[(int)MatrixType::Matrix_World] = true;
	}

	if (CheckTransUpdate[(int)MatrixType::Matrix_Rotation] == true)
	{
		TransData.Rotation.RotationXYZDeg(m_LocalRotation);
		CheckTransUpdate[(int)MatrixType::Matrix_Rotation] = false;
		CheckTransUpdate[(int)MatrixType::Matrix_World] = true;
	}

	if (CheckTransUpdate[(int)MatrixType::Matrix_Position] == true)
	{
		TransData.Position.Position(m_LocalPosition);
		CheckTransUpdate[(int)MatrixType::Matrix_Position] = false;
		CheckTransUpdate[(int)MatrixType::Matrix_World] = true;
	}

	if (CheckTransUpdate[(int)MatrixType::Matrix_World] == true)
	{
		if (m_ParentTrans.use_count() != 0)
		{
			SharedPtr<GJHGameEngineTransform> Parent = m_ParentTrans.lock();
			TransData.Parent = Parent->TransData.World;
		}

		TransData.CalWorld();
		CheckTransUpdate[(int)MatrixType::Matrix_World] = false;
	}

	for (auto& _Child : m_ChildTransList)
	{
		_Child->CalTransData();
	}
}

void GJHGameEngineTransform::Release()
{

}

void GJHGameEngineTransform::ColUpdate(COLTYPE _Type, float4 _WorldPos, float4 _WorldScale, float4 _WorldRot) 
{
	m_ColData.Obb.Center = _WorldPos.DXXmFloat3;
	m_ColData.Obb.Extents = _WorldScale.HalfVector3D().DXXmFloat3;
	m_ColData.Obb.Orientation = _WorldRot.DegToQuaternion().DXXmFloat4;
}

void GJHGameEngineTransform::ColUpdate(COLTYPE _Type)
{
	m_ColData.Obb.Center = m_WorldPosition.DXXmFloat3;

	m_ColData.Obb.Extents = m_WorldScale.HalfVector3D().DXXmFloat3;

	if (_Type != COLTYPE::CIRCULARSECTOR2D)
	{
		m_ColData.Obb.Orientation = m_WorldRotation.DegToQuaternion().DXXmFloat4;
	}
	else
	{
		m_ColData.Obb.Orientation.x = GetLocalRight().x;
		m_ColData.Obb.Orientation.y = GetLocalRight().y;
		m_ColData.Obb.Orientation.z = GetLocalRight().z;
	}
}

DirectX::BoundingBox& GJHGameEngineTransform::AABB()
{
	return m_ColData.Aabb;
}

DirectX::BoundingOrientedBox& GJHGameEngineTransform::OBB()
{
	return m_ColData.Obb;
}

DirectX::BoundingSphere& GJHGameEngineTransform::SPHERE()
{
	return m_ColData.Sphere;
}

void GJHGameEngineTransform::Reset()
{
	m_LocalScale = float4::ONE;
	m_LocalRotation = float4::ZERO;
	m_LocalPosition = float4::ZERO;
	m_WorldScale = float4::ONE;
	m_WorldRotation = float4::ZERO;
	m_WorldPosition = float4::ZERO;

	for (size_t i = 0; i < (int)MatrixType::Matrix_Max; i++)
	{
		CheckTransUpdate[i] = false;
	}

	m_ParentTrans = std::shared_ptr<GJHGameEngineTransform>(nullptr);
	m_ChildTransList.clear();
}