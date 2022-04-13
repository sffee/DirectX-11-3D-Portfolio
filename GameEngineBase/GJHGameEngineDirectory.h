#pragma once

#include <vector>
#include "GJHGameEngineFile.h"

class GJHGameEngineDirectory : public GJHGameEnginePath
{
public:
	GJHGameEngineFile FindFile(const GJHGameEngineString& _FileName);
	GJHGameEngineFile FindOneFileInDirectory(const GJHGameEngineString& _DirPath, const GJHGameEngineString& _FileName);

public:
	std::vector<GJHGameEngineFile> AllFile(const GJHGameEngineString& _FileType = L"png", bool _IsSubDirectory = true);

private:
	void FindFileInDirectory(const GJHGameEngineString& _DirPath, const GJHGameEngineString& _FileType, std::vector<GJHGameEngineFile>& _FileList, bool _IsSubDirectory = true);

public:
	void MoveParent();
	void MoveParent(const GJHGameEngineString& _ParentName);
	void Move(const GJHGameEngineString& _ParentName);

	GJHGameEngineString PlusFilePath(const GJHGameEngineString& _FileName);

public:
	GJHGameEngineDirectory(const GJHGameEngineString& _FilePath);
	GJHGameEngineDirectory();
};

