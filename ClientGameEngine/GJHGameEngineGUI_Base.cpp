#include "GJHGameEngineGUI.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <GJHGameEngineWindow.h>
#include <GJHGameEngineDevice.h>
#include <iostream>

GJHGUIWindow::GJHGUIWindow() :
    IsOpen(true)
{

}

GJHGUIWindow::~GJHGUIWindow()
{

}

void GJHGUIWindow::OnGUI()
{

}

void GJHGUIWindow::Begin()
{
    std::string ConvertName = GetName().GetConvertMultiByteString();
    
    ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);
    bool Check = ImGui::Begin(ConvertName.c_str(), &IsOpen, GuiStyle);
}

void GJHGUIWindow::End()
{
    ImGui::End();
}

void GJHGUIWindowManager::GUIWindowRender()
{
    if (GJHGameEngineGUI::m_IsGUI == false)
    {
        return;
    }

    for (auto& _Window : m_GUIWindows)
    {
        if (_Window.second->GetIsOpen())
        {
			_Window.second->Begin();
			_Window.second->OnGUI();
            _Window.second->Update();
			_Window.second->End();
        }
    }
}

GJHGUIWindowManager GJHGameEngineGUI::m_GlobalWindowManager;

void GJHGameEngineGUI::Init()
{
    setlocale(LC_ALL, "KOR");
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    if (ImGui_ImplWin32_Init(GJHGameEngineWindow::GetWindowHWnd()) == false)
    {

    }
    ;
    if (ImGui_ImplDX11_Init(GJHGameEngineDevice::GetDevice(), GJHGameEngineDevice::GetContext()) == false)
    {

    }

    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\gulim.ttc", 16.0f, 0, io.Fonts->GetGlyphRangesKorean());
}

void GJHGameEngineGUI::Start()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void GJHGameEngineGUI::GUIRenderEnd()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void GJHGameEngineGUI::GUIRenderStart()
{
    Start();
}

void GJHGameEngineGUI::Release()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void GJHGameEngineGUI::GUIWindowRender()
{
    m_GlobalWindowManager.GUIWindowRender();
}