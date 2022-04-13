#include "BasePre.h"
#include "GJHGameEngineDirectory.h"
#include "GJHGameEngineFile.h"

std::vector<GJHGameEngineFile> GJHGameEngineDirectory::AllFile(const GJHGameEngineString& _FileType, bool _IsSubDirectory)
{
	if (Type() != PATHTYPE::DIR)
	{
		MsgAssert("if (Type() != PATHTYPE::DIR)");
		return std::vector<GJHGameEngineFile>();
	}

	std::vector<GJHGameEngineFile> FileList;

	FindFileInDirectory(m_PathStr, _FileType.Lower(), FileList, _IsSubDirectory);

	return FileList;
}

void GJHGameEngineDirectory::FindFileInDirectory(const GJHGameEngineString& _DirPath, const GJHGameEngineString& _FileType, std::vector<GJHGameEngineFile>& _FileList, bool _IsSubDirectory)
{
	WIN32_FIND_DATAW fd;
	HANDLE hFind = FindFirstFileW((_DirPath + L"*.*").c_str(), &fd);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}
	
	do
	{
		if (lstrcmpW(fd.cFileName, L".") == 0
			|| lstrcmpW(fd.cFileName, L"..") == 0)
		{
			continue;
		}

		if (_IsSubDirectory == true && fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			FindFileInDirectory(_DirPath + fd.cFileName + L"\\", _FileType, _FileList, _IsSubDirectory);
		}

		GJHGameEngineFile File = GJHGameEngineFile(_DirPath + fd.cFileName);
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
		{
			if (_FileType == L"*.*" ||
				File.Extension().Lower() == _FileType)
			{
				_FileList.push_back(File);
			}
		}

	} while (FindNextFileW(hFind, &fd));

	FindClose(hFind);
}

GJHGameEngineFile GJHGameEngineDirectory::FindOneFileInDirectory(const GJHGameEngineString& _DirPath, const GJHGameEngineString& _FileName)
{
	WIN32_FIND_DATAW fd;
	HANDLE hFind = FindFirstFileW((_DirPath + L"*.*").c_str(), &fd);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		return GJHGameEngineFile();
	}

	do
	{
		if (lstrcmpW(fd.cFileName, L".") == 0
			|| lstrcmpW(fd.cFileName, L"..") == 0)
		{
			continue;
		}

		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			GJHGameEngineFile File = FindOneFileInDirectory(_DirPath + fd.cFileName + L"\\", _FileName);
			if (File.FullPath() != "")
			{
				return File;
			}
		}

		GJHGameEngineFile File = GJHGameEngineFile(_DirPath + fd.cFileName);
		if (File.FileName() == _FileName && fd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
		{
			return File;
		}

	} while (FindNextFileW(hFind, &fd));

	FindClose(hFind);

	return GJHGameEngineFile();
}

GJHGameEngineFile GJHGameEngineDirectory::FindFile(const GJHGameEngineString& _FileName)
{
	if (Type() != PATHTYPE::DIR)
	{
		MsgAssert("if (Type() != PATHTYPE::DIR)");
		return GJHGameEngineFile();
	}

	return FindOneFileInDirectory(m_PathStr, _FileName);
}

GJHGameEngineDirectory::GJHGameEngineDirectory(const GJHGameEngineString& _FilePath) :
	GJHGameEnginePath(_FilePath)
{

}

GJHGameEngineDirectory::GJHGameEngineDirectory() :
	GJHGameEnginePath(L"")
{
	wchar_t Path[512] = { 0, };
	GetCurrentDirectoryW(512, Path);

	m_PathStr = Path;
	m_PathStr += L"\\";
}

void GJHGameEngineDirectory::MoveParent()
{
	if (Type() != PATHTYPE::DIR)
	{
		MsgAssert("Type() != PATHTYPE::DIR");
	}

	size_t Test = m_PathStr.GetSize();

	size_t LastCharPos = m_PathStr.LastFind(L"\\", m_PathStr.GetSize() - 2);

	if (LastCharPos == GJHGameEngineString::FindError)
	{
		MsgAssert("LastChar == GJHGameEngineString::FindError");
	}

	m_PathStr = m_PathStr.Cut(0, LastCharPos + 1);
}

void GJHGameEngineDirectory::MoveParent(const GJHGameEngineString& _ParentName)
{
	while (_ParentName != DirectoryName())
	{
		MoveParent();
	}
}

void GJHGameEngineDirectory::Move(const GJHGameEngineString& _ParentName)
{
	if (Type() != PATHTYPE::DIR)
	{
		MsgAssert("Type() != PATHTYPE::DIR");
	}

	if (IsExist(m_PathStr + _ParentName + L"\\") == false)
	{
		MsgAssert("IsExsit(m_PathStr + _ParentName + \"\\\") == false");
	}

	m_PathStr += _ParentName + L"\\";
}

GJHGameEngineString GJHGameEngineDirectory::PlusFilePath(const GJHGameEngineString& _FileName)
{
	return m_PathStr + _FileName;
}