#include "RenderingWindow.h"
#include <GJHClientGameEngine.h>
#include <GJHGameEngineTime.h>
#include <GJHSceneManager.h>
#include <GJHRenderManagerComponent.h>
#include <GJHCamera.h>
#include <GJHLight.h>
#include <GJHDirectRenderTarget.h>
#include <GJHDirectTexture2D.h>


void RenderTargetWindow::OnGUI()
{
	if (m_RenderTarget != nullptr)
	{
		float4 WindowSize = GJHGameEngineWindow::GetSize();
		if (GJHGameEngineGUI::ImageButton(m_Index, m_RenderTarget->GetTex(m_Index), WindowSize) == true)
		{
			Close();
		}
	}
}

void RenderingWindow::OnGUI()
{
	std::shared_ptr<GJHGameEngineScene> Scene = GJHClientGameEngine::GetCurScene();

	if (Scene == nullptr)
	{
		return;
	}

	std::shared_ptr<GJHRenderManagerComponent> RenderCom = Scene->FindComponent<GJHRenderManagerComponent>();

	if (RenderCom == nullptr)
	{
		GJHGameEngineGUI::Text(L"랜더 매니저 컴포넌트를 찾지 못했습니다");
		return;
	}

	float4 WindowSize = GJHGameEngineWindow::GetSize();
	float4 WindowButtonSize = WindowSize * 0.1f;
	{
		std::map<int, std::list<GJHCamera*>> CamList = RenderCom->GetCameraList();

		for (auto& CamGroup : CamList)
		{
			if (true == GJHGameEngineGUI::TreeNode(L"카메라 그룹 : ", CamGroup.first))
			{
				int Count = 0;
				for (auto& Cam : CamGroup.second)
				{
					if (true == GJHGameEngineGUI::TreeNode(L"카메라", Count++))
					{
						for (size_t i = 0; i < Cam->GetPathList().size(); i++)
						{
							std::shared_ptr<GJHDirectRenderTarget> Target = Cam->GetLightTarget(Cam->GetPathList()[i]);

							if (Target == nullptr)
							{
								continue;
							}

							for (int j = 0; j < Target->GetTexsize(); j++)
							{
								if (Target->GetTex(j) == nullptr)
								{
									continue;
								}

								if (GJHGameEngineGUI::ImageButton
								(
									static_cast<int>(j),
									Target->GetTex(j),
									WindowButtonSize
								) == true)
								{
									std::shared_ptr<RenderTargetWindow> RWindow = GJHGameEngineGUI::CreateGUIWindow<RenderTargetWindow>(L"카메라 메인랜더타겟", GJHGUIWindow::Style::AutoSize);
									RWindow->SetRenderTarget(Target.get(), static_cast<int>(j));
								}
							}

						}

						for (size_t i = 0; i < Cam->GetPathList().size(); i++)
						{
							std::shared_ptr<GJHDirectRenderTarget> Target = Cam->GetPathTarget(Cam->GetPathList()[i]);

							if (Target == nullptr)
							{
								continue;
							}

							for (int j = 0; j < Target->GetTexsize(); j++)
							{
								if (Target->GetTex(j) == nullptr)
								{
									continue;
								}

								if (GJHGameEngineGUI::ImageButton
								(
									static_cast<int>(j),
									Target->GetTex(j),
									WindowButtonSize
								) == true)
								{
									std::shared_ptr<RenderTargetWindow> RWindow = GJHGameEngineGUI::CreateGUIWindow<RenderTargetWindow>(L"카메라 메인랜더타겟", GJHGUIWindow::Style::AutoSize);
									RWindow->SetRenderTarget(Target.get(), static_cast<int>(j));
								}
							}
						}

						for (int i = 0; i < Cam->GetCameraTarget()->GetTexsize(); i++)
						{
							if (GJHGameEngineGUI::ImageButton
							(
								static_cast<int>(i),
								Cam->GetCameraTarget()->GetTex(i),
								WindowButtonSize
							) == true)
							{
								std::shared_ptr<RenderTargetWindow> RWindow = GJHGameEngineGUI::CreateGUIWindow<RenderTargetWindow>(L"카메라 메인랜더타겟", GJHGUIWindow::Style::AutoSize);
								RWindow->SetRenderTarget(Cam->GetCameraTarget().get(), static_cast<int>(i));
							}
						}

						GJHGameEngineGUI::TreePop();
					}
				}
				GJHGameEngineGUI::TreePop();
			}
		}
	}

	{
		std::map<int, std::list<GJHLight*>> LightList = RenderCom->GetLightList();

		for (auto& LightGroup : LightList)
		{
			if (GJHGameEngineGUI::TreeNode(L"Light Group : ", LightGroup.first) == true)
			{
				for (GJHLight* Light : LightGroup.second)
				{
					if (Light == nullptr)
					{
						continue;
					}

					if (GJHGameEngineGUI::ImageButton
					(
						static_cast<int>(reinterpret_cast<int>(Light)),
						Light->GetShadowTarget()->GetTex(0),
						WindowButtonSize
					) == true)
					{
						std::shared_ptr<RenderTargetWindow> RWindow = GJHGameEngineGUI::CreateGUIWindow<RenderTargetWindow>(L"Light Shadow Target", GJHGUIWindow::Style::AutoSize);
						RWindow->SetRenderTarget(Light->GetShadowTarget().get(), 0);
					}
				}

				GJHGameEngineGUI::TreePop();
			}
		}
	}

	for (int i = 0; i < OtherTarget.size(); i++)
	{
		if (GJHGameEngineGUI::TreeNode(OtherTargetInfo[i].Name.c_str()) == true)
		{
			if (GJHGameEngineGUI::ImageButton
			(

				i, OtherTarget[i]->GetTex(0),
				OtherTargetInfo[i].Size
			) == true)
			{
				std::shared_ptr<RenderTargetWindow> RWindow =
					GJHGameEngineGUI::CreateGUIWindow<RenderTargetWindow>(OtherTargetInfo[i].Name.c_str(), GJHGUIWindow::Style::AutoSize);
				RWindow->SetRenderTarget(OtherTarget[i].get(), 0);
			}
			GJHGameEngineGUI::TreePop();
		}
	}
}


std::vector<RenderingWindow::TargetInfo> RenderingWindow::OtherTargetInfo;
std::vector<std::shared_ptr<GJHDirectRenderTarget>> RenderingWindow::OtherTarget;