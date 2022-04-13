#include "GJHFbxAnimationRenderer.h"
#include "GJHCamera.h"

GJHFbxAnimation::GJHFbxAnimation() :
	m_AnimationEnd(false),
	m_CurFrame(0)
{

}

GJHFbxAnimationRenderer::GJHFbxAnimationRenderer()
{
	SetAniOn();
}

GJHFbxAnimationRenderer::~GJHFbxAnimationRenderer()
{
	m_CurAnimation = nullptr;
	AllAnimation.clear();
}

std::shared_ptr<GJHFbxAnimation> GJHFbxAnimationRenderer::FindAnimation(const GJHGameEngineString& _Name)
{
	if (AllAnimation.end() == AllAnimation.find(_Name))
	{
		return nullptr;
	}

	return AllAnimation[_Name];
}

void GJHFbxAnimationRenderer::CreateUserAnimation(
	GJHGameEngineString _AniName,
	GJHGameEngineString _MeshFbx,
	GJHGameEngineString _AniFbx,
	GJHGameEngineString _MatName,
	int _AnimationIndex,
	bool _Loop,
	float _FrameTime)
{
	std::shared_ptr<GJHDirectFbx> AniFbx = GJHDirectFbx::Find(_AniFbx);

	if (AniFbx == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("애니메이션을 담당할 fbx가 존재하지 않습니다..");
		return;
	}

	FbxExAniData* AniData = AniFbx->GetAniData(_AnimationIndex);

	if (AniData == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("애니메이션 정보가 없는 애니 fbx입니다.");
		return;
	}

	int StartFrame = (0 < (int)AniData->TimeStartCount) ? 0 : (int)AniData->TimeStartCount;
	int EndFrame = (0 < (int)AniData->TimeStartCount) ? (int)AniData->TimeEndCount - (int)AniData->TimeStartCount : (int)AniData->TimeEndCount;

	CreateUserAnimation(_AniName, _MeshFbx, _AniFbx, _MatName, _AnimationIndex, _Loop, StartFrame, EndFrame, _FrameTime);
}


void GJHFbxAnimationRenderer::CreateUserAnimation(
	GJHGameEngineString _AniName,
	GJHGameEngineString _MeshFbx,
	GJHGameEngineString _AniFbx,
	GJHGameEngineString _MatName,
	int _AnimationIndex,
	bool _Loop,
	int _Start,
	int _End,
	float _FrameTime)
{
	if (FindAnimation(_AniName) != nullptr)
	{
		GJHGameEngineDebug::AssertMsg("이미 존재하는 애니메이션 입니다.");
		return;
	}

	std::shared_ptr<GJHDirectFbx> MeshFbx = GJHDirectFbx::Find(_MeshFbx);
	std::shared_ptr<GJHDirectFbx> AniFbx = GJHDirectFbx::Find(_AniFbx);

	if (MeshFbx == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("매쉬를 담당하는 fbx가 존재하지 않습니다.");
	}

	if (AniFbx == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("애니메이션을 담당할 fbx가 존재하지 않습니다..");
	}

	FbxExAniData* AniData = AniFbx->GetAniData(_AnimationIndex);

	if (AniData == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("애니메이션 정보가 없는 애니 fbx입니다.");
	}

	if (_End < _Start)
	{
		GJHGameEngineDebug::AssertMsg("스타트 프레임과 앤드 프레임이 맞지 않습니다.");
	}

	if (AniData->TimeStartCount < _Start)
	{
		GJHGameEngineDebug::AssertMsg("애니메이션 fbx의 애니메이션 데이터의 start보다 작습니다.");
		return;
	}

	if (AniData->TimeEndCount < _End)
	{
		GJHGameEngineDebug::AssertMsg("애니메이션 fbx의 애니메이션 데이터의 _End보다 큽니다.");
		return;
	}

	std::shared_ptr<GJHFbxAnimation> NewAni = std::make_shared<GJHFbxAnimation>();

	NewAni->m_Name = _AniName;
	NewAni->m_MeshFbx = MeshFbx;
	NewAni->m_AniFbx = AniFbx;
	NewAni->m_AniData = AniData;
	NewAni->m_Loop = _Loop;
	NewAni->m_FrameTime = _FrameTime;
	NewAni->m_Start = _Start;
	NewAni->m_End = _End;
	NewAni->m_CurFrame = 0;
	NewAni->m_CurFrameTime = 0.0f;
	NewAni->m_Parent = this;
	NewAni->FrameBoneData.resize(MeshFbx->GetBoneCount());
	NewAni->AnimationMatrix.resize(MeshFbx->GetBoneCount());

	NewAni->AniRenderPlayer = SetFbxToCreateAllRenderPlayer(_MeshFbx, _MatName, GetOrder(),
		[NewAni](GJHRenderPlayer* _Other)
		{
			if (_Other->IsCBufferSetting(L"OutPutColor") == true)
			{
				_Other->CBufferSettingNew(L"OutPutColor", float4::WHITE);
			}

			if (_Other->IsSBufferSetting(L"ArrAniMationMatrix") == false)
			{
				GJHGameEngineDebug::AssertMsg("애니메이션을 동작시킬수 없는 메테리얼 입니다.");
				return;
			}
			else
			{
				_Other->SBufferSettingLink(L"ArrAniMationMatrix", NewAni->AnimationMatrix);
			}
		});

	for (size_t i = 0; i < NewAni->AniRenderPlayer.size(); i++)
	{
		NewAni->AniRenderPlayer[i]->Off();
	}

	AllAnimation[_AniName] = NewAni;
}

