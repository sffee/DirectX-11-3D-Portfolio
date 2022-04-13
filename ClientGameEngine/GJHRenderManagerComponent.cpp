#include "GJHRenderManagerComponent.h"
#include <GJHGameEngineDevice.h>
#include "GJHRenderer.h"
#include "GJHCamera.h"
#include "GJHGameEngineGUI.h"
#include "imgui.h"
#include <GJHGameEngineScene.h>
#include <GJHDirectRenderTarget.h>
#include <GJHTargetPostEffect.h>
#include "GJHMaskRenderer.h"
#include <GJHMaterial.h>
#include <GJHDirectStructuredBuffer.h>
#include "GJHLight.h"

std::map<int, std::shared_ptr<GJHDirectRenderTarget>> GJHRenderManagerComponent::m_MaskTarget;

GJHRenderManagerComponent::GJHRenderManagerComponent()
{

}

GJHRenderManagerComponent::~GJHRenderManagerComponent()
{
	m_LightBufferList.clear();
}

void GJHRenderManagerComponent::RenderPrev()
{
	GJHGameEngineDevice::BackBufferSetting();
}

void GJHRenderManagerComponent::RenderNext()
{

}

bool ZSort(GJHRenderer* _Left, GJHRenderer* _Right)
{
	return _Right->GetWorldPosition().z < _Left->GetWorldPosition().z;
}

void GJHRenderManagerComponent::LightUpdate(int _Order, std::vector<GJHLightData>& _LightData, GJHCamera* _Cam)
{
	_LightData.reserve(m_LightList[_Order].size());

	for (auto Light : m_LightList[_Order])
	{
		Light->m_CurCamera = _Cam;
		Light->CalLightData();
		_LightData.push_back(Light->GetLightData());
	}
}

void GJHRenderManagerComponent::Render()
{
	GJHGameEngineDevice::Reset();

	for (auto& _CamList : m_CameraList)
	{
		for (auto& _Cam : _CamList.second)
		{
			_Cam->CamTargetClear();
		}
	}

	GetScene()->DebugRender();

	GJHGameEngineDevice::Reset();

	for (auto& _CamList : m_CameraList)
	{
		for (auto& _Cam : _CamList.second)
		{
			if (_Cam->IsUpdate() == false)
			{
				continue;
			}

			GJHGameEngineDevice::Reset();

			for (const auto& _ViewIndex : _Cam->GetViewList())
			{
				std::list<GJHLight*>& GroupLightList = m_LightList[_ViewIndex];
				std::shared_ptr<GJHDirectStructuredBuffer> LightBuffer = m_LightBufferList[_ViewIndex];

				std::vector<GJHLightData> Data;
				LightUpdate(_ViewIndex, Data, _Cam);
				
				m_LightSetData.LightSetCount = static_cast<int>(Data.size());
				if (Data.size() != 0)
				{
					LightBuffer->ChangeData(&Data[0], sizeof(GJHLightData) * Data.size());
				}

				std::list<GJHRenderer*>& _RenderList = m_RendererList[_ViewIndex];

				for (auto& _Render : _RenderList)
				{
					if (_Render->IsUpdate() == false)
					{
						continue;
					}


					_Render->m_CurCamera = _Cam;
					_Render->m_pLightStructuredBuffer = LightBuffer;
					_Render->m_pLightSetData = &m_LightSetData;
					_Render->RenderingSetting();
				}

				for (const auto& PathType : _Cam->GetPathList())
				{
					std::list<std::shared_ptr<GJHRenderPlayer>>& _RenderPlayerList = m_RendererPlayerList[_ViewIndex][PathType];

					if (_RenderPlayerList.size() <= 0)
					{
						continue;
					}

					if (_Cam->CamPathTargetSetting(PathType) == false)
					{
						continue;
					}

					for (auto& _RenderPlayer : _RenderPlayerList)
					{
						if (_RenderPlayer->IsUpdate() == false)
						{
							continue;
						}

						_RenderPlayer->Render();
					}

					if (_RenderPlayerList.size() != 0)
					{
						if (PathType == OutPutPathType::Defferd)
						{
							_Cam->DefferdLightTargetClear();

							for (auto& DefferdLight : GroupLightList)
							{
								for (auto& _Render : _RenderList)
								{
									if (_Render->IsUpdate() == false)
									{
										continue;
									}

									if (_Render->GetIsShadow() == false)
									{
										continue;
									}

									_Render->CurLight = DefferdLight;
									_Render->LightRenderingSetting();
								}

								DefferdLight->GetShadowTarget()->Clear();
								DefferdLight->GetShadowTarget()->Setting();

								std::list<std::shared_ptr<GJHRenderPlayer>>& _ShadowRenderPlayerList = m_ShadowRendererPlayerList[_ViewIndex][PathType];

								for (auto& _RenderPlayer : _ShadowRenderPlayerList)
								{
									if (_RenderPlayer->IsUpdate() == false)
									{
										continue;
									}

									_RenderPlayer->Render();
								}

								_Cam->CurDefferdLight = DefferdLight;
								_Cam->OneLightData = &DefferdLight->GetLightData();
								_Cam->DefferdLightTargetProcess(PathType);
								GJHGameEngineDevice::Reset();
							}
						}
					}
				}
			}
		}
	}

	GJHGameEngineDevice::Reset();

	for (auto& _CamList : m_CameraList)
	{
		for (auto& _Cam : _CamList.second)
		{
			_Cam->CamTargetMerge();
		}
	}

	GJHGameEngineDevice::Reset();

	GJHTargetPostEffect::ProcessEffect(5);

	GJHGameEngineDevice::Reset();

	GJHGameEngineDevice::BackBufferSetting();

	for (auto& _CamList : m_CameraList)
	{
		for (auto& _Cam : _CamList.second)
		{
			GJHGameEngineDevice::GetBackBuffer()->Merge(_Cam->GetCameraTarget());
		}
	}

	GJHGameEngineDevice::Reset();
	
	GJHGameEngineGUI::GUIRenderStart();
	GetScene()->OnGUI();
	GJHGameEngineGUI::GUIWindowRender();
	GJHGameEngineGUI::GUIRenderEnd();

	GJHGameEngineDevice::Present();
}

