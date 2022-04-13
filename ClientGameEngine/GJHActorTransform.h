#pragma once
#include <GJHGameEngineComponent.h>
#include <GJHGameEngineTransForm.h>
#include "GJHRenderingStruct.h"

class GJHCamera;
class GJHWorldComponent;
class GJHActorTransform : public GJHGameEngineComponent
{
private:
	friend GJHCamera;

private:
	GJHWorldComponent* m_MasterWorld;
	GJHRenderingTransformData m_RenderingData;

	// 만드는 동안은 환장할거 같다.
public:
	std::shared_ptr<GJHGameEngineTransform> m_WorldTransform;

protected:
	GJHGameEngineTransform* GetWorldTransform()
	{
		return m_WorldTransform.get();
	}



public:
	std::shared_ptr<GJHGameEngineTransform> GetSharedWorldTransform()
	{
		return m_WorldTransform;
	}

	GJHWorldComponent* GetMasterWorld()
	{
		return m_MasterWorld;
	}

	void AttachParent(std::shared_ptr<GJHActorTransform> _Parent)
	{
		m_WorldTransform->AttachParent(_Parent->m_WorldTransform);
	}

	void DetachParent()
	{
		m_WorldTransform->DetachParent();
	}

public:
	void CalRenderDataCopy();

	inline GJHRenderingTransformData& GetRenderingData()
	{
		return m_RenderingData;
	}

	inline TransformData& GetTransformData()
	{
		return m_WorldTransform->TransData;
	}

public:
	float4 GetLocalForward()
	{
		return m_WorldTransform->GetLocalForward();
	}

	float4 GetWorldForward()
	{
		return m_WorldTransform->GetWorldForward();
	}

	float4 GetLocalBack()
	{
		return m_WorldTransform->GetLocalBack();
	}

	float4 GetWorldBack()
	{
		return m_WorldTransform->GetWorldBack();
	}

	float4 GetLocalUp()
	{
		return m_WorldTransform->GetLocalUp();
	}

	float4 GetWorldUp()
	{
		return m_WorldTransform->GetWorldUp();
	}

	float4x4 GetWorldMatrix()
	{
		m_WorldTransform->CalTransData();
		return m_WorldTransform->GetWorldMatrix();
	}

	float4 GetLocalDown()
	{
		return m_WorldTransform->GetLocalDown();
	}

	float4 GetWorldDown()
	{
		return m_WorldTransform->GetWorldDown();
	}

	float4 GetLocalRight()
	{
		return m_WorldTransform->GetLocalRight();
	}

	float4 GetWorldRight()
	{
		return m_WorldTransform->GetWorldRight();
	}

	float4 GetLocalLeft()
	{
		return m_WorldTransform->GetLocalLeft();
	}

	float4 GetWorldLeft()
	{
		return m_WorldTransform->GetWorldLeft();
	}

	float4 GetLocalPosition() const
	{
		return m_WorldTransform->GetLocalPosition();
	}

	float4 GetWorldPosition() const
	{
		return m_WorldTransform->GetWorldPosition();
	}

	float4 GetLocalScale() const
	{
		return m_WorldTransform->GetLocalScale();
	}

	float4 GetWorldScale() const
	{
		return m_WorldTransform->GetWorldScale();
	}

	float4 GetLocalRotation() const
	{
		return m_WorldTransform->GetLocalRotation();
	}

	float4 GetWorldRotation() const
	{
		return m_WorldTransform->GetWorldRotation();
	}

	void SetLocalScale(const float4& _Scale)
	{
		m_WorldTransform->SetLocalScale(_Scale);
	}

	void SetLocalScaleX(float _Scale)
	{
		m_WorldTransform->SetLocalScaleX(_Scale);
	}

	void SetLocalScaleY(float _Scale)
	{
		m_WorldTransform->SetLocalScaleY(_Scale);
	}

	void SetLocalScaleZ(float _Scale)
	{
		m_WorldTransform->SetLocalScaleZ(_Scale);
	}

	void SetWorldScaleX(float _Scale)
	{
		m_WorldTransform->SetWorldScaleX(_Scale);
	}

	void SetWorldScaleY(float _Scale)
	{
		m_WorldTransform->SetWorldScaleY(_Scale);
	}

