#include "GJHTargetPostEffect.h"

std::map<GJHGameEngineString, std::shared_ptr<GJHTargetPostEffect>> GJHTargetPostEffect::m_AllPostEffect;
std::map<int, std::list<std::shared_ptr<GJHTargetPostEffect>>> GJHTargetPostEffect::m_UpdatePostEffect;

GJHTargetPostEffect::GJHTargetPostEffect() :
	m_ResultTarget(std::make_shared<GJHDirectRenderTarget>()),
	m_EffectTarget(nullptr)
{

}

GJHTargetPostEffect::~GJHTargetPostEffect()
{

}

void GJHTargetPostEffect::ProcessEffect(int _EffectOrder)
{
	std::list<std::shared_ptr<GJHTargetPostEffect>>& _EffectList = m_UpdatePostEffect[_EffectOrder];

	for (auto& _Effect : _EffectList)
	{
		if (_Effect->m_IsEffect == false)
		{
			continue;
		}

		_Effect->Effect();
	}
}

void GJHTargetPostEffect::CreatePostEffect(std::shared_ptr<GJHDirectRenderTarget> _EffectTarget, const GJHGameEngineString& _Name, int _Order, std::shared_ptr<GJHTargetPostEffect> _Effect)
{
	_Effect->m_EffectTarget = _EffectTarget;
	_Effect->m_EffectName = _Name;

	m_UpdatePostEffect[_Order].push_back(_Effect);
	m_AllPostEffect[_Name] = _Effect;

	_Effect->Start();
}