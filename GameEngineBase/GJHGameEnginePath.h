#pragma once
#include "GJHGameEngineString.h"
#include "GJHGameEngineDebug.h"

enum class PATHTYPE
{
	FILE,
	DIR,
	NONE,
};

class GJHGameEnginePath
{
public:
	GJHGameEngineString m_PathStr;

public:
	static bool IsExist(const GJHGameEnginePath& _Path);
	static bool IsExist(const GJHGameEngineString& _Path);

	GJHGameEngineString FullPath() const
	{
		return m_PathStr;
	}

	GJHGameEngineString Path()
	{
		if (Type() == PATHTYPE::DIR)
		{
			return m_PathStr;
		}

		size_t LastCharPos = m_PathStr.LastFind(L"\\", m_PathStr.GetSize() - 2);

		if (LastCharPos == GJHGameEngineString::FindError)
		{
			MsgAssert("if (LastCharPos == GJHGameEngineString::FindError");
		}

		GJHGameEngineString Path = m_PathStr.Cut(0, LastCharPos + 1);

		return Path;
	}

public:
	PATHTYPE Type() const
	{
		if (m_PathStr.GetSize() == 0)
		{
			return PATHTYPE::NONE;
		}

		if (m_PathStr.ConstAt(m_PathStr.GetSize() - 1) == '\\')
		{
			return PATHTYPE::DIR;
		}

		return PATHTYPE::FILE;
	}

public:
	GJHGameEngineString DirectoryName() const
	{
		if (Type() != PATHTYPE::DIR)
		{
			MsgAssert("Type() != PATHTYPE::DIR");
		}

		size_t LastCharPos = m_PathStr.LastFind(L"\\", m_PathStr.GetSize() - 2);

		if (LastCharPos == GJHGameEngineString::FindError)
		{
			MsgAssert("if (LastCharPos == GJHGameEngineString::FindError");
		}

		GJHGameEngineString DirName = m_PathStr.Cut(LastCharPos + 1, m_PathStr.GetSize() - LastCharPos - 2);

		return DirName;
	}

	static GJHGameEngineString GetWithOutExtFileName(const GJHGameEngineString& _FileName)
	{
		GJHGameEngineString ReturnName = _FileName;

		size_t Size = ReturnName.LastFind(L".");

		ReturnName.Erase(Size, -1);

		return ReturnName;
	}

	static GJHGameEngineString GetFileExt(const GJHGameEngineString& _Path)
	{
		GJHGameEngineString FileName = GetFileName(_Path);

		size_t Size = FileName.LastFind(L".");

		FileName.Erase(FileName.LastFind(L"."), -1);

		return FileName;
	}

	static GJHGameEngineString GetFileName(const GJHGameEngineString& _Path)
	{
		GJHGameEngineString Result = _Path;

		size_t LastCharPos = Result.LastFind(L"\\");

		if (LastCharPos == GJHGameEngineString::FindError)
		{
			MsgAssert("if (LastCharPos == GJHGameEngineString::FindError)");
		}

		GJHGameEngineString FileName = Result.Cut(LastCharPos + 1, Result.GetSize() - LastCharPos);

		return FileName;
	}

	GJHGameEngineString FileName() const
	{
		if (Type() != PATHTYPE::FILE)
		{
			MsgAssert("if (Type() != PATHTYPE::FILE)");
		}

		return GetFileName(m_PathStr);
	}


public:
	GJHGameEnginePath(const GJHGameEngineString& _Path) :
		m_PathStr(_Path)
	{

	}
	~GJHGameEnginePath()
	{

	}
};