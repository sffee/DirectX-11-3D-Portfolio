#pragma once
#include "GJHGameEngineString.h"

class GJHNameBase
{
private:
	GJHGameEngineString m_Name;

public:
	void SetName(const GJHGameEngineString& _Name)
	{
		m_Name = _Name;
	}

	GJHGameEngineString GetName()
	{
		return m_Name;
	}

public:
	GJHNameBase() { }
	~GJHNameBase() { }
	GJHNameBase(const GJHNameBase& _Other) = delete;
	GJHNameBase(const GJHNameBase&& _Other) = delete;

public:
	GJHNameBase operator=(const GJHNameBase& _Other) = delete;
	GJHNameBase operator=(const GJHNameBase&& _Other) = delete;
};