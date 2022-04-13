#include "PlaySceneWindow.h"

float4 PlaySceneWindow::FreeCamPos;
float4 PlaySceneWindow::FreeCamRot;
float4 PlaySceneWindow::RenderRot;

PlaySceneWindow::PlaySceneWindow()
{

}

PlaySceneWindow::~PlaySceneWindow()
{

}

void PlaySceneWindow::OnGUI()
{
	GJHGameEngineGUI::Text(L"FreeCamPos = ", FreeCamPos);
	GJHGameEngineGUI::Text(L"FreeCamRot = ", FreeCamRot);
	GJHGameEngineGUI::Text(L"RenderRot = ", RenderRot);
}