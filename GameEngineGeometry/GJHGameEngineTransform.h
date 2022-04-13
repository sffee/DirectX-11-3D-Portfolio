#pragma once
#include <GJHGameEngineMath.h>
#include <GJHGameEngineMemory.h>
#include <GJHUpdaterBase.h>

#include <DirectXCollision.h>
#include <DirectXCollision.inl>

enum class COLTYPE
{
	AABB,
	OBB,
	SPHERE,
	POINT,
	CIRCULARSECTOR2D,
	NONE
};

class GJHGameEngineWorld;
class GJHGameEngineTransform : public GJHUpdaterBase, public SharedPtrThis<GJHGameEngineTransform>
{
private:
	friend GJHGameEngineWorld;

private:
	bool CalOn;

public:
	bool IsCalMatrix()
	{
		return CalOn;
	}

	void CalMatrixOff()
	{
		CalOn = false;
	}

	void CalMatrixOn()
	{
		CalOn = true;
	}

private:
	GJHGameEngineWorld* m_ParentWorld;

	float4 m_LocalScale;
	float4 m_LocalRotation;
	float4 m_LocalPosition;

	float4 m_WorldScale;
	float4 m_WorldRotation;
	float4 m_WorldPosition;

	bool CheckTransUpdate[(int)MatrixType::Matrix_Max];

	std::weak_ptr<GJHGameEngineTransform> m_ParentTrans;
	std::list<GJHGameEngineTransform*> m_ChildTransList;

public:
	TransformData TransData;

public:
	void AttachParent(std::shared_ptr<GJHGameEngineTransform> _Parent);
	void DetachParent();

public:
	float4x4 GetWorldMatrix()
	{
		return TransData.World;
	}

	void SetLocalMove(const float4& _Value)
	{
		float4 LocalPos = m_LocalPosition;
		SetLocalPosition(LocalPos + _Value);
	}

	void SetWorldMove(const float4& _Value)
	{
		float4 WorldPos = m_WorldPosition;
		SetWorldPosition(WorldPos + _Value);
	}

	void SetLocalRotationX(const float _Value)
	{
		float4 LocalRot = m_LocalRotation;
		LocalRot.x = _Value;
		SetLocalRotation(LocalRot);
	}

	void SetLocalRotationY(const float _Value)
	{
		float4 LocalRot = m_LocalRotation;
		LocalRot.y = _Value;
		SetLocalRotation(LocalRot);
	}

	void SetLocalRotationZ(const float _Value)
	{
		float4 LocalRot = m_LocalRotation;
		LocalRot.z = _Value;
		SetLocalRotation(LocalRot);
	}

	void AddLocalRotation(const float4& _Value)
	{
		float4 LocalRot = m_LocalRotation;
		LocalRot += _Value;
		SetLocalRotation(LocalRot);
	}

	void AddLocalRotationX(const float _Value)
	{
		float4 LocalRot = m_LocalRotation;
		LocalRot.x += _Value;
		SetLocalRotation(LocalRot);
	}

	void AddLocalRotationY(const float _Value)
	{
		float4 LocalRot = m_LocalRotation;
		LocalRot.y += _Value;
		SetLocalRotation(LocalRot);
	}

	void AddLocalRotationZ(const float _Value)
	{
		float4 LocalRot = m_LocalRotation;
		LocalRot.z += _Value;
		SetLocalRotation(LocalRot);
	}

	void SetWorldRotationX(const float _Value)
	{
		float4 WorldRot = m_WorldRotation;
		WorldRot.x = _Value;
		SetWorldRotation(WorldRot);
	}

	void SetWorldRotationY(const float _Value)
	{
		float4 WorldRot = m_WorldRotation;
		WorldRot.y = _Value;
		SetWorldRotation(WorldRot);
	}

	void SetWorldRotationZ(const float _Value)
	{
		float4 WorldRot = m_WorldRotation;
		WorldRot.z = _Value;
		SetWorldRotation(WorldRot);
	}

	void AddWorldRotation(const float4& _Value)
	{
		float4 WorldRot = m_WorldRotation;
		WorldRot += _Value;
		SetWorldRotation(WorldRot);
	}

	void AddWorldRotationX(const float _Value)
	{
		float4 WorldRot = m_WorldRotation;
		WorldRot.x += _Value;
		SetWorldRotation(WorldRot);
	}

	void AddWorldRotationY(const float _Value)
	{
		float4 WorldRot = m_WorldRotation;
		WorldRot.y += _Value;
		SetWorldRotation(WorldRot);
	}

	void AddWorldRotationZ(const float _Value)
	{
		float4 WorldRot = m_WorldRotation;
		WorldRot.z += _Value;
		SetWorldRotation(WorldRot);
	}

private:
	void CalLocalPosition()
	{
		std::shared_ptr<GJHGameEngineTransform> Parent = m_ParentTrans.lock();
		float4 ChangeVector = ((m_WorldPosition - Parent->m_WorldPosition) / Parent->m_WorldScale);

		ChangeVector.RotationDeg3D(-Parent->m_WorldRotation);
		m_LocalPosition = ChangeVector;
		CheckTransUpdate[(int)MatrixType::Matrix_Position] = true;
	}

