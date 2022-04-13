#include "BasePre.h"
#include "GJHGameEngineString.h"

const size_t GJHGameEngineString::FindError = std::wstring::npos;

void GJHGameEngineString::AppendfInternal(const char* fmt, ...)
{
	int BufferSize = 512;
	char StartingBuffer[512];
	char* Buffer = StartingBuffer;

	va_list ap;
	va_start(ap, fmt);
	int Result = vsprintf_s(Buffer, BufferSize, fmt, ap);
	va_end(ap);

	if (BufferSize <= Result)
	{
		Buffer = nullptr;
		while (Result == -1)
		{
			BufferSize *= 2;
			Buffer = (char*)malloc(BufferSize * sizeof(char));
			va_start(ap, fmt);
			Result = vsprintf_s(Buffer, BufferSize, fmt, ap);
			va_end(ap);
		};
	}

	if (Buffer == nullptr)
	{
		return;
	}

	Buffer[Result] = 0;

	m_String += CA2W(Buffer).m_psz;

	if (BufferSize != 512)
	{
		free(Buffer);
	}
}


void GJHGameEngineString::AppendfInternal(const wchar_t* fmt, ...)
{
	int BufferSize = 512;
	wchar_t	StartingBuffer[512];
	wchar_t* Buffer = StartingBuffer;

	va_list ap;
	va_start(ap, fmt);
	int Result = vswprintf_s(Buffer, BufferSize, fmt, ap);
	va_end(ap);

	if (BufferSize <= Result)
	{
		Buffer = nullptr;
		while (Result == -1)
		{
			BufferSize *= 2;
			Buffer = (wchar_t*)malloc(BufferSize * sizeof(wchar_t));
			va_start(ap, fmt);
			Result = vswprintf_s(Buffer, BufferSize, fmt, ap);
			va_end(ap);
		};
	}

	if (Buffer == nullptr)
	{
		return;
	}

	Buffer[Result] = 0;

	m_String += Buffer;

	if (BufferSize != 512)
	{
		free(Buffer);
	}
}