void GJHRenderManagerComponent::PushCamera(GJHCamera* _Camera)
{
	if (m_CameraList.find(_Camera->GetOrder()) == m_CameraList.end())
	{
		m_CameraList[_Camera->GetOrder()];
	}

	m_CameraList[_Camera->GetOrder()].push_back(_Camera);
}

void GJHRenderManagerComponent::PushLight(GJHLight* _Light)
{
	std::vector<int>& ViewList = _Light->GetViewList();

	for (size_t i = 0; i < ViewList.size(); i++)
	{
		if (m_LightList.find(ViewList[i]) == m_LightList.end())
		{
			std::shared_ptr<GJHDirectStructuredBuffer> NewBuffer = std::make_shared<GJHDirectStructuredBuffer>();
			NewBuffer->Create(sizeof(GJHLightData), 512, nullptr);
			m_LightBufferList.insert(std::map<int, std::shared_ptr<GJHDirectStructuredBuffer>>::value_type(ViewList[i], NewBuffer));
		}

		m_LightList[ViewList[i]].push_back(_Light);
	}
}

void GJHRenderManagerComponent::PushMaskRenderer(GJHMaskRenderer* _Render)
{
}

void GJHRenderManagerComponent::PushRenderer(GJHRenderer* _ParentRenderer)
{
	if (m_RendererList.find(_ParentRenderer->GetOrder()) == m_RendererList.end())
	{
		m_RendererList[_ParentRenderer->GetOrder()];
	}

	if (m_LightList.find(_ParentRenderer->GetOrder()) == m_LightList.end())
	{
		std::shared_ptr<GJHDirectStructuredBuffer> NewBuffer = std::make_shared<GJHDirectStructuredBuffer>();
		NewBuffer->Create(sizeof(GJHLightData), 512, nullptr);
		m_LightBufferList.insert(std::map<int, std::shared_ptr<GJHDirectStructuredBuffer>>::value_type(_ParentRenderer->GetOrder(), NewBuffer));
	}

	m_RendererList[_ParentRenderer->GetOrder()].push_back(_ParentRenderer);
}

