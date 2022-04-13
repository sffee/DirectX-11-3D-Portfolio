#include "GJHGlobalAppWindow.h"
#include <GJHClientGameEngine.h>
#include <GJHGameEngineTime.h>

void GJHGlobalAppWindow::OnGUI()
{
	GJHGameEngineGUI::Text(L"Frame", 1.f / GJHGameEngineTime::FDeltaTime());

	for (size_t i = 0; i < SceneName.size(); i++)
	{
		if (GJHGameEngineGUI::Button(SceneName[i]) == true)
		{
			GJHClientGameEngine::ChangeScene(SceneName[i]);
		}
	}
}