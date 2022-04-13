#include "BasePre.h"
#include "GJHGameEngineThread.h"

std::mutex GJHGameEngineThread::Lock;
std::map<GJHGameEngineString, std::shared_ptr<GJHGameEngineThread>> GJHGameEngineThread::m_ThreadMap;

GJHGameEngineThread::~GJHGameEngineThread()
{
	if (ThreadPtr != nullptr)
	{
		ThreadPtr->join();
	}
}