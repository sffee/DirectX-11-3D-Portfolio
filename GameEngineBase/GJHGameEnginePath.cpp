#include "BasePre.h"
#include "GJHGameEnginePath.h"

bool GJHGameEnginePath::IsExist(const GJHGameEnginePath& _Path)
{
	if (_waccess(_Path.m_PathStr.GetConstStringPtr(), 0) != 0)
	{
		return false;
	}

	return true;
}

bool GJHGameEnginePath::IsExist(const GJHGameEngineString& _Path)
{
	if (_waccess(_Path.c_str(), 0) != 0)
	{
		return false;
	}

	return true;
}