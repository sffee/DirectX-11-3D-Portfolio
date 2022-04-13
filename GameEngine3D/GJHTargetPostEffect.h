#pragma once
#include "GJHRenderPlayer.h"
#include "GJHDirectRenderTarget.h"
#include <list>

class GJHTargetPostEffect : std::enable_shared_from_this<GJHTargetPostEffect>
{
private:
	static std::map<GJHGameEngineString, std::shared_ptr<GJHTargetPostEffect>> m_AllPostEffect;
	static std::map<int, std::list<std::shared_ptr<GJHTargetPostEffect>>> m_UpdatePostEffect;

public:
	static void ProcessEffect(int _EffectOrder);

public:
	template<typename EffectType>
	static std::shared_ptr<EffectType> CreatePostEffect(std::shared_ptr<GJHDirectRenderTarget> _EffectTarget, const GJHGameEngineString& _Name, int _Order)
	{
		std::shared_ptr<EffectType> CreateEffect = std::make_shared<EffectType>();

		std::shared_ptr<GJHTargetPostEffect> Ptr = std::dynamic_pointer_cast<GJHTargetPostEffect>(CreateEffect);
		CreatePostEffect(_EffectTarget, _Name, _Order, Ptr);

		return CreateEffect;
	}

	template<typename EffectType>
	static std::shared_ptr<EffectType> FindPostEffect(const GJHGameEngineString& _Name)
	{
		std::map<GJHGameEngineString, std::shared_ptr<GJHTargetPostEffect>>::iterator Find = m_AllPostEffect.find(_Name);
		if (Find == m_AllPostEffect.end())
		{
			GJHGameEngineDebug::AssertMsg("if (Find == m_AllPostEffect.end())");
		}

		return std::dynamic_pointer_cast<EffectType>(Find->second);
	}

private:
	static void CreatePostEffect(std::shared_ptr<GJHDirectRenderTarget> _EffectTarget, const GJHGameEngineString& _Name, int _Order, std::shared_ptr<GJHTargetPostEffect> _Effect);

private:
	bool m_IsEffect;
	GJHGameEngineString m_EffectName;

public:
	void IsEffectOn()
	{
		m_IsEffect = true;
	}

	void IsEffectOff()
	{
		m_IsEffect = false;
	}

protected:
	std::shared_ptr<GJHDirectRenderTarget> m_EffectTarget;
	std::shared_ptr<GJHDirectRenderTarget> m_ResultTarget;

public:
	void SetEffectTarget(std::shared_ptr<GJHDirectRenderTarget> _EffectTarget)
	{
		m_EffectTarget = _EffectTarget;
	}

	void SetResultTarget(std::shared_ptr<GJHDirectRenderTarget> _ResultTarget)
	{
		m_ResultTarget = _ResultTarget;
	}

	std::shared_ptr<GJHDirectRenderTarget> GetEffectTarget()
	{
		return m_EffectTarget;
	}

	std::shared_ptr<GJHDirectRenderTarget> GetResultTarget()
	{
		return m_ResultTarget;
	}

public:
	GJHTargetPostEffect();
	virtual ~GJHTargetPostEffect();

public:
	GJHTargetPostEffect(const GJHTargetPostEffect& _Other) = delete;
	GJHTargetPostEffect(const GJHTargetPostEffect&& _Other) = delete;

public:
	GJHTargetPostEffect operator=(const GJHTargetPostEffect& _Other) = delete;
	GJHTargetPostEffect operator=(const GJHTargetPostEffect&& _Other) = delete;

public:
	virtual void Start() { }
	virtual void Effect() { }
};