void GJHFbxAnimationRenderer::ChangeAnimation(const GJHGameEngineString& _AniName)
{
	std::shared_ptr<GJHFbxAnimation> NextAni = FindAnimation(_AniName);

	if (NextAni == m_CurAnimation)
	{
		return;
	}

	if (m_CurAnimation != nullptr)
	{
		for (size_t i = 0; i < m_CurAnimation->AniRenderPlayer.size(); i++)
		{
			m_CurAnimation->AniRenderPlayer[i]->Off();
		}
	}

	m_CurAnimation = NextAni;

	if (m_CurAnimation != nullptr)
	{
		m_CurAnimation->Reset();
		m_CurAnimation->CalAnimationMatrix();
	}
	else
	{
		GJHGameEngineDebug::AssertMsg("존재하지 않는 애니메이션으로 체인지하려고 했습니다.");
	}
}

void GJHFbxAnimationRenderer::CreateAnimation(
	GJHGameEngineString _AniName,
	GJHGameEngineString _MeshFbx,
	GJHGameEngineString _AniFbx,
	GJHGameEngineString _MatName,
	int _AnimationIndex,
	bool _Loop,
	float _FrameTime
)
{
	std::shared_ptr<GJHDirectFbx> AniFbx = GJHDirectFbx::Find(_AniFbx);

	if (AniFbx == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("애니메이션을 담당할 fbx가 존재하지 않습니다..");
		return;
	}

	FbxExAniData* AniData = AniFbx->GetAniData(_AnimationIndex);

	if (AniData == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("애니메이션 정보가 없는 애니 fbx입니다.");
	}

	int StartFrame = (0 < (int)AniData->TimeStartCount) ? 0 : (int)AniData->TimeStartCount;
	int EndFrame = (0 < (int)AniData->TimeStartCount) ? (int)AniData->TimeEndCount - (int)AniData->TimeStartCount : (int)AniData->TimeEndCount;

	CreateAnimation(_AniName, _MeshFbx, _AniFbx, _MatName, _AnimationIndex, _Loop, StartFrame, EndFrame, _FrameTime);
}

void GJHFbxAnimationRenderer::CreateAnimation(
	GJHGameEngineString _AniName,
	GJHGameEngineString _MeshFbx,
	GJHGameEngineString _AniFbx,
	GJHGameEngineString _MatName,
	int _AnimationIndex,
	bool _Loop,
	int _Start,
	int _End,
	float _FrameTime
)
{
	if (FindAnimation(_AniName) != nullptr)
	{
		GJHGameEngineDebug::AssertMsg("이미 존재하는 애니메이션 입니다.");
		return;
	}
	
	std::shared_ptr<GJHDirectFbx> MeshFbx = GJHDirectFbx::Find(_MeshFbx);
	std::shared_ptr<GJHDirectFbx> AniFbx = GJHDirectFbx::Find(_AniFbx);

	if (MeshFbx == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("매쉬를 담당하는 fbx가 존재하지 않습니다.");
	}

	if (AniFbx == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("애니메이션을 담당할 fbx가 존재하지 않습니다..");
	}

	FbxExAniData* AniData = AniFbx->GetAniData(_AnimationIndex);

	if (AniData == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("애니메이션 정보가 없는 애니 fbx입니다.");
	}

	if (_End < _Start)
	{
		GJHGameEngineDebug::AssertMsg("스타트 프레임과 앤드 프레임이 맞지 않습니다.");
	}

	if (AniData->TimeStartCount < _Start)
	{
		GJHGameEngineDebug::AssertMsg("애니메이션 fbx의 애니메이션 데이터의 start보다 작습니다.");
		return;
	}

	if (AniData->TimeEndCount < _End)
	{
		GJHGameEngineDebug::AssertMsg("애니메이션 fbx의 애니메이션 데이터의 _End보다 큽니다.");
		return;
	}

	AniFbx->CalFbxExBoneFrameTransMatrix(MeshFbx);

	std::shared_ptr<GJHFbxAnimation> NewAni = std::make_shared<GJHFbxAnimation>();

	NewAni->m_Name = _AniName;
	NewAni->m_MeshFbx = MeshFbx;
	NewAni->m_AniFbx = AniFbx;
	NewAni->m_AniData = AniData;
	NewAni->m_Loop = _Loop;
	NewAni->m_FrameTime = _FrameTime;
	NewAni->m_Start = _Start;
	NewAni->m_End = _End;
	NewAni->m_CurFrame = 0;
	NewAni->m_CurFrameTime = 0.0f;
	NewAni->m_Parent = this;
	NewAni->FrameBoneData.resize(MeshFbx->GetBoneCount());
	NewAni->AnimationMatrix.resize(MeshFbx->GetBoneCount());

	NewAni->AniRenderPlayer = SetFbxToCreateAllRenderPlayer(_MeshFbx, _MatName, GetOrder(),
		[NewAni](GJHRenderPlayer* _Other)
		{
			if (_Other->IsSBufferSetting(L"ArrAniMationMatrix") == false)
			{
				GJHGameEngineDebug::AssertMsg("애니메이션을 동작시킬수 없는 메테리얼 입니다.");
				return;
			}
			else
			{
				_Other->SBufferSettingLink(L"ArrAniMationMatrix", NewAni->AnimationMatrix);
			}
		}
	);

	AllAnimation[_AniName] = NewAni;
}

