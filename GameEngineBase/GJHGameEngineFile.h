#pragma once
#include "GJHGameEnginePath.h"
#include <set>

enum class OPENMODE
{
	NONE,
	READ,
	WRITE
};

class GJHGameEngineDirectory;
class GJHGameEngineFile : public GJHGameEnginePath
{
public:
	GJHGameEngineDirectory Directory() const;

public:
	GJHGameEngineString Extension() const;

public:
	OPENMODE m_Mode;
	FILE* m_FilePointer;

public:
	void Open(const GJHGameEngineString& _Path, const GJHGameEngineString& _Mode);
	void Open(const GJHGameEngineString& _Mode);

public:
	template<typename T>
	void operator<<(const T& _Data)
	{
		Write(&_Data, sizeof(_Data));
	}

	template<>
	void operator <<(const GJHGameEngineString& _Data)
	{
		int StrSize = static_cast<int>(_Data.GetSize());
		Write(&StrSize, sizeof(int));

		if (StrSize != 0)
		{
			Write(static_cast<const void*>(&_Data.ConstAt(0)), sizeof(wchar_t) * StrSize);
		}
	}

	template<typename DataType>
	void operator <<(const std::vector<DataType>& _Data)
	{
		int StrSize = static_cast<int>(_Data.size());
		Write(&StrSize, sizeof(int));

		for (size_t i = 0; i < _Data.size(); i++)
		{
			this->operator<<(_Data[i]);
		}
	}

	template<typename KeyType, typename ValueType>
	void operator <<(const std::map<KeyType, ValueType>& _Data)
	{
		int MapSize = static_cast<int>(_Data.size());
		Write(&MapSize, sizeof(int));

		for (auto& Data : _Data)
		{
			this->operator<<(Data.first);
			this->operator<<(Data.second);
		}
	}

	template<typename KeyType, typename ValueType>
	void operator <<(const std::map<KeyType, std::vector<ValueType>>& _Data)
	{
		int MapSize = static_cast<int>(_Data.size());
		Write(&MapSize, sizeof(int));

		for (auto& Data : _Data)
		{
			this->operator<<(Data.first);

			int VectorSize = static_cast<int>(Data.size());
			Write(&VectorSize, sizeof(int));

			for (auto& VecData : Data.second)
			{
				this->operator<<(VecData);
			}
		}
	}

	template<typename KeyType, typename ValueType>
	void operator <<(const std::map<KeyType, std::set<ValueType>>& _Data)
	{
		int MapSize = static_cast<int>(_Data.size());
		Write(&MapSize, sizeof(int));

		for (auto& Data : _Data)
		{
			this->operator<<(Data.first);

			int SetSize = static_cast<int>(Data.second.size());
			Write(&SetSize, sizeof(int));

			for (auto& SetData : Data.second)
			{
				this->operator<<(SetData);
			}
		}
	}

	template<typename T>
	void operator >>(T& _Data)
	{
		Read(&_Data, sizeof(_Data));
	}

	template<typename DataType>
	void operator >>(std::vector<DataType>& _Data)
	{
		int StrSize = static_cast<int>(_Data.size());
		Read(&StrSize, sizeof(int));

		_Data.resize(StrSize);

		for (size_t i = 0; i < StrSize; i++)
		{
			this->operator>>(_Data[i]);
		}
	}

	template<>
	void operator >>(GJHGameEngineString& _Data)
	{
		int StrSize = 0;
		Read(&StrSize, sizeof(int));

		if (StrSize != 0)
		{
			_Data.Resize(StrSize);
			Read(&_Data[0], sizeof(wchar_t) * StrSize);
		}
	}

	template<typename KeyType, typename ValueType>
	void operator >>(std::map<KeyType, ValueType>& _Data)
	{
		int MapSize = 0;
		Read(&MapSize, sizeof(int));

		for (int i = 0; i < MapSize; i++)
		{
			KeyType Key;
			this->operator>>(Key);

			ValueType Value;
			this->operator>>(Value);

			_Data.insert(std::make_pair(Key, Value));
		}
	}

	template<typename KeyType, typename ValueType>
	void operator >>(std::map<KeyType, std::vector<ValueType>>& _Data)
	{
		int MapSize = 0;
		Read(&MapSize, sizeof(int));

		for (int i = 0; i < MapSize; i++)
		{
			KeyType Key;
			this->operator>>(Key);

			_Data.insert(std::make_pair(Key, std::vector<ValueType>()));

			int VectorSize = 0;
			Read(&VectorSize, sizeof(int));

			for (int j = 0; j < VectorSize; j++)
			{
				ValueType Value;
				this->operator>>(Value);

				_Data[Key].push_back(Value);
			}
		}
	}

	template<typename KeyType, typename ValueType>
	void operator >>(std::map<KeyType, std::set<ValueType>>& _Data)
	{
		int MapSize = 0;
		Read(&MapSize, sizeof(int));

		for (int i = 0; i < MapSize; i++)
		{
			KeyType Key;
			this->operator>>(Key);

			_Data.insert(std::make_pair(Key, std::set<ValueType>()));

			int SetSize = 0;
			Read(&SetSize, sizeof(int));

			for (int j = 0; j < SetSize; j++)
			{
				ValueType Value;
				this->operator>>(Value);

				_Data[Key].insert(Value);
			}
		}
	}

public:
	void Write(const void* _Data, int _Size, int _Count = 1);
	void Read(void* _Data, int _BufferSize, int _ReadSize, int _Count = 1);
	void Read(void* _Data, int _ReadSize, int _Count = 1);

	void Close();

public:
	GJHGameEngineFile();
	GJHGameEngineFile(const GJHGameEngineString& _Path);
	~GJHGameEngineFile();
};