	void CalWorldPosition()
	{
		std::shared_ptr<GJHGameEngineTransform> Parent = m_ParentTrans.lock();
		float4 ChangeVector = m_LocalPosition;
		ChangeVector.RotationDeg3D(Parent->m_WorldRotation);
		m_WorldPosition = (ChangeVector * Parent->m_WorldScale) + Parent->m_WorldPosition;
		CheckTransUpdate[(int)MatrixType::Matrix_Position] = true;
	}

	void CalChildWorldPosition()
	{
		for (auto& _Child : m_ChildTransList)
		{
			_Child->CalWorldPosition();
			_Child->CalChildWorldPosition();
		}
	}

public:
	void SetLocalPosition(const float4& _Value)
	{
		if (m_ParentTrans.lock() == nullptr)
		{
			m_WorldPosition = m_LocalPosition = _Value;
			CalChildWorldPosition();
			CheckTransUpdate[(int)MatrixType::Matrix_Position] = true;

			return;
		}

		m_LocalPosition = _Value;
		CalWorldPosition();
		CalChildWorldPosition();
		CheckTransUpdate[(int)MatrixType::Matrix_Position] = true;
	}

	void SetLocalPositionX(float _Value)
	{
		SetLocalPosition({ _Value, m_LocalPosition.y, m_LocalPosition.z });
	}

	void SetLocalPositionY(float _Value)
	{
		SetLocalPosition({ m_LocalPosition.x, _Value, m_LocalPosition.z });
	}

	void SetLocalPositionZ(float _Value)
	{
		SetLocalPosition({ m_LocalPosition.x, m_LocalPosition.y, _Value });
	}

	void SetWorldPosition(const float4& _Value)
	{
		if (m_ParentTrans.lock() == nullptr)
		{
			m_WorldPosition = m_LocalPosition = _Value;
			CalChildWorldPosition();
			CheckTransUpdate[(int)MatrixType::Matrix_Position] = true;

			return;
		}

		m_WorldPosition = _Value;
		CalLocalPosition();
		CalChildWorldPosition();
		CheckTransUpdate[(int)MatrixType::Matrix_Position] = true;
	}

	void SetWorldPositionX(float _Value)
	{
		SetWorldPosition({ _Value, m_WorldPosition.y, m_WorldPosition.z });
	}

	void SetWorldPositionY(float _Value)
	{
		SetWorldPosition({ m_WorldPosition.x, _Value, m_WorldPosition.z });
	}

	void SetWorldPositionZ(float _Value)
	{
		SetWorldPosition({ m_WorldPosition.x, m_WorldPosition.y, _Value });
	}

private:
	void CalLocalRotation()
	{
		m_LocalRotation = m_WorldRotation - m_ParentTrans.lock()->m_WorldRotation;
		CheckTransUpdate[(int)MatrixType::Matrix_Rotation] = true;
	}

	void CalWorldRotation()
	{
		m_WorldRotation = m_ParentTrans.lock()->m_LocalRotation + m_LocalRotation;
		CheckTransUpdate[(int)MatrixType::Matrix_Rotation] = true;
	}

	void CalChildWorldRotation()
	{
		for (auto& _Child : m_ChildTransList)
		{
			_Child->CalWorldRotation();
			_Child->CalChildWorldRotation();
		}
	}

public:
	void SetWorldRotation(const float4& _Value)
	{
		if (m_ParentTrans.lock() == nullptr)
		{
			m_WorldRotation = m_LocalRotation = _Value;
			CalChildWorldRotation();
			CalChildWorldPosition();
			CheckTransUpdate[(int)MatrixType::Matrix_Rotation] = true;

			return;
		}

		m_WorldRotation = _Value;
		CalLocalRotation();
		CalChildWorldRotation();
		CalChildWorldPosition();
		CheckTransUpdate[(int)MatrixType::Matrix_Rotation] = true;
	}

	void SetLocalRotation(const float4& _Value)
	{
		if (m_ParentTrans.lock() == nullptr)
		{
			m_WorldRotation = m_LocalRotation = _Value;
			CalChildWorldRotation();
			CalChildWorldPosition();
			CheckTransUpdate[(int)MatrixType::Matrix_Rotation] = true;
			return;
		}

		m_LocalRotation = _Value;
		CalWorldRotation();
		CalWorldPosition();
		CalChildWorldRotation();
		CalChildWorldPosition();
		CheckTransUpdate[(int)MatrixType::Matrix_Rotation] = true;
	}

private:
	void CalLocalScale()
	{
		m_LocalScale = m_WorldScale / m_ParentTrans.lock()->m_WorldScale;
		CheckTransUpdate[(int)MatrixType::Matrix_Scale] = true;
	}

	void CalWorldScale()
	{
		m_WorldScale = m_ParentTrans.lock()->m_WorldScale * m_LocalScale;
		CheckTransUpdate[(int)MatrixType::Matrix_Scale] = true;
	}