void GJHRenderManagerComponent::PushRenderPlayer(GJHRenderer* _ParentRenderer, std::shared_ptr<GJHRenderPlayer> _RenderPlayer)
{
	if (m_RendererPlayerList.end() == m_RendererPlayerList.find(_ParentRenderer->GetOrder()))
	{
		m_RendererPlayerList[_ParentRenderer->GetOrder()];
	}

	if (m_LightList.find(_ParentRenderer->GetOrder()) == m_LightList.end())
	{
		std::shared_ptr<GJHDirectStructuredBuffer> NewBuffer = std::make_shared<GJHDirectStructuredBuffer>();
		NewBuffer->Create(sizeof(GJHLightData), 512, nullptr);
		m_LightBufferList.insert(std::map<int, std::shared_ptr<GJHDirectStructuredBuffer>>::value_type(_ParentRenderer->GetOrder(), NewBuffer));
	}

	m_RendererPlayerList[_ParentRenderer->GetOrder()][_RenderPlayer->GetOutPutPath()].push_back(_RenderPlayer);

	if (_ParentRenderer->GetIsShadow() == true)
	{
		std::shared_ptr<GJHRenderPlayer> NewShadowPlayer = std::make_shared<GJHRenderPlayer>();

		if (_ParentRenderer->GetIsAni() == false)
		{
			NewShadowPlayer->SetParent(_ParentRenderer);
			NewShadowPlayer->SetMaterial("ShadowDepth"
				, [NewShadowPlayer, _ParentRenderer]
				{
					if (NewShadowPlayer->IsCBufferSetting(L"RenderingTransformData"))
					{
						GJHRenderingTransformData& RendererData = _ParentRenderer->GetRenderingData();
						NewShadowPlayer->CBufferSettingLink(L"RenderingTransformData", RendererData);
					}
				}
			);
			NewShadowPlayer->SetMesh(_RenderPlayer->GetMesh());
		}
		else if (_ParentRenderer->GetIsAni() == true)
		{
			NewShadowPlayer->SetParent(_ParentRenderer);
			NewShadowPlayer->SetMaterial("ShadowAniDepth"
				, [NewShadowPlayer, _ParentRenderer, _RenderPlayer]
				{

					if (_RenderPlayer->IsSBufferSetting(L"ArrAniMationMatrix"))
					{
						std::shared_ptr<GJHDirectStructuredBuffer> SBuffer = _RenderPlayer->GetSBuffer(L"ArrAniMationMatrix", ShaderType::Vertex);

						if (SBuffer == nullptr)
						{
							GJHGameEngineDebug::AssertMsg("if (SBuffer == nullptr)");
						}

						NewShadowPlayer->SBufferSettingLink(L"ArrAniMationMatrix", SBuffer);
					}

					if (NewShadowPlayer->IsCBufferSetting(L"RenderingTransformData"))
					{
						GJHRenderingTransformData& RendererData = _ParentRenderer->GetRenderingData();
						NewShadowPlayer->CBufferSettingLink(L"RenderingTransformData", RendererData);
					}
				}
			);
			NewShadowPlayer->SetMesh(_RenderPlayer->GetMesh());
		}

		NewShadowPlayer->SetParent(_RenderPlayer.get());
		m_ShadowRendererPlayerList[_ParentRenderer->GetOrder()][_RenderPlayer->GetOutPutPath()].push_back(NewShadowPlayer);
	}
}

void GJHRenderManagerComponent::Release()
{
	for (auto& _CamList : m_CameraList)
	{
		ReleaseList(_CamList.second);
	}

	for (auto& _RenderPathList : m_RendererList)
	{
		ReleaseList<GJHRenderer*>(_RenderPathList.second);
	}
}

void GJHRenderManagerComponent::CreateMaskTarget(int _MaskTargetIndex, float4 _Scale, float4 _ClearColor, DXGI_FORMAT _Fmt)
{
	if (m_MaskTarget.find(_MaskTargetIndex) != m_MaskTarget.end())
	{
		return;
	}

	m_MaskTarget[_MaskTargetIndex] = std::make_shared<GJHDirectRenderTarget>();
	m_MaskTarget[_MaskTargetIndex]->CreateRenderTarget(_Scale, _ClearColor);
}

std::shared_ptr<GJHDirectRenderTarget> GJHRenderManagerComponent::GetMask(int _MaskTargetIndex)
{
	return m_MaskTarget[_MaskTargetIndex];
}