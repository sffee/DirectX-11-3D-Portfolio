#pragma once
#include <GJHGameEngineGUI.h>
#include <vector>
#include <GJHGameEngineWindow.h>
#include <GJHDirectRenderTarget.h>

class GJHDirectRenderTarget;
class RenderTargetWindow : public GJHGUIWindow
{
private:
	GJHDirectRenderTarget* m_RenderTarget;
	int m_Index;

public:
	void SetRenderTarget(GJHDirectRenderTarget* _RenderTarget, int _Index)
	{
		m_RenderTarget = _RenderTarget;
		m_Index = _Index;
	}

public:
	void OnGUI() override;
};

class RenderingWindow : public GJHGUIWindow
{
public:
	class TargetInfo
	{
	public:
		GJHGameEngineString Name;
		float4 Size;
	};

public:
	static std::vector<TargetInfo> OtherTargetInfo;
	static std::vector<std::shared_ptr<GJHDirectRenderTarget>> OtherTarget;

public:
	static void PushOtherRenderTarget(const GJHGameEngineString& _Name, const float4& _Size, std::shared_ptr<GJHDirectRenderTarget> _Target)
	{
		TargetInfo NewInfo;

		NewInfo.Name = _Name;
		NewInfo.Size = _Size;

		OtherTargetInfo.push_back(NewInfo);
		OtherTarget.push_back(_Target);
	}

public:
	void OnGUI() override;
};