	void CalChildWorldScale()
	{
		for (auto& _Child : m_ChildTransList)
		{
			_Child->CalWorldScale();
			_Child->CalChildWorldScale();
		}
	}

public:
	void SetLocalScale(const float4& _Value)
	{
		if (m_ParentTrans.lock() == nullptr)
		{
			m_WorldScale = m_LocalScale = _Value;
			CalChildWorldScale();
			CalChildWorldPosition();
			CheckTransUpdate[(int)MatrixType::Matrix_Scale] = true;

			return;
		}

		m_LocalScale = _Value;
		CalWorldScale();
		CalChildWorldScale();
		CalChildWorldPosition();
		CheckTransUpdate[(int)MatrixType::Matrix_Scale] = true;
	}

	void SetLocalScaleX(float _Value)
	{
		SetLocalScale({ _Value, m_WorldScale.y, m_WorldScale.z });
	}

	void SetLocalScaleY(float _Value)
	{
		SetLocalScale({ m_WorldScale.x, _Value, m_WorldScale.z });
	}

	void SetLocalScaleZ(float _Value)
	{
		SetLocalScale({ m_WorldScale.x, m_WorldScale.y, _Value });
	}

	void SetWorldScale(const float4& _Value)
	{
		if (m_ParentTrans.lock() == nullptr)
		{
			m_WorldScale = m_LocalScale = _Value;
			CalChildWorldScale();
			CalChildWorldPosition();
			CheckTransUpdate[(int)MatrixType::Matrix_Scale] = true;

			return;
		}

		m_WorldScale = _Value;
		CalLocalScale();
		CalChildWorldScale();
		CalChildWorldPosition();
		CheckTransUpdate[(int)MatrixType::Matrix_Scale] = true;
	}

	void SetWorldScaleX(float _Value)
	{
		SetWorldScale({ _Value, m_WorldScale.y, m_WorldScale.z });
	}

	void SetWorldScaleY(float _Value)
	{
		SetWorldScale({ m_WorldScale.x, _Value, m_WorldScale.z });
	}

	void SetWorldScaleZ(float _Value)
	{
		SetWorldScale({ m_WorldScale.x, m_WorldScale.y, _Value });
	}

public:
	float4 GetLocalScale()
	{
		return m_LocalScale;
	}

	float4 GetWorldScale()
	{
		return m_WorldScale;
	}

	float4 GetLocalPosition()
	{
		return m_LocalPosition;
	}

	float4 GetWorldPosition()
	{
		return m_WorldPosition;
	}

	float4 GetLocalRotation()
	{
		return m_LocalRotation;
	}

	float4 GetWorldRotation()
	{
		return m_WorldRotation;
	}

	float4 GetLocalForward()
	{
		return TransData.LocalWorld.ArrVector[2].GetNormalize();
	}

	float4 GetWorldForward()
	{
		return TransData.World.ArrVector[2].GetNormalize();
	}

	float4 GetLocalBack()
	{
		return -GetLocalForward();
	}

	float4 GetWorldBack()
	{
		return -GetWorldForward();
	}

	float4 GetLocalUp()
	{
		return TransData.LocalWorld.ArrVector[1].GetNormalize();
	}

	float4 GetWorldUp()
	{
		return TransData.World.ArrVector[1].GetNormalize();
	}

	float4 GetLocalDown()
	{
		return -GetLocalUp();
	}

	float4 GetWorldDown()
	{
		return -GetWorldUp();
	}

	float4 GetLocalRight()
	{
		return TransData.LocalWorld.ArrVector[0].GetNormalize();
	}

	float4 GetWorldRight()
	{
		return TransData.World.ArrVector[0].GetNormalize();
	}

	float4 GetLocalLeft()
	{
		return -GetLocalRight();
	}

	float4 GetWorldLeft()
	{
		return -GetWorldRight();
	}

public:
	bool IsParent()
	{
		return m_ParentTrans.use_count() != 0;
	}

public:
	void CalTransData();

public:
	GJHGameEngineTransform();
	GJHGameEngineTransform(GJHGameEngineWorld* _ParentWorld);
	~GJHGameEngineTransform();

public:
	GJHGameEngineTransform(const GJHGameEngineTransform& _Other) = delete;
	GJHGameEngineTransform(const GJHGameEngineTransform&& _Other) = delete;

public:
	GJHGameEngineTransform operator=(const GJHGameEngineTransform& _Other) = delete;
	GJHGameEngineTransform operator=(const GJHGameEngineTransform&& _Other) = delete;

public:
	struct ColData
	{
		union
		{
			DirectX::BoundingBox Aabb;
			DirectX::BoundingSphere Sphere;
			DirectX::BoundingOrientedBox Obb;
		};

	public:
		ColData() :
			Obb()
		{

		}
	};

	ColData m_ColData;

	void ColUpdate(COLTYPE _Type);
	void ColUpdate(COLTYPE _Type, float4 _WorldPos, float4 _WorldScale, float4 _WorldRot);

	DirectX::BoundingBox& AABB();
	DirectX::BoundingOrientedBox& OBB();
	DirectX::BoundingSphere& SPHERE();

public:
	void Release();
	void Reset();
};

class Ray
{
public:
	DirectX::XMFLOAT3 Start;
	DirectX::XMFLOAT3 Dir;
};