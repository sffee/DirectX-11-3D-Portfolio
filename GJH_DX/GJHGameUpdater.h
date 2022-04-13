#pragma once
#include <GJHGameEngineWindow.h>
#include <GJHGameEngineDevice.h>

class GJHGameEngineWorld;
class GJHGameEngineTransform;
class GJHGlobalAppWindow;
class GJHGameUpdater : public GJHGameEngineWindowUpdater
{
public:
	static bool IsLoadingEnd;

public:
	virtual void UpdateStart() override;
	virtual void DeadTimeUpdate() override;
	virtual void End() override;

private:
	void ResLodingThread();
	void MakeKey();
};