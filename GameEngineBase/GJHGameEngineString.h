#pragma once
#include <string>
#include <atlstr.h>
#include "GJHGameEngineMath.h"

class GJHGameEngineString
{
public:
	static const size_t FindError;

private:
	std::wstring m_String;

public:
	template<typename FmtType, typename... TemplateTypes>
	void Appendf(const FmtType& Fmt, TemplateTypes... Args)
	{
		AppendfInternal(Fmt, Args...);
	}

	void AppendfInternal(const char* fmt, ...);
	void AppendfInternal(const wchar_t* fmt, ...);

	bool IsEmpty()
	{
		return GetSize() == 0;
	}

	void Resize(size_t _ReSize)
	{
		m_String.resize(_ReSize);
	}

	size_t GetSize() const
	{
		return m_String.size();
	}

	void SetUpper()
	{
		for (size_t i = 0; i < m_String.size(); i++)
		{
			m_String[i] = toupper(m_String[i]);
		}
	}

	size_t Find(const wchar_t* _FindChar) const
	{
		return m_String.find(_FindChar);
	}

	size_t LastFind(const wchar_t* _FindChar, size_t _Offset = FindError) const
	{
		return m_String.find_last_of(_FindChar, _Offset);
	}

	void Erase(size_t _Start, size_t _Len)
	{
		m_String.erase(_Start, _Len);

		return;
	}

	GJHGameEngineString Cut(size_t _Start, size_t _Len) const
	{
		GJHGameEngineString ReturnString = m_String.substr(_Start, _Len);
		return ReturnString;
	}

	std::string GetConvertMultiByteString(int _encoding = CP_ACP) const
	{
		return CW2A(m_String.c_str(), _encoding).m_psz;
	}

	std::string GetConvertMultiByteStringUTF8() const
	{
		return CW2A(m_String.c_str(), CP_UTF8).m_psz;
	}

	static GJHGameEngineString ConvertMultiByteString(const char* _Multi, int _encoding = CP_ACP)
	{
		GJHGameEngineString m_String = CA2W(_Multi, _encoding).m_psz;
		return m_String;
	}

	void SetConvertMultiByteString(const char* _Multi)
	{
		m_String = CA2W(_Multi).m_psz;
	}

	const wchar_t& ConstAt(size_t _Index) const
	{
		return m_String.at(_Index);
	}

	wchar_t& operator[](size_t _Index) 
	{
		return m_String.at(_Index);
	}

	const wchar_t* c_str() const
	{
		return m_String.c_str();
	}

	const wchar_t* GetConstStringPtr() const
	{
		return m_String.c_str();
	}

	bool operator<(const GJHGameEngineString& _Other) const
	{
		return m_String < _Other.m_String;
	}

	bool operator>(const GJHGameEngineString& _Other) const
	{
		return m_String > _Other.m_String;
	}

	bool operator==(const GJHGameEngineString& _Other) const
	{
		return m_String == _Other.m_String;
	}

	bool operator!=(const GJHGameEngineString& _Other) const
	{
		return m_String != _Other.m_String;
	}

	GJHGameEngineString operator+(const GJHGameEngineString& _Other) const
	{
		return m_String + _Other.m_String;
	}

	GJHGameEngineString& operator+=(const GJHGameEngineString& _Other)
	{
		m_String += _Other.m_String;

		return *this;
	}

	GJHGameEngineString& operator=(const std::wstring& _Other)
	{
		m_String = _Other;

		return *this;
	}

public:
	static GJHGameEngineString ToString(int _Value)
	{
		wchar_t Arr[256];
		_itow_s(_Value, Arr, 10);

		return Arr;
	}

	template<typename T>
	static GJHGameEngineString ToString(T& _Value)
	{
		return std::to_wstring(_Value);
	}

	static GJHGameEngineString ToString(float4 _Value)
	{
		GJHGameEngineString Str = L"X : ";
		Str += ToString(_Value.x);
		Str += L" Y : ";
		Str += ToString(_Value.y);
		Str += L" Z : ";
		Str += ToString(_Value.z);
		Str += L" W : ";
		Str += ToString(_Value.w);

		return Str;
	}

	static GJHGameEngineString ToString(GJHGameEngineString& _Value)
	{
		return _Value;
	}

	GJHGameEngineString Upper() const
	{
		std::wstring Text = m_String;

		for (size_t i = 0; i < Text.size(); i++)
		{
			Text[i] = toupper(Text[i]);
		}

		return Text;
	}

	GJHGameEngineString Lower() const
	{
		std::wstring Text = m_String;

		for (size_t i = 0; i < Text.size(); i++)
		{
			Text[i] = tolower(Text[i]);
		}

		return Text;
	}

public:
	template<typename ... REST>
	static GJHGameEngineString TextCreate(REST ... _Arg)
	{
		GJHGameEngineString Text;
		TextPush(L"", Text, _Arg...);

		return Text;
	}

	template<typename ... REST>
	static GJHGameEngineString TextCreateInter(const GJHGameEngineString& _Inter, REST ... _Arg)
	{
		GJHGameEngineString Text;
		TextPush(_Inter, Text, _Arg...);

		return Text;
	}

private:
	template<typename Type, typename ... REST>
	static GJHGameEngineString TextPush(const GJHGameEngineString& _Inter, GJHGameEngineString& _Text, Type& _Data, REST ... _Arg)
	{
		_Text += _Inter;
		_Text += ToString(_Data);
		
		return TextPush(_Inter, _Text, _Arg...);
	}

	template<typename Type>
	static GJHGameEngineString TextPush(const GJHGameEngineString& _Inter, GJHGameEngineString& _Text, Type& _Data)
	{
		_Text += _Inter;
		_Text += ToString(_Data);

		return _Text;
	}

	template<typename ... REST>
	static GJHGameEngineString TextPush(const GJHGameEngineString& _Inter, GJHGameEngineString& _Text, const GJHGameEngineString& _Data, REST ... _Arg)
	{
		_Text += _Inter;
		_Text += _Data;

		return TextPush(_Inter, _Text, _Arg...);
	}

	template<>
	static GJHGameEngineString TextPush(const GJHGameEngineString& _Inter, GJHGameEngineString& _Text, const GJHGameEngineString& _Data)
	{
		_Text += _Inter;
		_Text += _Data;

		return _Text;
	}

	template<typename ... REST>
	static GJHGameEngineString TextPush(const GJHGameEngineString& _Inter, GJHGameEngineString& _Text, const wchar_t* _Data, REST ... _Arg)
	{
		_Text += _Inter;
		_Text += _Data;

		return TextPush(_Inter, _Text, _Arg...);
	}

	template<>
	static GJHGameEngineString TextPush(const GJHGameEngineString& _Inter, GJHGameEngineString& _Text, const wchar_t* _Data)
	{
		_Text += _Inter;
		_Text += _Data;

		return _Text;
	}

public:
	GJHGameEngineString() :
		m_String(L"")
	{

	}

	GJHGameEngineString(int _Value) :
		m_String()
	{
		*this = ToString(_Value);
	}

	GJHGameEngineString(const char* _String)
	{
		*this = GJHGameEngineString::ConvertMultiByteString(_String);
	}

	GJHGameEngineString(const wchar_t* _String)
		: m_String(_String)
	{

	}

	GJHGameEngineString(const std::wstring& _String) :
		m_String(_String)
	{

	}

	GJHGameEngineString(char* _String)
	{
		std::string Str = _String;
		m_String.assign(Str.begin(), Str.end());
	}
};