#pragma once
#include "imgui.h"
#include <GJHUpdaterBase.h>
#include <GJHGameEngineString.h>
#include <map>
#include <GJHGameEngineMemory.h>
#include <GJHNameBase.h>
#include <GJHGameEngineDebug.h>
#include <vector>
#include <GJHDirectSprite.h>

class GJHGUIWindowManager;
class GJHGUIWindow : public GJHUpdaterBase, public GJHNameBase, std::enable_shared_from_this<GJHGUIWindow>
{
public:
	enum Style
	{
		None = ImGuiWindowFlags_::ImGuiWindowFlags_None,
		NoMove = ImGuiWindowFlags_::ImGuiWindowFlags_NoMove,
		AutoSize = ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize
	};

private:
	friend GJHGUIWindowManager;

private:
	bool IsOpen;
	int GuiStyle;

public:
	bool GetIsOpen()
	{
		return IsOpen;
	}

public:
	void SetGuiStyle(int _GuiStyle)
	{
		GuiStyle = _GuiStyle;
	}

public:
	void Close()
	{
		IsOpen = false;
	}

	void Open()
	{
		IsOpen = true;
	}

public:
	virtual void OnGUI() = 0;
	virtual void Update() { };

private:
	void Begin();
	void End();

public:
	GJHGUIWindow();
	~GJHGUIWindow();
};

class GJHGUIWindowManager
{
private:
	std::map<GJHGameEngineString, std::shared_ptr<GJHGUIWindow>> m_GUIWindows;

public:
	template<typename WindowType>
	std::shared_ptr<WindowType> FindGUIWindow(const GJHGameEngineString& _Name)
	{
		if (m_GUIWindows.find(_Name) != m_GUIWindows.end())
		{
			m_GUIWindows.find(_Name)->second->Open();
			return std::dynamic_pointer_cast<WindowType>(m_GUIWindows.find(_Name)->second);
		}

		return nullptr;
	}

	template<typename WindowType>
	std::shared_ptr<WindowType> CreateGUIWindow(const GJHGameEngineString& _Name, int _Style = (int)GJHGUIWindow::Style::None)
	{
		if (m_GUIWindows.find(_Name) != m_GUIWindows.end())
		{
			m_GUIWindows.find(_Name)->second->Open();
			return std::dynamic_pointer_cast<WindowType>(m_GUIWindows.find(_Name)->second);
		}

		std::shared_ptr<WindowType> ReturnWindow = std::make_shared<WindowType>();

		m_GUIWindows[_Name] = ReturnWindow;
		m_GUIWindows[_Name]->SetGuiStyle(_Style);
		ReturnWindow->SetName(_Name);

		return ReturnWindow;
	}

public:
	void GUIWindowRender();
};

class GJHGameEngineGUI
{
public:
	static bool m_IsGUI;

	static void IsGUIOn()
	{
		m_IsGUI = true;
	}

	static void IsGUIOff()
	{
		m_IsGUI = false;
	}

private:
	static void Start();

public:
	static void Release();
	static void Init();
	static void GUIRenderStart();
	static void GUIRenderEnd();

private:
	static GJHGUIWindowManager m_GlobalWindowManager;

public:
	template<typename WindowType>
	static std::shared_ptr<WindowType> CreateGUIWindow(const GJHGameEngineString& _Name, int _Style = 0)
	{
		return m_GlobalWindowManager.CreateGUIWindow<WindowType>(_Name, _Style);
	}

	template<typename WindowType>
	static std::shared_ptr<WindowType> FindGUIWindow(const GJHGameEngineString& _Name)
	{
		return m_GlobalWindowManager.FindGUIWindow<WindowType>(_Name);
	}

private:
	GJHGameEngineGUI();
	~GJHGameEngineGUI();

public:
	static void GUIWindowRender();

private:
	static std::vector<const char*> GameEngineStringToConstChar(const std::vector<GJHGameEngineString>& _StringVec);

public:
	template<typename ... REST>
	static void Text(REST ... _Arg)
	{
		Text(GJHGameEngineString::TextCreate(_Arg...));
	}

	static void Text(const GJHGameEngineString& _GameEngine);
	static void TextColored(const float4& _Color, const GJHGameEngineString& _Text);
	static bool Button(const GJHGameEngineString& _ButtonName, const float4& _Size = { 0, 0 });
	static bool TextList(const wchar_t* _Label, int& _CurSelect, std::vector<GJHGameEngineString>& _List, float _Width, void(*)(const GJHGameEngineString&) = nullptr);
	static bool TextList(const wchar_t* _Label, int& _CurSelect, std::vector<GJHGameEngineString> _List, float _Width, int _Height, void(*_Func)(const GJHGameEngineString&) = nullptr);
	static void Image(int Id, std::shared_ptr<class GJHDirectTexture2D> _Text, const float4& _Size, const float4& _Start = float4::ZERO, const float4& _End = float4::ONE);
	static bool ImageButton(int Id, std::shared_ptr<class GJHDirectTexture2D> _Text, const float4& _Size, const float4& _Start = float4::ZERO, const float4& _End = float4::ONE);
	static bool Combo(const GJHGameEngineString& _Label, int& _CurSelect, std::vector<GJHGameEngineString> _Vec, void(*_Func)(const GJHGameEngineString&) = nullptr);
	static bool RadioButton(const GJHGameEngineString& _Label, int* _CurSelect, int _ButtonIndex);
	static void AddLine(float4 _Start, float4 _End, float4 _Color, float thickness = 1.f);
	static void AddRect(float4 _Start, float4 _End, float4 _Color, float thickness = 1.f);
	static void InputText(const wchar_t* _Label, GJHGameEngineString& _Text, int BufferSize = 1);
	static ImGuiTextFilter Filter(const GJHGameEngineString& _Label, std::vector<GJHGameEngineString> _Vec);
	static void Filter(std::vector<GJHGameEngineString> _Vec, const ImGuiTextFilter& _Filter);
	static void BeginChild(const GJHGameEngineString& _Id, const float4& _Size, bool _Border = false, ImGuiWindowFlags _Flags = 0);
	static void EndChild();
	static bool BeginTabBar(const GJHGameEngineString& _Id, int _Flag = ImGuiTabBarFlags_::ImGuiTabBarFlags_None);
	static bool BeginTabItem(const GJHGameEngineString& _Label);
	static void EndTabItem();
	static void EndTabBar();

	template<typename ... REST>
	static bool TreeNode(REST ... _Arg)
	{
		return TreeNode(GJHGameEngineString::TextCreate(_Arg...));
	}

	static bool TreeNode(const GJHGameEngineString& _Text);
	static void TreePop();

	static float4 GetCursorScreenPos();
	static float4 GetMousePos();
	static float GetWindowContentRegionWidth();
	static void SetNextItemWidth(float _Value);
	static void PushItemWidth(float _Value);
	static void PopItemWidth();
	static void NewLine();
	static void SameLine();

	static void ProgressBar(float _Value);
};