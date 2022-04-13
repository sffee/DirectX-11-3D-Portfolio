#include "GJHGameEngineGUI.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <GJHGameEngineWindow.h>
#include <GJHGameEngineDevice.h>
#include <GJHDirectTexture2D.h>

bool GJHGameEngineGUI::m_IsGUI = true;

void GJHGameEngineGUI::Text(const GJHGameEngineString& _Text)
{
    ImGui::Text(_Text.GetConvertMultiByteStringUTF8().c_str());
}

void GJHGameEngineGUI::TextColored(const float4& _Color, const GJHGameEngineString& _Text)
{
    ImGui::TextColored(ImColor(ImVec4(_Color.r, _Color.g, _Color.b, _Color.a)), _Text.GetConvertMultiByteStringUTF8().c_str());
}

bool GJHGameEngineGUI::Button(const GJHGameEngineString& _ButtonName, const float4& _Size)
{
    return ImGui::Button(_ButtonName.GetConvertMultiByteStringUTF8().c_str(), { _Size.x, _Size.y });
}

GJHGameEngineGUI::GJHGameEngineGUI()
{

}

GJHGameEngineGUI::~GJHGameEngineGUI()
{

}

std::vector<const char*> GJHGameEngineGUI::GameEngineStringToConstChar(const std::vector<GJHGameEngineString>& _StringVec)
{
    static std::vector<std::string> StringVec;
    StringVec.clear();

    for (size_t i = 0; i < _StringVec.size(); i++)
    {
        StringVec.push_back(_StringVec[i].GetConvertMultiByteString());
    }

    std::vector<const char*> CharVec;

    for (size_t i = 0; i < StringVec.size(); i++)
    {
        CharVec.push_back(StringVec[i].c_str());
    }

    return CharVec;
}

bool GJHGameEngineGUI::TextList(const wchar_t* _Label, int& _CurSelect, std::vector<GJHGameEngineString>& _List, float _Width, void(*_Ptr)(const GJHGameEngineString&))
{
    return TextList(_Label, _CurSelect, _List, _Width, 10, _Ptr);
}

bool GJHGameEngineGUI::TextList(const wchar_t* _Label, int& _CurSelect, std::vector<GJHGameEngineString> _List, float _Width, int _Height, void(*_Func)(const GJHGameEngineString&))
{
    ImGui::PushItemWidth(_Width);
    ImGui::PushID(_Label);

    int CheckIndex = _CurSelect;
    std::vector<std::string> m_List;

    for (size_t i = 0; i < _List.size(); i++)
    {
        m_List.push_back(_List[i].GetConvertMultiByteString());
    }

    std::vector<const char*> PtrArr;
    for (size_t i = 0; i < m_List.size(); i++)
    {
        PtrArr.push_back(m_List[i].c_str());
    }

    bool Check;
    if (PtrArr.size() != 0)
    {
        Check = ImGui::ListBox("", &CheckIndex, &PtrArr[0], (int)PtrArr.size(), _Height);
    }
    else
    {
        Check = ImGui::ListBox("", &CheckIndex, nullptr, 0, _Height);
    }

    if (_CurSelect != CheckIndex && _Func != nullptr)
    {
        _Func(_List[CheckIndex]);
    }

    _CurSelect = CheckIndex;

    ImGui::PopID();

    return Check;
}

void GJHGameEngineGUI::Image(int Id, std::shared_ptr<GJHDirectTexture2D> _Texture, const float4& _Size, const float4& _Start, const float4& _End)
{
    ImGui::PushID(Id);

    ImGui::Image(reinterpret_cast<ImTextureID>(_Texture->GetShaderResourceView()), { _Size.x , _Size.y }, { _Start.x , _Start.y }, { _End.x , _End.y });

    ImGui::PopID();
}

bool GJHGameEngineGUI::ImageButton(int Id, std::shared_ptr<GJHDirectTexture2D> _Texture, const float4& _Size, const float4& _Start, const float4& _End)
{
    ImGui::PushID(Id);

    bool Check = ImGui::ImageButton(reinterpret_cast<ImTextureID>(_Texture->GetShaderResourceView()), { _Size.x , _Size.y }, { _Start.x , _Start.y }, { _End.x , _End.y });

    ImGui::PopID();

    return Check;
}

bool GJHGameEngineGUI::Combo(const GJHGameEngineString& _Label, int& _CurSelect, std::vector<GJHGameEngineString> _Vec, void(*_Func)(const GJHGameEngineString&))
{
    int CheckIndex = _CurSelect;

    std::vector<const char*> Arr = GameEngineStringToConstChar(_Vec);

    Text(_Label);

    std::string Str = GJHGameEngineString(GJHGameEngineString(L"##") + _Label).GetConvertMultiByteString();
    const char* Label = Str.c_str();
    bool Check = ImGui::Combo(Str.c_str(), &CheckIndex, &Arr[0], (int)Arr.size());

    if (CheckIndex != _CurSelect && _Func != nullptr)
    {
        _Func(_Vec[CheckIndex]);
    }

    _CurSelect = CheckIndex;

    return Check;
}

