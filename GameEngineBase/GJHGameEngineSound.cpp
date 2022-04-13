#include "BasePre.h"
#include "GJHGameEngineSound.h"
#include "GJHGameEngineDebug.h"
#include "GJHGameEnginePath.h"
#include <fmod.h>

void GJHSoundPlayer::Stop()
{
	m_pController->stop();
}

FMOD::System* GJHGameEngineSound::m_pSoundSystem;
std::map<GJHGameEngineString, std::shared_ptr<GJHGameEngineSound>> GJHGameEngineSound::m_AllLoadingSound;
std::map<GJHGameEngineString, std::shared_ptr<GJHSoundPlayer>> GJHGameEngineSound::m_AllPlaySoundControl;
std::vector<std::shared_ptr<GJHSoundPlayer>> GJHGameEngineSound::m_AllNormalSound;
float GJHGameEngineSound::m_Pitch = 1.f;

GJHGameEngineSound::GJHGameEngineSound() :
	m_pSound(nullptr)
{

}

GJHGameEngineSound::~GJHGameEngineSound()
{

}

void GJHGameEngineSound::Release()
{
	if (m_pSoundSystem != nullptr)
	{
		m_pSoundSystem->release();
	}
}

void GJHGameEngineSound::Init()
{
	FMOD::System_Create(&GJHGameEngineSound::m_pSoundSystem);

	if (GJHGameEngineSound::m_pSoundSystem == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (GJHGameEngineSound::m_pSoundSystem == nullptr)");
	}

	if (GJHGameEngineSound::m_pSoundSystem->init(32, FMOD_DEFAULT, nullptr) != FMOD_OK)
	{
		GJHGameEngineDebug::AssertMsg("if (GJHGameEngineSound::m_pSoundSystem->init(32, FMOD_DEFAULT, nullptr) != FMOD_OK)");
	}
}

void GJHGameEngineSound::Update()
{
	m_pSoundSystem->update();
}

void GJHGameEngineSound::Load(const GJHGameEngineString& _Path)
{
	GJHGameEnginePath Path = _Path;
	if (FindSound(Path.FileName()) != nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (FindSound(Path.FileName()) != nullptr)");
	}

	std::shared_ptr<GJHGameEngineSound> NewSoundFile = std::make_shared<GJHGameEngineSound>();

	NewSoundFile->LoadSound(_Path);

	m_AllLoadingSound.insert(std::map<GJHGameEngineString, std::shared_ptr<GJHGameEngineSound>>::value_type(Path.FileName(), NewSoundFile));
}

std::shared_ptr<GJHGameEngineSound> GJHGameEngineSound::FindSound(const GJHGameEngineString& _FileName)
{
	std::map<GJHGameEngineString, std::shared_ptr<GJHGameEngineSound>>::iterator FindSoundIter = m_AllLoadingSound.find(_FileName);

	if (FindSoundIter == m_AllLoadingSound.end())
	{
		return nullptr;
	}

	return FindSoundIter->second;
}

void GJHGameEngineSound::LoadSound(const GJHGameEngineString& _Path)
{
	if (m_pSoundSystem->createSound(_Path.GetConvertMultiByteString().c_str(), FMOD_DEFAULT, nullptr, &m_pSound) != FMOD_OK)
	{
		GJHGameEngineDebug::AssertMsg("if (m_pSoundSystem->createSound(_Path.GetConvertMultiByteString().c_str(), FMOD_DEFAULT, nullptr, &m_pSound) != FMOD_OK)");
	}
}

FMOD_RESULT _stdcall G_FMOD_CHANNELCONTROL_CALLBACK(
	FMOD_CHANNELCONTROL* channelcontrol,
	FMOD_CHANNELCONTROL_TYPE controltype,
	FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype,
	void* commanddata1,
	void* commanddata2)
{
	switch (callbacktype)
	{
	case FMOD_CHANNELCONTROL_CALLBACK_END:
	{
		void* Ptr;
		FMOD::Channel* ch = reinterpret_cast<FMOD::Channel*>(channelcontrol);
		ch->getUserData(&Ptr);
		GJHSoundPlayer* NewPlayer = reinterpret_cast<GJHSoundPlayer*>(Ptr);
		GJHGameEngineSound::ErasePlayer(NewPlayer->GetName());
		break;
	}
	case FMOD_CHANNELCONTROL_CALLBACK_VIRTUALVOICE:
	case FMOD_CHANNELCONTROL_CALLBACK_SYNCPOINT:
	case FMOD_CHANNELCONTROL_CALLBACK_OCCLUSION:
	case FMOD_CHANNELCONTROL_CALLBACK_MAX:
	case FMOD_CHANNELCONTROL_CALLBACK_FORCEINT:
	default:
		break;
	}

	return FMOD_RESULT::FMOD_OK;
}

