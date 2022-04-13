#pragma once
#include <fmod.hpp>
#include <map>
#include "GJHGameEngineMemory.h"
#include "GJHGameEngineString.h"

#ifdef _DEBUG
#pragma comment(lib, "fmodL_vc.lib")
#else
#pragma comment(lib, "fmod_vc.lib")
#endif

class GJHGameEngineSound;
class GJHSoundPlayer : std::enable_shared_from_this<GJHSoundPlayer>
{
public:
	friend GJHGameEngineSound;

private:
	GJHGameEngineString m_Name;
	FMOD::Channel* m_pController;
	float m_Volume;
	bool m_EnablePich;

public:
	GJHGameEngineString GetName()
	{
		return m_Name;
	}

public:
	void Stop();

public:
	void SetVolume(float _Volume)
	{
		m_Volume = _Volume;
		m_pController->setVolume(_Volume);
	}

	void SetFadeOut(int _Rate, float _Time)
	{
		unsigned long long DspClock;

		m_pController->getDSPClock(0, &DspClock);
		m_pController->addFadePoint(DspClock, m_Volume);
		m_pController->addFadePoint(DspClock + ((unsigned long long)_Rate * (unsigned long long)_Time), 0.f);
	}

	void SetEnablePich(bool _Set)
	{
		m_EnablePich = _Set;
	}

	void SetPitch(float _Pitch)
	{
		if (m_EnablePich == true)
		{
			m_pController->setPitch(_Pitch);
		}
	}

	void SetPause(bool _Pause)
	{
		m_pController->setPaused(_Pause);
	}

public:
	GJHSoundPlayer() :
		m_pController(nullptr),
		m_EnablePich(true)
	{
	}

	GJHSoundPlayer(const GJHSoundPlayer& _OtherPlayer) :
		m_pController(_OtherPlayer.m_pController)
	{
	}

	GJHSoundPlayer(FMOD::Channel* _pController) :
		m_pController(_pController)
	{
	}

public:

};

class GJHGameEngineSound : std::enable_shared_from_this<GJHGameEngineSound>
{
private:
	static FMOD::System* m_pSoundSystem;
	static float m_Pitch;

public:
	static void Init();
	static void Update();
	static void Release();

public:
	friend FMOD_RESULT _stdcall G_FMOD_CHANNELCONTROL_CALLBACK(
		FMOD_CHANNELCONTROL* channelcontrol,
		FMOD_CHANNELCONTROL_TYPE controltype,
		FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype,
		void* commanddata1,
		void* commanddata2);

private:
	static std::map<GJHGameEngineString, std::shared_ptr<GJHGameEngineSound>> m_AllLoadingSound;
	static std::map<GJHGameEngineString, std::shared_ptr<GJHSoundPlayer>> m_AllPlaySoundControl;
	static std::vector<std::shared_ptr<GJHSoundPlayer>> m_AllNormalSound;

private:
	static void ErasePlayer(const GJHGameEngineString& _PlayerName);

public:
	static std::shared_ptr<GJHSoundPlayer> FindPlayer(const GJHGameEngineString& _PlayerName);
	static std::shared_ptr<GJHGameEngineSound> FindSound(const GJHGameEngineString& _FileName);
	static void Load(const GJHGameEngineString& _Path);
	static void Play(const GJHGameEngineString& _Path, int _LoopCount = 0, float _Volume = 1.f, bool _EnablePitch = true);
	static void Play(const GJHGameEngineString& _ControlName, const GJHGameEngineString& _FileName, int _LoopCount = 0, float _Volume = 1.f, bool _EnablePitch = true);
	static void Stop(const GJHGameEngineString& _ControlName);
	static void Pause(const GJHGameEngineString& _ControlName);
	static void Resume(const GJHGameEngineString& _ControlName);
	static void FadeOut(const GJHGameEngineString& _ControlName, float _Time);
	static void SetPitch(float _Pitch);

public:
	GJHGameEngineSound();
	~GJHGameEngineSound();

public:
	GJHGameEngineSound(const GJHGameEngineSound& _Other) = delete;
	GJHGameEngineSound(const GJHGameEngineSound&& _Other) = delete;

public:
	GJHGameEngineSound operator=(const GJHGameEngineSound& _Other) = delete;
	GJHGameEngineSound operator=(const GJHGameEngineSound&& _Other) = delete;

private:
	FMOD::Sound* m_pSound;

public:
	void LoadSound(const GJHGameEngineString& _Path);
};