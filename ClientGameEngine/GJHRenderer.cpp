#include "GJHRenderer.h"
#include <GJHGameEngineActor.h>
#include <GJHGameEngineDebug.h>
#include "GJHCamera.h"
#include <GJHGameEngineScene.h>
#include "GJHRenderManagerComponent.h"
#include "GJHActorTransform.h"
#include <GJHDirectFbx.h>
#include "GJHLight.h"

GJHRenderer::GJHRenderer() :
	m_pActorTransform(nullptr),
	m_ChangeOrder(false),
	m_ShadowOn(true),
	m_AniOn(false)
{

}

GJHRenderer::~GJHRenderer()
{

}

std::shared_ptr<GJHRenderPlayer> GJHRenderer::CreateRenderPlayer(std::shared_ptr<GJHDirectMesh> _Mesh)
{
	std::shared_ptr<GJHRenderPlayer> NewRenderPlayer = std::make_shared<GJHRenderPlayer>();
	NewRenderPlayer->SetMesh(_Mesh);
	m_PlayerList.push_back(NewRenderPlayer);

	return NewRenderPlayer;
}

std::shared_ptr<GJHRenderPlayer> GJHRenderer::CreateRenderPlayer(const GJHGameEngineString& _Material, std::shared_ptr<GJHDirectMesh> _Mesh)
{
	std::shared_ptr<GJHRenderPlayer> NewRenderPlayer = std::make_shared<GJHRenderPlayer>();
	NewRenderPlayer->SetParent(this);

	NewRenderPlayer->SetMaterial(_Material
		, [NewRenderPlayer, this]
		{
			if (NewRenderPlayer->IsCBufferSetting(L"RenderingTransformData"))
			{
				GJHRenderingTransformData& RendererData = GetRenderingData();
				NewRenderPlayer->CBufferSettingLink(L"RenderingTransformData", RendererData);
			}

			if (NewRenderPlayer->IsCBufferSetting(L"RenderOption"))
			{
				NewRenderPlayer->CBufferSettingLink(L"RenderOption", Option);
			}
		});

	NewRenderPlayer->SetMesh(_Mesh);

	m_PlayerList.push_back(NewRenderPlayer);

	std::shared_ptr<GJHRenderManagerComponent> Ptr = GetScene()->FindComponent<GJHRenderManagerComponent>(L"RenderManager");
	Ptr->PushRenderPlayer(this, NewRenderPlayer);

	return NewRenderPlayer;

}

std::shared_ptr<GJHRenderPlayer> GJHRenderer::CreateRenderPlayer(const GJHGameEngineString& _Material, const GJHGameEngineString& _Mesh)
{
	std::shared_ptr<GJHRenderPlayer> NewRenderPlayer = std::make_shared<GJHRenderPlayer>();
	NewRenderPlayer->SetParent(this);

	NewRenderPlayer->SetMaterial(_Material
		, [NewRenderPlayer, this]
		{
			if (NewRenderPlayer->IsCBufferSetting(L"RenderingTransformData"))
			{
				GJHRenderingTransformData& RendererData = GetRenderingData();
				NewRenderPlayer->CBufferSettingLink(L"RenderingTransformData", RendererData);
			}

			if (NewRenderPlayer->IsCBufferSetting(L"RenderOption"))
			{
				NewRenderPlayer->CBufferSettingLink(L"RenderOption", Option);
			}
		});

	NewRenderPlayer->SetMesh(_Mesh);

	if (NewRenderPlayer->IsCBufferSetting(L"RenderingTransformData"))
	{
		GJHRenderingTransformData& RendererData = GetRenderingData();
		NewRenderPlayer->CBufferSettingLink(L"RenderingTransformData", RendererData);
	}

	m_PlayerList.push_back(NewRenderPlayer);

	std::shared_ptr<GJHRenderManagerComponent> Ptr = GetScene()->FindComponent<GJHRenderManagerComponent>(L"RenderManager");
	Ptr->PushRenderPlayer(this, NewRenderPlayer);

	return NewRenderPlayer;
}

void GJHRenderer::SetMaterial(const GJHGameEngineString& _Name, size_t _Index)
{
	std::shared_ptr<GJHRenderPlayer> SettingRenderPlayer = m_PlayerList[_Index];

	SettingRenderPlayer->SetMaterial(_Name,
		[SettingRenderPlayer, this]
		{
			if (SettingRenderPlayer->IsCBufferSetting(L"RenderingTransformData"))
			{
				GJHRenderingTransformData& RendererData = GetRenderingData();
				SettingRenderPlayer->CBufferSettingLink(L"RenderingTransformData", RendererData);
			}

			if (SettingRenderPlayer->IsCBufferSetting(L"RenderOption"))
			{
				SettingRenderPlayer->CBufferSettingLink(L"RenderOption", Option);
			}
		}
		);
}

