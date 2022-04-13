#include "BasePre.h"
#include "GJHGameEngineInput.h"
#include "GJHGameEngineWindow.h"

GJHGameEngineInput GJHGameEngineInput::Inst;
bool GJHGameEngineInput::m_IsMouseMoveToCenter = false;

bool GJHGameEngineInput::GJHGameEngineKey::KeyCheck()
{
	for (size_t i = 0; i < m_CheckKey.size(); i++)
	{
		if (GetAsyncKeyState(m_CheckKey[i]) == 0)
		{
			return false;
		}
	}

	return true;
}

void GJHGameEngineInput::GJHCommandKey::Update() 
{
    if (m_Key.size() <= 1)
    {
        return;
    }

    if (m_CurCheckKey.size() == 0)
    {
        m_CurCheckKey = m_Key;
        m_CurWaitNext = m_WaitNext;
        IsResult = false;
    }

    if (m_Key.size() != m_CurCheckKey.size())
    {
        m_CurWaitNext.front() -= GJHGameEngineTime::FDeltaTime();

        if (m_CurWaitNext.front() <= 0)
        {
            m_CurCheckKey = m_Key;
            m_CurWaitNext = m_WaitNext;
        }

        m_CurCheckKey.front()->Update();
        if (m_CurCheckKey.size() == 1)
        {
            if (m_CurCheckKey.front()->m_Press == true)
            {
                m_CurCheckKey.pop_front();
                m_CurWaitNext.pop_front();
            }
        }
        else
        {
            if (m_CurCheckKey.front()->m_Up == true)
            {
                m_CurCheckKey.pop_front();
                m_CurWaitNext.pop_front();
            }
        }
    }
    else if (m_Key.size() == m_CurCheckKey.size())
    {
        m_CurCheckKey.front()->Update();

        if (m_CurCheckKey.front()->m_Up == true)
        {
            m_CurCheckKey.pop_front();
        }
    }

    if (m_CurCheckKey.size() == 0)
    {
        IsResult = true;
    }
}


void GJHGameEngineInput::GJHGameEngineKey::Update()
{
    if (KeyCheck() == true)
    {
        if (m_Press == false)
        {
            m_Down = true;
            m_Press = true;
            m_Up = false;
            m_Free = false;
            UpTime = 0.0f;
            PressTime = 0.0f;
        }
        else
        {
            PressTime += GJHGameEngineTime::FDeltaTime();
            m_Down = false;
            m_Press = true;
            m_Up = false;
            m_Free = false;
        }
    }
    else
    {
        if (m_Press == true)
        {
            PressTime = 0.0f;
            UpTime = 0.0f;
            m_Down = false;
            m_Press = false;
            m_Up = true;
            m_Free = false;
        }
        else
        {
            UpTime += GJHGameEngineTime::FDeltaTime();
            m_Down = false;
            m_Press = false;
            m_Up = false;
            m_Free = true;
        }

    }
}

void GJHGameEngineInput::GJHGameEngineKey::Reset()
{
	m_Down = false;
	m_Press = false;
	m_Up = false;
	m_Free = true;
}

bool GJHGameEngineInput::IsKey(const GJHGameEngineString& _Name)
{
    if (GetInst().FindKey(_Name) != nullptr)
    {
        return true;
    }

    return false;
}

bool GJHGameEngineInput::Down(const GJHGameEngineString& _Name)
{
    KeyPtr FindKey = GetInst().FindKey(_Name);

    if (FindKey == nullptr)
    {
        return false;
    }

    return FindKey->m_Down;
}

bool GJHGameEngineInput::Up(const GJHGameEngineString& _Name)
{
    KeyPtr FindKey = GetInst().FindKey(_Name);

    if (FindKey == nullptr)
    {
        return false;
    }

    return FindKey->m_Up;
}

bool GJHGameEngineInput::Press(const GJHGameEngineString& _Name)
{
    KeyPtr FindKey = GetInst().FindKey(_Name);

    if (FindKey == nullptr)
    {
        return false;
    }

    return FindKey->m_Press;
}

