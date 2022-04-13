#pragma once
#include <GJHContentsComponent.h>
#include <GJHCollisionComponent.h>
#include <GJHStateUpdater.h>

class GJHFbxRenderer;
class GJHFbxAnimationRenderer;
class ObjectBase : public GJHContentsComponent
{
public:
	ObjectBase();
	~ObjectBase();

public:
	ObjectBase(const ObjectBase& _Other) = delete;
	ObjectBase(const ObjectBase&& _Other) = delete;

public:
	ObjectBase operator=(const ObjectBase& _Other) = delete;
	ObjectBase operator=(const ObjectBase&& _Other) = delete;

protected:
	std::shared_ptr<GJHCollisionComponent> m_Collision;
	std::shared_ptr<GJHFbxRenderer> m_Render;
	std::shared_ptr<GJHFbxAnimationRenderer> m_AniRender;

protected:
	template <typename T>
	void CreateCollision(int _Order, COLTYPE _Type, void(T::* _EnterFunc)(GJHCollisionComponent*, GJHCollisionComponent*), void(T::* _StayFunc)(GJHCollisionComponent*, GJHCollisionComponent*), void(T::* _ExitFunc)(GJHCollisionComponent*, GJHCollisionComponent*), T& _Obj)
	{
		m_Collision = CreateComponent<GJHCollisionComponent>(_Order, _Type);

		if (_EnterFunc != nullptr)
		{
			m_Collision->AddEnterEvent(_EnterFunc, _Obj);
		}

		if (_StayFunc != nullptr)
		{
			m_Collision->AddStayEvent(_StayFunc, _Obj);
		}

		if (_ExitFunc != nullptr)
		{
			m_Collision->AddExitEvent(_ExitFunc, _Obj);
		}
	}

protected:
	void CreateAnimation(const GJHGameEngineString& _AnimationName, const GJHGameEngineString& _MeshFbxName, const GJHGameEngineString& _AnimationFbxName, int _AnimationIndex, bool _Loop, float _FrameTime);
	void ChangeAnimation(const GJHGameEngineString& _AnimationName);
	bool AnimationEnd();

public:
	void Update() override;

private:
	void CollisionUpdate();
};