void GJHGameEngineSound::Play(const GJHGameEngineString& _FileName, int _LoopCount, float _Volume, bool _EnablePitch)
{
	std::shared_ptr<GJHGameEngineSound> SoundPtr = FindSound(_FileName);

	if (SoundPtr == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (SoundPtr == nullptr)");
	}

	FMOD::Channel* Controller;

	if (m_pSoundSystem->playSound(SoundPtr->m_pSound, nullptr, false, &Controller) != FMOD_OK)
	{
		GJHGameEngineDebug::AssertMsg("if (m_pSoundSystem->playSound(SoundPtr->m_pSound, nullptr, false, &Controller) != FMOD_OK)");
	}

	Controller->setMode(FMOD_LOOP_NORMAL);
	Controller->setLoopCount(_LoopCount);
	Controller->setCallback(G_FMOD_CHANNELCONTROL_CALLBACK);
	
	std::shared_ptr<GJHSoundPlayer> Player = std::make_shared<GJHSoundPlayer>(Controller);
	Player->m_Name = L"";
	Player->SetVolume(_Volume);

	Player->SetEnablePich(_EnablePitch);
	if (_EnablePitch == true)
	{
		Player->SetPitch(m_Pitch);
	}

	Controller->setUserData(Player.get());

	m_AllNormalSound.push_back(Player);
}

void GJHGameEngineSound::Play(const GJHGameEngineString& _ControlName, const GJHGameEngineString& _FileName, int _LoopCount, float _Volume, bool _EnablePitch)
{
	std::shared_ptr<GJHGameEngineSound> SoundPtr = FindSound(_FileName);
	if (SoundPtr == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (SoundPtr == nullptr)");
	}

	if (FindPlayer(_ControlName) != nullptr)
	{
		return;
	}

	FMOD::Channel* Controller;
	if (m_pSoundSystem->playSound(SoundPtr->m_pSound, nullptr, false, &Controller) != FMOD_OK)
	{
		GJHGameEngineDebug::AssertMsg("if (m_pSoundSystem->playSound(SoundPtr->m_pSound, nullptr, false, &Controller) != FMOD_OK)");
	}

	Controller->setMode(FMOD_LOOP_NORMAL);
	Controller->setLoopCount(_LoopCount);
	Controller->setCallback(G_FMOD_CHANNELCONTROL_CALLBACK);

	std::shared_ptr<GJHSoundPlayer> Player = std::make_shared<GJHSoundPlayer>(Controller);
	Player->m_Name = _ControlName;
	Player->SetVolume(_Volume);

	Player->SetEnablePich(_EnablePitch);
	if (_EnablePitch == true)
	{
		Player->SetPitch(m_Pitch);
	}

	Controller->setUserData(Player.get());

	m_AllPlaySoundControl.insert(std::map<GJHGameEngineString, std::shared_ptr<GJHSoundPlayer>>::value_type(_ControlName, Player));
}

void GJHGameEngineSound::Stop(const GJHGameEngineString& _ControlName)
{
	std::shared_ptr<GJHSoundPlayer> Player = FindPlayer(_ControlName);
	if (Player == nullptr)
	{
		return;
	}

	Player->Stop();
	Player = nullptr;
}

void GJHGameEngineSound::Pause(const GJHGameEngineString& _ControlName)
{
	std::shared_ptr<GJHSoundPlayer> Player = FindPlayer(_ControlName);
	if (Player == nullptr)
	{
		return;
	}

	Player->SetPause(true);
}

void GJHGameEngineSound::Resume(const GJHGameEngineString& _ControlName)
{
	std::shared_ptr<GJHSoundPlayer> Player = FindPlayer(_ControlName);
	if (Player == nullptr)
	{
		return;
	}

	Player->SetPause(false);
}

std::shared_ptr<GJHSoundPlayer> GJHGameEngineSound::FindPlayer(const GJHGameEngineString& _PlayerName)
{
	std::map<GJHGameEngineString, std::shared_ptr<GJHSoundPlayer>>::iterator FindIter = m_AllPlaySoundControl.find(_PlayerName);

	if (FindIter == m_AllPlaySoundControl.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

void GJHGameEngineSound::ErasePlayer(const GJHGameEngineString& _PlayerName)
{
	std::map<GJHGameEngineString, std::shared_ptr<GJHSoundPlayer>>::iterator FindIter = m_AllPlaySoundControl.find(_PlayerName);

	if (FindIter != m_AllPlaySoundControl.end())
	{
		m_AllPlaySoundControl.erase(FindIter);
	}
}

void GJHGameEngineSound::FadeOut(const GJHGameEngineString& _ControlName, float _Time)
{
	std::shared_ptr<GJHSoundPlayer> Player = FindPlayer(_ControlName);

	if (Player == nullptr)
	{
		return;
	}

	int Rate;
	m_pSoundSystem->getSoftwareFormat(&Rate, 0, 0);

	Player->SetFadeOut(Rate, _Time);
}

void GJHGameEngineSound::SetPitch(float _Pitch)
{
	for (auto& Sound : m_AllPlaySoundControl)
	{
		Sound.second->SetPitch(_Pitch);
	}

	for (auto& Sound : m_AllNormalSound)
	{
		Sound->SetPitch(_Pitch);
	}

	m_Pitch = _Pitch;
}