void GJHRenderer::Start(int _Order)
{
	SetOrder(_Order);

	{
		GJHActorTransform::Start();

		ActorTransPtr Trans = GetActor()->FindComponent<GJHActorTransform>();

		if (Trans == nullptr)
		{
			GJHGameEngineDebug::AssertMsg("if (Trans == nullptr)");
		}

		this->AttachParent(Trans);

		std::shared_ptr<GJHRenderManagerComponent> Ptr = GetScene()->FindComponent<GJHRenderManagerComponent>(L"RenderManager");

		Ptr->PushRenderer(this);
	}
}

void GJHRenderer::Start(int _Order, const GJHGameEngineString& _Material, const GJHGameEngineString& _Mesh)
{
	CreateRenderPlayer(_Material, _Mesh);
	Start(_Order);
}

void GJHRenderer::RenderingSetting()
{
	GJHRenderingTransformData& CameraData = m_CurCamera->GetCameraTransformMatrix();
	CalRenderDataCopy();

	GJHRenderingTransformData& RendererData = GetRenderingData();
	RendererData.View = CameraData.View;
	RendererData.Projection = CameraData.Projection;
	RendererData.CalRenderingMatrix();
	RendererData.CamPos = m_CurCamera->GetActorTrans()->GetWorldPosition() * RendererData.View;

	for (size_t i = 0; i < m_PlayerList.size(); i++)
	{
		if (m_PlayerList[i]->IsSBufferSetting("ArrLightData") == true)
		{
			m_PlayerList[i]->SBufferSettingLink("ArrLightData", m_pLightStructuredBuffer);
			m_PlayerList[i]->CBufferSettingLink("LightSetData", *m_pLightSetData);
		}
	}
}

void GJHRenderer::LightRenderingSetting()
{
	GJHRenderingTransformData& RendererData = GetRenderingData();
	RendererData.View = CurLight->GetLightData().View;
	RendererData.Projection = CurLight->GetLightData().Proj;
	RendererData.CalRenderingMatrix();
}

void GJHRenderer::TextureSetting(const GJHGameEngineString& _Name, std::shared_ptr<GJHDirectTexture2D> _Res, bool _AutoSize, int _RenderPlayerIndex)
{
	m_PlayerList[_RenderPlayerIndex]->TextureSetting(_Name, _Res);

	if (_AutoSize == true)
	{
		SetWorldScale(GetTexture()->GetSize());
	}
}

void GJHRenderer::TextureSetting(const GJHGameEngineString& _Name, const GJHGameEngineString& _TexName, bool _AutoSize, int _RenderPlayerIndex)
{
	m_PlayerList[_RenderPlayerIndex]->TextureSetting(_Name, _TexName);

	if (_AutoSize == true)
	{
		SetWorldScale(GetTexture()->GetSize());
	}
}

void GJHRenderer::TextureSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, const GJHGameEngineString& _TexName, bool _AutoSize, int _RenderPlayerIndex)
{
	m_PlayerList[_RenderPlayerIndex]->TextureSetting(_Name, _ShaderType, _TexName);

	if (_AutoSize == true)
	{
		SetWorldScale(GetTexture()->GetSize());
	}
}

void GJHRenderer::TextureSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, std::shared_ptr<GJHDirectTexture2D> _Res, bool _AutoSize, int _RenderPlayerIndex)
{
	m_PlayerList[_RenderPlayerIndex]->TextureSetting(_Name, _ShaderType, _Res);

	if (_AutoSize == true)
	{
		SetWorldScale(GetTexture()->GetSize());
	}
}

void GJHRenderer::SamplerSetting(const GJHGameEngineString& _Name, const GJHGameEngineString& _SmpName, int _RenderPlayerIndex)
{
	m_PlayerList[_RenderPlayerIndex]->SamplerSetting(_Name, _SmpName);
}

void GJHRenderer::SamplerSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, const GJHGameEngineString& _SmpName, int _RenderPlayerIndex)
{
	m_PlayerList[_RenderPlayerIndex]->SamplerSetting(_Name, _ShaderType, _SmpName);
}

void GJHRenderer::SamplerSetting(const GJHGameEngineString& _Name, ShaderType _ShaderType, std::shared_ptr<GJHDirectSampler> _Res, int _RenderPlayerIndex)
{
	m_PlayerList[_RenderPlayerIndex]->SamplerSetting(_Name, _ShaderType, _Res);
}

std::vector<std::shared_ptr<GJHDirectTexture2D>> GJHRenderer::GetTexture(const GJHGameEngineString& _Name, int _RenderPlayerIndex)
{
	return m_PlayerList[_RenderPlayerIndex]->GetTexture(_Name);
}