bool GJHGameEngineGUI::RadioButton(const GJHGameEngineString& _Label, int* _CurSelect, int _ButtonIndex)
{
    return ImGui::RadioButton(_Label.GetConvertMultiByteStringUTF8().c_str(), _CurSelect, _ButtonIndex);
}

void GJHGameEngineGUI::AddLine(float4 _Start, float4 _End, float4 _Color, float thickness)
{
    ImGui::GetWindowDrawList()->AddLine(ImVec2(_Start.x, _Start.y), ImVec2(_End.x, _End.y), ImColor(ImVec4(_Color.r, _Color.g, _Color.b, _Color.a)), thickness);
}

void GJHGameEngineGUI::AddRect(float4 _Start, float4 _End, float4 _Color, float thickness)
{
    ImGui::GetWindowDrawList()->AddRect(ImVec2(_Start.x, _Start.y), ImVec2(_End.x, _End.y), ImColor(ImVec4(_Color.r, _Color.g, _Color.b, _Color.a)), 0.f, 0, thickness);
}

ImGuiTextFilter GJHGameEngineGUI::Filter(const GJHGameEngineString& _Label, std::vector<GJHGameEngineString> _Vec)
{
    ImGuiTextFilter Filter;

    std::vector<const char*> Arr = GameEngineStringToConstChar(_Vec);
    for (size_t i = 0; i < Arr.size(); i++)
    {
        if (Filter.PassFilter(Arr[i]))
        {
            ImGui::Text("%s", Arr[i]);
        }
    }

    return Filter;
}

void GJHGameEngineGUI::Filter(std::vector<GJHGameEngineString> _Vec, const ImGuiTextFilter& _Filter)
{
    std::vector<const char*> Arr = GameEngineStringToConstChar(_Vec);
    for (size_t i = 0; i < Arr.size(); i++)
    {
        if (_Filter.PassFilter(Arr[i]))
        {
            ImGui::Text("%s", Arr[i]);
        }
    }
}

void GJHGameEngineGUI::BeginChild(const GJHGameEngineString& _Id, const float4& _Size, bool _Border, ImGuiWindowFlags _Flags)
{
    ImGui::BeginChild(_Id.GetConvertMultiByteStringUTF8().c_str() , ImVec2(_Size.x, _Size.y), _Border, _Flags);
}

void GJHGameEngineGUI::EndChild()
{
    ImGui::EndChild();
}

bool GJHGameEngineGUI::BeginTabBar(const GJHGameEngineString& _Id, int _Flag)
{
    if (ImGui::BeginTabBar(_Id.GetConvertMultiByteString().c_str(), _Flag))
    {
        return true;
    }

    return false;
}

bool GJHGameEngineGUI::BeginTabItem(const GJHGameEngineString& _Label)
{
    if (ImGui::BeginTabItem(_Label.GetConvertMultiByteStringUTF8().c_str()))
    {
        return true;
    }

    return false;
}

void GJHGameEngineGUI::EndTabItem()
{
    ImGui::EndTabItem();
}

void GJHGameEngineGUI::EndTabBar()
{
    ImGui::EndTabBar();
}

float4 GJHGameEngineGUI::GetCursorScreenPos()
{
    ImVec2 CursorPos = ImGui::GetCursorScreenPos();

    return float4(CursorPos.x, CursorPos.y);
}

float4 GJHGameEngineGUI::GetMousePos()
{
    ImVec2 WindowPos = ImGui::GetWindowPos();
    ImVec2 MousePos = ImGui::GetMousePos();

    float4 Pos = float4(MousePos.x - WindowPos.x, MousePos.y - WindowPos.y);

    return Pos;
}

float GJHGameEngineGUI::GetWindowContentRegionWidth()
{
    return ImGui::GetWindowContentRegionWidth();
}

void GJHGameEngineGUI::SetNextItemWidth(float _Value)
{
    ImGui::SetNextItemWidth(_Value);
}

void GJHGameEngineGUI::PushItemWidth(float _Value)
{
    ImGui::PushItemWidth(_Value);
}

void GJHGameEngineGUI::PopItemWidth()
{
    ImGui::PopItemWidth();
}

void GJHGameEngineGUI::NewLine()
{
    ImGui::NewLine();
}

void GJHGameEngineGUI::SameLine()
{
    ImGui::SameLine();
}

void GJHGameEngineGUI::InputText(const wchar_t* _Label, GJHGameEngineString& _Text, int BufferSize)
{
    ImGui::PushID(_Label);
    std::string ConvertText;
    ConvertText = _Text.GetConvertMultiByteString();
    ConvertText.resize(2048);
    Text(_Label);
    GJHGameEngineGUI::SameLine();
    ImGui::InputText("", &ConvertText[0], 2048);
    _Text.SetConvertMultiByteString(ConvertText.c_str());
    ImGui::PopID();
}

bool GJHGameEngineGUI::TreeNode(const GJHGameEngineString& _Text)
{
    std::string ConvertText;
    ConvertText = _Text.GetConvertMultiByteStringUTF8();
    return ImGui::TreeNode(ConvertText.c_str());
}

void GJHGameEngineGUI::TreePop()
{
    ImGui::TreePop();
}

void GJHGameEngineGUI::ProgressBar(float _Value)
{
    ImGui::ProgressBar(_Value);
}