bool GJHGameEngineInput::CommandCheck(const GJHGameEngineString& _Name) 
{
    CommandPtr FindCommand = GetInst().FindCommandKey(_Name);

    if (FindCommand == nullptr)
    {
        return false;
    }

    return FindCommand->IsResult;

}

bool GJHGameEngineInput::Free(const GJHGameEngineString& _Name)
{
    KeyPtr FindKey = GetInst().FindKey(_Name);

    if (FindKey == nullptr)
    {
        return false;
    }

    return FindKey->m_Free;
}

GJHGameEngineInput::GJHGameEngineInput()
{

}

GJHGameEngineInput::~GJHGameEngineInput()
{

}

std::shared_ptr<GJHGameEngineInput::GJHCommandKey> GJHGameEngineInput::FindCommandKey(const GJHGameEngineString& _KeyName) 
{

    std::map<GJHGameEngineString, std::shared_ptr<GJHGameEngineInput::GJHCommandKey>>::iterator FindIter = 
        m_AllCommandKey.find(_KeyName);

    if (FindIter == m_AllCommandKey.end())
    {
        return nullptr;
    }

    return FindIter->second;


}

std::shared_ptr<GJHGameEngineInput::GJHGameEngineKey> GJHGameEngineInput::FindKey(const GJHGameEngineString& _KeyName)
{
	std::map<GJHGameEngineString, std::shared_ptr<GJHGameEngineKey>>::iterator FindIter = m_AllKey.find(_KeyName);

	if (FindIter == m_AllKey.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

void GJHGameEngineInput::Update()
{
    for (auto& _Key : m_AllCommandKey)
    {
        _Key.second->Update();
    }

    for (std::pair<GJHGameEngineString, KeyPtr>&& _Key : m_AllKey)
    {
        _Key.second->Update();
    }

    POINT P;

    GetCursorPos(&P);

    //ScreenToClient(GJHGameEngineWindow::GetWindowHWnd(), &P);
    
    m_MousePos.x = static_cast<float>(P.x);
    m_MousePos.y = static_cast<float>(P.y);

    m_PrevMousePos3D = m_MousePos3D;
    m_MousePos3D.x = m_MousePos.x - GJHGameEngineWindow::GetSize().hx();
    m_MousePos3D.y = m_MousePos.y - GJHGameEngineWindow::GetSize().hy();
    m_MousePos3D.y *= -1.0f;

    m_MouseDir3D = m_MousePos3D - m_PrevMousePos3D;
    m_MouseDir3D = m_MouseDir3D.GetNormalize();

    m_MousePos3DDistance = m_MousePos3D - m_PrevMousePos3D;

    MouseMoveToCenter();
}

float4 GJHGameEngineInput::GetMousePos()
{
    return Inst.m_MousePos;
}

float4 GJHGameEngineInput::GetMouse3DPos()
{
    return Inst.m_MousePos3D;
}

float4 GJHGameEngineInput::GetMouse3DDistance()
{
    return Inst.m_MousePos3DDistance;
}

float4 GJHGameEngineInput::GetMouse3DDir()
{
    return Inst.m_MouseDir3D;
}

void GJHGameEngineInput::MouseMoveToCenter()
{
    if (m_IsMouseMoveToCenter == false)
    {
        return;
    }

    RECT Rect;
    GetWindowRect(GJHGameEngineWindow::GetWindowHWnd(), &Rect);

    float4 WindowSize = GJHGameEngineWindow::GetSize();
    float4 MousePos;
    MousePos.x = Rect.left + WindowSize.hx();
    MousePos.y = Rect.top + WindowSize.hy();

    SetCursorPos(MousePos.ix(), MousePos.iy());

    Inst.m_MousePos3D.x = MousePos.x - GJHGameEngineWindow::GetSize().hx();
    Inst.m_MousePos3D.y = MousePos.y - GJHGameEngineWindow::GetSize().hy();
    Inst.m_MousePos3D.y *= -1.f;
}