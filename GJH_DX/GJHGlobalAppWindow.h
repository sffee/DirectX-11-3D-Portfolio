#pragma once
#include <GJHGameEngineGUI.h>
#include <vector>
#include <GJHGameEngineWindow.h>

class GJHGlobalAppWindow : public GJHGUIWindow
{
public:
	std::vector<GJHGameEngineString> SceneName;

public:
	void OnGUI() override;
};