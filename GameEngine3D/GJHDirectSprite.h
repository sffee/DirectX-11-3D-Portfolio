#pragma once
#include "GJHDirectTexture2D.h"
#include <GJHGameEngineMath.h>

class GJHDirectSprite : public GJHResBase, public GJHDirectResourcesManager<GJHDirectSprite>
{
public:
	static std::shared_ptr<GJHDirectSprite> Create(const GJHGameEngineString& _Name, float4 _CutCount)
	{
		return Create(_Name, _Name, _CutCount);
	}

	static std::shared_ptr<GJHDirectSprite> Create(const GJHGameEngineString& _Name, const GJHGameEngineString& _TexName, float4 _CutCount)
	{
		GJHDirectSprite* NewRes = new GJHDirectSprite();
		NewRes->CreateRes(_TexName, _CutCount);

		std::shared_ptr<GJHDirectSprite> ReturnPtr = InsertRes(_Name, NewRes);

		return ReturnPtr;
	}

	static void Create(const GJHGameEngineString& _TexName, const GJHGameEngineString& _Ext = L"png")
	{
		int Count = 0;
		while (true)
		{
			std::shared_ptr<GJHDirectTexture2D> Texture = GJHDirectTexture2D::Find(_TexName + L"_" + Count + L"." + _Ext);
			if (Texture != nullptr)
			{
				++Count;
				continue;
			}

			break;
		}

		GJHDirectSprite* NewRes = new GJHDirectSprite();
		int i = 0;
		do
		{
			GJHGameEngineString Name = (Count == 0) ? _TexName : _TexName + L"_" + i;
			NewRes->CreateResPiece(Name + L"." + _Ext);

			++i;
		} while (i < Count);

		InsertRes(_TexName, NewRes);
	}

private:
	std::shared_ptr<GJHDirectTexture2D> m_Tex;
	std::vector<float4> m_CutData;
	float4 m_CutDataSize;
	std::vector<std::shared_ptr<GJHDirectTexture2D>> m_TexPiece;

public:
	std::shared_ptr<GJHDirectTexture2D> GetTexture()
	{
		return m_Tex;
	}

	float4 GetCutData(size_t _Index)
	{
		return m_CutData[_Index];
	}

	float4 GetCutDataCount()
	{
		return float4(m_CutDataSize.x, m_CutDataSize.y);
	}

	int GetCutDataXCount()
	{
		return m_CutDataSize.ix();
	}

	int GetCutDataYCount()
	{
		return m_CutDataSize.iy();
	}

	float4 GetCutDataSize()
	{
		return float4(m_CutDataSize.z, m_CutDataSize.w);
	}

	int GetCutDataXSize()
	{
		return (int)m_CutDataSize.z;
	}

	int GetCutDataYSize()
	{
		return (int)m_CutDataSize.w;
	}

	size_t SpriteSize()
	{
		return m_CutData.size();
	}

	size_t GetPieceSpriteSize()
	{
		return m_TexPiece.size();
	}
	
	std::shared_ptr<GJHDirectTexture2D> GetPieceSprite(int _Index)
	{
		return m_TexPiece[_Index];
	}

public:
	GJHDirectSprite();
	~GJHDirectSprite();

public:
	GJHDirectSprite(const GJHDirectSprite& _Other) = delete;
	GJHDirectSprite(const GJHDirectSprite&& _Other) = delete;

public:
	GJHDirectSprite operator=(const GJHDirectSprite& _Other) = delete;
	GJHDirectSprite operator=(const GJHDirectSprite&& _Other) = delete;

public:
	void CreateRes(const GJHGameEngineString& _TexName, float4 _Size);
	void CreateResPiece(const GJHGameEngineString& _TexName);
};