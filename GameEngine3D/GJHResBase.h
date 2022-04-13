#pragma once
#include <GJHNameBase.h>
#include <GJHTypeBase.h>
#include <GJHGameEngineMemory.h>

class GJHResBase : public GJHNameBase, public GJHTypeBase, public SharedPtrThis<GJHResBase>
{
public:
	GJHResBase()
	{

	}

	virtual ~GJHResBase() = 0
	{

	}

public:
	GJHResBase(const GJHResBase& _Other) = delete;
	GJHResBase(const GJHResBase&& _Other) = delete;

public:
	GJHResBase operator=(const GJHResBase& _Other) = delete;
	GJHResBase operator=(const GJHResBase&& _Other) = delete;

public:
	virtual void Setting() { }
};