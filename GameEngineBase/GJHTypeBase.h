#pragma once

class GJHTypeBase
{
public:
	GJHTypeBase() { }
	virtual ~GJHTypeBase() { }

public:
	GJHTypeBase(const GJHTypeBase& _Other) = delete;
	GJHTypeBase(const GJHTypeBase&& _Other) = delete;

public:
	GJHTypeBase operator=(const GJHTypeBase& _Other) = delete;
	GJHTypeBase operator=(const GJHTypeBase&& _Other) = delete;

public:
	const type_info& GetType()
	{
		return typeid(*this);
	}
};