void GJHFbxAnimation::Reset()
{
	for (size_t i = 0; i < AniRenderPlayer.size(); i++)
	{
		AniRenderPlayer[i]->On();
	}

	m_CurFrame = m_Start;
	m_CurFrameTime = 0.0f;
	m_AnimationEnd = false;
}


void GJHFbxAnimation::CalAnimationMatrix()
{
	m_CurFrameTime += GJHGameEngineTime::FDeltaTime();
	if (m_FrameTime <= m_CurFrameTime)
	{
		m_CurFrameTime -= m_FrameTime;
		++m_CurFrame;
	}

	if (m_End <= m_CurFrame)
	{
		if (m_Loop == true)
		{
			m_CurFrame = m_Start;
		}
		else
		{
			m_CurFrame = m_End;
			m_AnimationEnd = true;
		}
	}

	int NextFrame = m_CurFrame + 1;
	if (m_End <= NextFrame)
	{
		if (m_Loop == true)
		{
			NextFrame = m_Start;
		}
		else
		{
			NextFrame = m_End;
		}
	}

	for (int i = 0; i < AnimationMatrix.size(); i++)
	{
		Bone* BoneData = m_MeshFbx->FindBone(i);

		if (m_AniData->m_AniFrameData[i].m_Data.empty() == true)
		{
			AnimationMatrix[i] = float4x4::Affine(BoneData->BonePos.GlobalScale, BoneData->BonePos.GlobalRotation, BoneData->BonePos.GlobalTranslation);
			return;
		}

		FbxExBoneFrameData& CurData = m_AniData->m_AniFrameData[i].m_Data[m_CurFrame];
		FbxExBoneFrameData& NextData = m_AniData->m_AniFrameData[i].m_Data[NextFrame];

		float4 LerpScale = float4::Lerp(CurData.S, NextData.S, m_CurFrameTime);
		float4 SLerpQ = float4::SLerp(CurData.Q, NextData.Q, m_CurFrameTime);
		float4 LerpPos = float4::Lerp(CurData.T, NextData.T, m_CurFrameTime);

		size_t Size = sizeof(float4x4);

		float4x4 Mat = float4x4::Affine(LerpScale, SLerpQ, LerpPos);

		FrameBoneData[i].Pos = LerpPos;
		FrameBoneData[i].Q = SLerpQ;
		FrameBoneData[i].Scale = LerpScale;

		FrameBoneData[i].AniScaleMat = float4x4::Affine(LerpScale, float4::QIden, float4::ZERO);
		FrameBoneData[i].AniRotMat = float4x4::Affine(float4::ONE, SLerpQ, float4::NONE);
		FrameBoneData[i].AniPosMat = float4x4::Affine(float4::ONE, float4::QIden, LerpPos);

		FrameBoneData[i].AniWorldMat = float4x4::Affine(LerpScale, SLerpQ, LerpPos);

		AnimationMatrix[i] = BoneData->BonePos.Offset * float4x4::Affine(LerpScale, SLerpQ, LerpPos);
	}
}

std::shared_ptr<GJHFbxAnimation> GJHFbxAnimationRenderer::GetCurrentAnimation()
{
	return m_CurAnimation;
}

void GJHFbxAnimationRenderer::Update()
{
	if (m_CurAnimation == nullptr)
	{
		return;
	}

	m_CurAnimation->CalAnimationMatrix();
}

void GJHFbxAnimationRenderer::RenderingSetting()
{
	DebugCheck();

	GJHRenderingTransformData& _CameraData = m_CurCamera->GetCameraTransformMatrix();
	CalRenderDataCopy();
	GJHRenderingTransformData& RendererData = GetRenderingData();
	RendererData.View = _CameraData.View;
	RendererData.Projection = _CameraData.Projection;
	RendererData.CalRenderingMatrix();
}

bool GJHFbxAnimationRenderer::AnimationEnd()
{
	if (m_CurAnimation == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (m_CurAnimation == nullptr)");
		return false;
	}

	return m_CurAnimation->m_AnimationEnd;
}