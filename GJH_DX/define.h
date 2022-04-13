#pragma once

#include <GJHGameEngineInput.h>
#include <GJHGameEngineTime.h>

#define SAFE_DELETE(p) if (p) { delete p; p = nullptr; }

#define DELTATIME GJHGameEngineTime::FDeltaTime

#define CREATEKEY GJHGameEngineInput::GetInst().CreateKey

#define KEYDOWN GJHGameEngineInput::Down
#define KEYUP GJHGameEngineInput::Up
#define KEYPRESS GJHGameEngineInput::Press
#define KEYFREE GJHGameEngineInput::Free

#define CREATECOMMAND GJHGameEngineInput::GetInst().CreateCommand
#define ADDCOMMANDKEY GJHGameEngineInput::GetInst().AddCommmandKey
#define COMMANDCHECK GJHGameEngineInput::GetInst().CommandCheck

#define SOUNDPLAY GJHGameEngineSound::Play
#define SOUNDSTOP GJHGameEngineSound::Stop
#define SOUNDFADEOUT GJHGameEngineSound::FadeOut

#define CAMSIZEX 1920.f
#define CAMSIZEY 1080.f

enum class CollisionType
{
	Player,
	Monster,
};