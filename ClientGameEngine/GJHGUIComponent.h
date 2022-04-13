#pragma once
#include <GJHGameEngineComponent.h>
#include "GJHGameEngineGUI.h"

class GJHGUIComponent : public GJHGameEngineComponent
{
private:
	GJHGUIWindowManager Manager;

public:
	template<typename WindowType>
	std::shared_ptr<WindowType> CreateGUIWindow()
	{
		GJHGameEngineString Text;
		const type_info& Data = typeid(WindowType);
		Text.SetConvertMultiByteString(Data.name());
		return Manager.CreateGUIWindow<WindowType>(Text);
	}

	template<typename WindowType>
	std::shared_ptr<WindowType> CreateGUIWindow(const GJHGameEngineString& _Name)
	{
		return Manager.CreateGUIWindow<WindowType>(_Name);
	}

	template<typename WindowType>
	std::shared_ptr<WindowType> FindGUIWindow(const GJHGameEngineString& _Name)
	{
		return Manager.FindGUIWindow<WindowType>(_Name);
	}

public:
	GJHGUIComponent();
	~GJHGUIComponent();

public:
	GJHGUIComponent(const GJHGUIComponent& _Other) = delete;
	GJHGUIComponent(const GJHGUIComponent&& _Other) = delete;

public:
	GJHGUIComponent operator=(const GJHGUIComponent& _Other) = delete;
	GJHGUIComponent operator=(const GJHGUIComponent&& _Other) = delete;

public:
	void OnGUI() override;
};