	void SetWorldScaleZ(float _Scale)
	{
		m_WorldTransform->SetWorldScaleZ(_Scale);
	}

	void SetWorldScale(const float4& _Scale)
	{
		m_WorldTransform->SetWorldScale(_Scale);
	}

	void SetLocalPosition(const float4& _Pos)
	{
		m_WorldTransform->SetLocalPosition(_Pos);
	}

	void SetLocalPositionX(float _Value)
	{
		m_WorldTransform->SetLocalPositionX(_Value);
	}

	void SetLocalPositionY(float _Value)
	{
		m_WorldTransform->SetLocalPositionY(_Value);
	}

	void SetLocalPositionZ(float _Value)
	{
		m_WorldTransform->SetLocalPositionZ(_Value);
	}

	void SetWorldPosition(const float4& _Pos)
	{
		m_WorldTransform->SetWorldPosition(_Pos);
	}

	void SetWorldPositionX(float _Value)
	{
		m_WorldTransform->SetWorldPositionX(_Value);
	}

	void SetWorldPositionY(float _Value)
	{
		m_WorldTransform->SetWorldPositionY(_Value);
	}

	void SetWorldPositionZ(float _Value)
	{
		m_WorldTransform->SetWorldPositionZ(_Value);
	}

	void SetLocalRotation(const float4& _Value)
	{
		m_WorldTransform->SetLocalRotation(_Value);
	}

	void SetLocalRotationX(const float _Value)
	{
		m_WorldTransform->SetLocalRotationX(_Value);
	}

	void SetLocalRotationY(const float _Value)
	{
		m_WorldTransform->SetLocalRotationY(_Value);
	}

	void SetLocalRotationZ(const float _Value)
	{
		m_WorldTransform->SetLocalRotationZ(_Value);
	}

	void SetWorldRotation(const float4& _Value)
	{
		m_WorldTransform->SetWorldRotation(_Value);
	}

	void SetWorldRotationX(const float _Value)
	{
		m_WorldTransform->SetWorldRotationX(_Value);
	}

	void SetWorldRotationY(const float _Value)
	{
		m_WorldTransform->SetWorldRotationY(_Value);
	}

	void SetWorldRotationZ(const float _Value)
	{
		m_WorldTransform->SetWorldRotationZ(_Value);
	}

	void AddLocalRotation(const float4& _Value)
	{
		m_WorldTransform->AddLocalRotation(_Value);
	}

	void AddLocalRotationX(const float _Value)
	{
		m_WorldTransform->AddLocalRotationX(_Value);
	}

	void AddLocalRotationY(const float _Value)
	{
		m_WorldTransform->AddLocalRotationY(_Value);
	}

	void AddLocalRotationZ(const float _Value)
	{
		m_WorldTransform->AddLocalRotationZ(_Value);
	}

	void AddWorldRotation(const float4& _Value)
	{
		m_WorldTransform->AddWorldRotation(_Value);
	}

	void AddWorldRotationX(const float _Value)
	{
		m_WorldTransform->AddWorldRotationX(_Value);
	}

	void AddWorldRotationY(const float _Value)
	{
		m_WorldTransform->AddWorldRotationY(_Value);
	}

	void AddWorldRotationZ(const float _Value)
	{
		m_WorldTransform->AddWorldRotationZ(_Value);
	}

	void SetLocalMove(const float4& _Pos)
	{
		m_WorldTransform->SetLocalMove(_Pos);
	}

	void SetWorldMove(const float4& _Pos)
	{
		m_WorldTransform->SetWorldMove(_Pos);
	}

	void CalMatrixOn();
	void CalMatrixOff();

public:
	GJHActorTransform();
	~GJHActorTransform();

public:
	GJHActorTransform(const GJHActorTransform& _Other) = delete;
	GJHActorTransform(const GJHActorTransform&& _Other) = delete;

public:
	GJHActorTransform operator=(const GJHActorTransform& _Other) = delete;
	GJHActorTransform operator=(const GJHActorTransform&& _Other) = delete;

public:
	void CameraSetting(GJHCamera* _Camera);

public:
	void Start() override;
	void DeathTask() override;
};

using ActorTransPtr = std::shared_ptr<GJHActorTransform>;