std::shared_ptr<GJHDirectTexture2D> GJHRenderer::GetTexture(const GJHGameEngineString& _Name, ShaderType _ShaderType, int _RenderPlayerIndex)
{
	return m_PlayerList[_RenderPlayerIndex]->GetTexture(_Name, _ShaderType);
}

std::shared_ptr<GJHRenderPlayer> GJHRenderer::GetRenderPlayer(size_t _Index)
{
	return m_PlayerList[_Index];
}

void GJHRenderer::SetVertexShader(const GJHGameEngineString& _Name, size_t _Index)
{
	m_PlayerList[_Index]->SetVertexShader(_Name);
}

void GJHRenderer::SetGeometryShader(const GJHGameEngineString& _Name, size_t _Index)
{
	m_PlayerList[_Index]->SetGeometryShader(_Name);
}

void GJHRenderer::SetPixelShader(const GJHGameEngineString& _Name, size_t _Index)
{
	m_PlayerList[_Index]->SetPixelShader(_Name);
}

void GJHRenderer::SetRasterizer(const GJHGameEngineString& _Name, size_t _Index)
{
	m_PlayerList[_Index]->SetRasterizer(_Name);
}

void GJHRenderer::SetDepthStencil(const GJHGameEngineString& _Name, size_t _Index)
{
	m_PlayerList[_Index]->SetDepthStencil(_Name);
}

void GJHRenderer::SetBlend(const GJHGameEngineString& _Name, size_t _Index)
{
	m_PlayerList[_Index]->SetBlend(_Name);
}

void GJHRenderer::SetMesh(const GJHGameEngineString& _Name, size_t _Index)
{
	m_PlayerList[_Index]->SetMesh(_Name);
}

std::vector<std::shared_ptr<GJHRenderPlayer>> GJHRenderer::SetFbxToCreateAllRenderPlayer(const GJHGameEngineString& _Name, const GJHGameEngineString& _MatName, int _Order, std::function<void(GJHRenderPlayer*)> _ResultPtr)
{
	std::vector<std::shared_ptr<GJHRenderPlayer>> Return;

	std::shared_ptr<GJHDirectFbx> FbxMesh = GJHDirectFbx::Find(_Name);

	for (int RenderSetIndex = 0; RenderSetIndex < FbxMesh->GetRenderSetCount(); RenderSetIndex++)
	{
		int MeshSetCount = FbxMesh->GetMeshSetCount(RenderSetIndex);

		for (int MeshSetIndex = 0; MeshSetIndex < MeshSetCount; MeshSetIndex++)
		{
			int MeshCount = FbxMesh->GetMeshCount(RenderSetIndex, MeshSetIndex);

			for (int MeshIndex = 0; MeshIndex < MeshCount; MeshIndex++)
			{
				std::shared_ptr<GJHDirectMesh> NewMesh = FbxMesh->GetMesh(MeshSetIndex, MeshIndex);

				if (nullptr != NewMesh)
				{
					std::shared_ptr<GJHRenderPlayer> Player = CreateRenderPlayer(NewMesh);

					Player->SetMaterial(_MatName,
						[Player, this] 
						{
							if (Player->IsCBufferSetting(L"RenderingTransformData"))
							{
								GJHRenderingTransformData& RendererData = GetRenderingData();
								Player->CBufferSettingLink(L"RenderingTransformData", RendererData);
							}

							if (Player->IsCBufferSetting(L"RenderOption"))
							{
								Player->CBufferSettingLink(L"RenderOption", Option);
							}
						}
						);

					if (_ResultPtr != nullptr)
					{
						_ResultPtr(Player.get());
					}

					Return.push_back(Player);

					std::shared_ptr<GJHRenderManagerComponent> Ptr = GetScene()->FindComponent<GJHRenderManagerComponent>(L"RenderManager");
					Ptr->PushRenderPlayer(this, Player);

					FbxExFaceSettingData* GetMatData = FbxMesh->GetMaterialData(RenderSetIndex, MeshSetIndex, MeshIndex);
					if (Player->IsTextureSetting(L"DiffuseTexture") == true
						&& GetMatData->DifTextureName.IsEmpty() == false
						&& GetMatData->DifTextureName != L"")
					{
						Player->TextureSetting(L"DiffuseTexture", GetMatData->DifTextureName);
					}

					if (Player->IsTextureSetting(L"BumpTexture") == true
						&& GetMatData->BmpTextureName.IsEmpty() == false
						&& GetMatData->BmpTextureName != L"")
					{
						Player->TextureSetting(L"BumpTexture", GetMatData->BmpTextureName);
						Option.bBump = 1;
					}
				}
			}
		}
	}

	return Return;
}