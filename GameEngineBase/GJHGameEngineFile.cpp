#include "BasePre.h"
#include "GJHGameEngineFile.h"
#include "GJHGameEngineDirectory.h"
#include "GJHGameEngineDebug.h"

GJHGameEngineDirectory GJHGameEngineFile::Directory() const
{
	GJHGameEngineString DirectoryPath = m_PathStr.Cut(0, m_PathStr.LastFind(L"\\") + 1);
	return GJHGameEngineDirectory(DirectoryPath);
}

GJHGameEngineString GJHGameEngineFile::Extension() const
{
	GJHGameEngineString Extension = m_PathStr.Cut(m_PathStr.LastFind(L".") + 1, m_PathStr.GetSize() - m_PathStr.LastFind(L"."));

	return Extension;
}

GJHGameEngineFile::GJHGameEngineFile() :
	GJHGameEnginePath(L""),
	m_FilePointer(nullptr)
{

}

GJHGameEngineFile::GJHGameEngineFile(const GJHGameEngineString& _Path) :
	GJHGameEnginePath(_Path),
	m_Mode(OPENMODE::NONE),
	m_FilePointer(nullptr)
{
}

GJHGameEngineFile::~GJHGameEngineFile()
{
	Close();
}

void GJHGameEngineFile::Open(const GJHGameEngineString& _Path, const GJHGameEngineString& _Mode)
{
	m_PathStr = _Path;

	_wfopen_s(&m_FilePointer, _Path.c_str(), _Mode.c_str());

	if (_Mode.ConstAt(0) == L'w')
	{
		m_Mode = OPENMODE::WRITE;
	}
	else if (_Mode.ConstAt(0) == L'r')
	{
		m_Mode = OPENMODE::READ;
	}
	else
	{
		GJHGameEngineDebug::AssertMsg("if (m_FilePointer == nullptr)");
	}

	if (m_FilePointer == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (m_FilePointer == nullptr)");
	}
}

void GJHGameEngineFile::Open(const GJHGameEngineString& _Mode)
{
	Open(m_PathStr, _Mode);
}

void GJHGameEngineFile::Write(const void* _Data, int _Size, int _Count)
{
	if (m_Mode != OPENMODE::WRITE)
	{
		GJHGameEngineDebug::AssertMsg("if (m_Mode != OPENMODE::WRITE)");
	}

	if (m_FilePointer == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (m_FilePointer == nullptr)");
	}

	fwrite(_Data, _Size, _Count, m_FilePointer);
}

void GJHGameEngineFile::Read(void* _Data, int _ReadSize, int _Count) {

	Read(_Data, _ReadSize, _ReadSize, _Count);
}

void GJHGameEngineFile::Read(void* _Data, int _BufferSize, int _ReadSize, int _Count)
{
	if (m_Mode != OPENMODE::READ)
	{
		GJHGameEngineDebug::AssertMsg("if (m_Mode != OPENMODE::WRITE)");
	}

	if (m_FilePointer == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("if (m_FilePointer == nullptr)");
	}

	fread_s(_Data, _BufferSize, _ReadSize, _Count, m_FilePointer);
}

void GJHGameEngineFile::Close()
{
	if (m_FilePointer != nullptr)
	{
		fclose(m_FilePointer);
		m_FilePointer = nullptr;
	}
}