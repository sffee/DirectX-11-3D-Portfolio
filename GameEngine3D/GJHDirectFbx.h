#pragma once
#include "GJHResBase.h"
#include "GJHDirectResourcesManager.h"
#include <GJHGameEnginePath.h>
#include <vector>
#include "GJHVertexHeader.h"
#include "GJHDirectVertexBuffer.h"
#include "GJHDirectIndexBuffer.h"
#include "GJHDirectMesh.h"
#include "GJHDirectTexture2D.h"
#include <GJHGameEngineFile.h>

#include <algorithm>
#include <fbxsdk.h>

#pragma comment (lib, "zlib-md.lib")
#pragma comment (lib, "libxml2-md.lib")
#pragma comment (lib, "libfbxsdk-md.lib")

struct JointPos
{
	float4 Scale;
	float4 Rotation;
	float4 Translation;

	float4 GlobalScale;
	float4 GlobalRotation;
	float4 GlobalTranslation;

	float4x4 Offset;
	float4x4 SotredOffset;
	float4x4 Local;
	float4x4 Global;

	float Length;
	float XSize;
	float YSize;
	float ZSize;

	void BuildMatrix()
	{
		Global = float4x4::Transformation(GlobalScale, GlobalRotation, GlobalTranslation);
		Local = float4x4::Transformation(Scale, Rotation, Translation);
		Offset = Global.InverseReturn();
	}

	void SetScale(fbxsdk::FbxVector4& scale)
	{
		Scale.x = (float)scale.mData[0];
		Scale.y = (float)scale.mData[1];
		Scale.z = (float)scale.mData[2];
		Scale.w = 0.0f;
	}

	void SetRotation(fbxsdk::FbxQuaternion& quaternion)
	{
		Rotation.x = (float)quaternion.mData[0];
		Rotation.y = (float)quaternion.mData[1];
		Rotation.z = -(float)quaternion.mData[2];
		Rotation.w = -(float)quaternion.mData[3];
	}

	void SetTranslation(fbxsdk::FbxVector4& translation)
	{
		Translation.x = (float)translation.mData[0];
		Translation.y = (float)translation.mData[1];
		Translation.z = -(float)translation.mData[2];
		Translation.w = 0.0f;
	}

	void SetGlobalScale(fbxsdk::FbxVector4& scale)
	{
		GlobalScale.x = (float)scale.mData[0];
		GlobalScale.y = (float)scale.mData[1];
		GlobalScale.z = (float)scale.mData[2];
		GlobalScale.w = 0.0f;
	}

	void SetGlobalRotation(fbxsdk::FbxQuaternion& quaternion)
	{
		GlobalRotation.x = (float)quaternion.mData[0];
		GlobalRotation.y = (float)quaternion.mData[1];
		GlobalRotation.z = -(float)quaternion.mData[2];
		GlobalRotation.w = -(float)quaternion.mData[3];
	}

	void SetGlobalTranslation(fbxsdk::FbxVector4& translation)
	{
		GlobalTranslation.x = (float)translation.mData[0];
		GlobalTranslation.y = (float)translation.mData[1];
		GlobalTranslation.z = -(float)translation.mData[2];
		GlobalTranslation.w = 0.0f;
	}

	void SetFromMatrix(const float4x4& InMatrix)
	{
		float4x4 M = InMatrix;

		Scale = M.ExtractScaling();
		Scale.w = 0.0;

		if (InMatrix.Determinant() < 0.f)
		{
			float4 FloatMinus1_111(-1.f, 1.f, 1.f, 1.f);
			Scale *= FloatMinus1_111;
			float4 XAxis = -float4(M.Arr2D[0][0], M.Arr2D[0][1], M.Arr2D[0][2], 0.0f);
			M.Arr2D[0][0] = XAxis.x;
			M.Arr2D[0][1] = XAxis.w;
			M.Arr2D[0][2] = XAxis.z;
		}

		Rotation = float4::MatrixToQuaternion(M);
		Translation = float4(InMatrix.Arr2D[3][0], InMatrix.Arr2D[3][1], InMatrix.Arr2D[3][2], 0.0f);

		__m128 Temp1 = _mm_mul_ps(Rotation.DirectVector, Rotation.DirectVector);
		__m128 Temp2 = _mm_shuffle_ps(Temp1, Temp1, _MM_SHUFFLE(1, 0, 3, 2));
		Temp1 = _mm_add_ps(Temp1, Temp2);
		Temp2 = _mm_shuffle_ps(Temp1, Temp1, _MM_SHUFFLE(0, 3, 2, 1));
		__m128 SquareSum = _mm_add_ps(Temp1, Temp2);
		const __m128 NonZeroMask = _mm_cmpge_ps(SquareSum, _mm_setr_ps(1.e-8f, 1.e-8f, 1.e-8f, 1.e-8f));
		const __m128 OneHalf = _mm_setr_ps(0.5f, 0.5f, 0.5f, 0.5f);
		const __m128 VecDivBy2 = _mm_mul_ps(SquareSum, OneHalf);

		const __m128 x0 = _mm_rsqrt_ps(SquareSum);

		__m128 x1 = _mm_mul_ps(x0, x0);
		x1 = _mm_sub_ps(OneHalf, _mm_mul_ps(VecDivBy2, x1));
		x1 = _mm_add_ps(_mm_mul_ps(x0, x1), x0);

		__m128 x2 = _mm_mul_ps(x1, x1);
		x2 = _mm_sub_ps(OneHalf, _mm_mul_ps(VecDivBy2, x2));
		const __m128 InvLength = _mm_add_ps(_mm_mul_ps(x1, x2), x1);

		const __m128 NormalizedVector = _mm_mul_ps(InvLength, Rotation.DirectVector);
		__m128 DefaultValue = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
		Rotation.DirectVector = _mm_xor_ps(DefaultValue, _mm_and_ps(NonZeroMask, _mm_xor_ps(NormalizedVector, DefaultValue)));
	}

	void ToMatrixInternal(__m128& OutDiagonals, __m128& OutAdds, __m128& OutSubtracts) const
	{
		const __m128 RotationX2Y2Z2 = _mm_add_ps(Rotation.DirectVector, Rotation.DirectVector);
		const __m128 RotationXX2YY2ZZ2 = _mm_mul_ps(RotationX2Y2Z2, Rotation.DirectVector);	

		const __m128 yy2_xx2_xx2 = _mm_shuffle_ps(RotationXX2YY2ZZ2, RotationXX2YY2ZZ2, _MM_SHUFFLE(0, 0, 0, 1));
		const __m128 zz2_zz2_yy2 = _mm_shuffle_ps(RotationXX2YY2ZZ2, RotationXX2YY2ZZ2, _MM_SHUFFLE(0, 1, 2, 2));
		const __m128 DiagonalSum = _mm_add_ps(yy2_xx2_xx2, zz2_zz2_yy2);
		const __m128 Diagonals = _mm_sub_ps(float4::ONE.DirectVector, DiagonalSum);
		OutDiagonals = _mm_mul_ps(Diagonals, Scale.DirectVector);
		
		const __m128 x_y_x = _mm_shuffle_ps(Rotation.DirectVector, Rotation.DirectVector, _MM_SHUFFLE(0, 0, 1, 0)); // 1, 2, 2, 0
		const __m128 y2_z2_z2 = _mm_shuffle_ps(RotationX2Y2Z2, RotationX2Y2Z2, _MM_SHUFFLE(0, 2, 2, 1));
		const __m128 RotBase = _mm_mul_ps(x_y_x, y2_z2_z2);

		const __m128 w_w_w = _mm_shuffle_ps(Rotation.DirectVector, Rotation.DirectVector, _MM_SHUFFLE(3, 3, 3, 3));
		const __m128 z2_x2_y2 = _mm_shuffle_ps(RotationX2Y2Z2, RotationX2Y2Z2, _MM_SHUFFLE(0, 1, 0, 2));
		const __m128 RotOffset = _mm_mul_ps(w_w_w, z2_x2_y2);

		const __m128 Adds = _mm_add_ps(RotBase, RotOffset);
		OutAdds = _mm_mul_ps(Adds, Scale.DirectVector);
		const __m128 Scale3DYZXW = _mm_shuffle_ps(Scale.DirectVector, Scale.DirectVector, _MM_SHUFFLE(3, 0, 2, 1));
		const __m128 Subtracts = _mm_sub_ps(RotBase, RotOffset);
		OutSubtracts = _mm_mul_ps(Subtracts, Scale3DYZXW);
	}

	float4x4 ToMatrixWithScale()
	{
		float4x4 OutMatrix;
		__m128 DiagonalsXYZ;
		__m128 Adds;
		__m128 Subtracts;

		ToMatrixInternal(DiagonalsXYZ, Adds, Subtracts);
		union { __m128 v; __m128i i; } tmp;
		tmp.i = _mm_setr_epi32((unsigned int)0xffffffff, (unsigned int)0xffffffff, (unsigned int)0xffffffff, (unsigned int)0x00000000);
		const __m128 DiagonalsXYZ_W0 = _mm_and_ps(DiagonalsXYZ, tmp.v);

		const __m128 AddX_DC_DiagX_DC = _mm_shuffle_ps(Adds, DiagonalsXYZ_W0, _MM_SHUFFLE(0, 0, 0, 0));
		const __m128 SubZ_DC_DiagW_DC = _mm_shuffle_ps(Subtracts, DiagonalsXYZ_W0, _MM_SHUFFLE(0, 3, 0, 2));
		const __m128 Row0 = _mm_shuffle_ps(AddX_DC_DiagX_DC, SubZ_DC_DiagW_DC, _MM_SHUFFLE(2, 0, 0, 2));

		const __m128 SubX_DC_DiagY_DC = _mm_shuffle_ps(Subtracts, DiagonalsXYZ_W0, _MM_SHUFFLE(0, 1, 0, 0));
		const __m128 AddY_DC_DiagW_DC = _mm_shuffle_ps(Adds, DiagonalsXYZ_W0, _MM_SHUFFLE(0, 3, 0, 1));
		const __m128 Row1 = _mm_shuffle_ps(SubX_DC_DiagY_DC, AddY_DC_DiagW_DC, _MM_SHUFFLE(2, 0, 2, 0));

		const __m128 AddZ_DC_SubY_DC = _mm_shuffle_ps(Adds, Subtracts, _MM_SHUFFLE(0, 1, 0, 2));
		const __m128 Row2 = _mm_shuffle_ps(AddZ_DC_SubY_DC, DiagonalsXYZ_W0, _MM_SHUFFLE(3, 2, 2, 0));

		_mm_store_ps(&(OutMatrix.Arr2D[0][0]), Row0);
		_mm_store_ps(&(OutMatrix.Arr2D[1][0]), Row1);
		_mm_store_ps(&(OutMatrix.Arr2D[2][0]), Row2);

		__m128 Temp1 = _mm_movehl_ps(float4::ONE.DirectVector, Translation.DirectVector);

		const __m128 Row3 = _mm_shuffle_ps(Translation.DirectVector, Temp1, _MM_SHUFFLE(3, 0, 1, 0));
		_mm_store_ps(&(OutMatrix.Arr2D[3][0]), Row3);

		return OutMatrix;
	}

	void Reset()
	{
		Scale = float4::ONE;
		Rotation = float4::ZERO;
		Translation = float4::ZERO;

		Offset.Iden();
	}

	JointPos()
	{
		Scale = float4::ONE;
		Rotation = float4::ZERO;
		Translation = float4::ZERO;

		Offset.Iden();
	}
};


struct Bone
{
	GJHGameEngineString Name;

	unsigned int Flags;
	int NumChildren;
	int Index;
	int ParentIndex;
	JointPos BonePos;

	Bone()
	{
		Name = L"";
		Flags = 0;
		NumChildren = 0;
		Index = -1;
		ParentIndex = 0;
		BonePos.Reset();
	}
};

class FbxExFaceSettingData
{
public:
	GJHGameEngineString Name;
	float4 DifColor;
	float4 AmbColor;
	float4 SpcColor;
	float4 EmvColor;
	float SpecularPower;
	float TransparencyFactor;
	float Shininess;
	GJHGameEngineString DifTexturePath; 
	GJHGameEngineString BmpTexturePath;
	GJHGameEngineString SpcTexturePath;
	
	GJHGameEngineString DifTextureName; 
	GJHGameEngineString BmpTextureName;
	GJHGameEngineString SpcTextureName;

public:
	FbxExFaceSettingData() {}
	~FbxExFaceSettingData() {}
};


class FbxExIW
{
public:
	int Index;
	double Weight;
};

class HVBBUFFER;
class HIBBUFFER;
struct FbxRenderSet : public std::enable_shared_from_this<FbxRenderSet>
{
public:
	int IsLodLv;
	bool IsLod;

	std::map<FbxMesh*, std::vector<VertexAnimation>*> m_FbxVtxMap;

	std::map<FbxMesh*, std::map<int, std::vector<FbxExIW>>> m_MapWI;

	std::vector<std::vector<VertexAnimation>*> m_FbxVtx;
	std::vector<std::vector<std::vector<UINT>>*> m_FbxIdx;

	std::vector<std::shared_ptr<GJHDirectVertexBuffer>> m_VB;
	std::vector<std::vector<std::shared_ptr<GJHDirectIndexBuffer>>> m_IB;
	std::vector<std::vector<std::shared_ptr<GJHDirectMesh>>> m_Mesh;

	std::vector<std::vector<FbxExFaceSettingData>> m_MatialData;

	FbxRenderSet() :
		IsLod(false),
		IsLodLv(-1)
	{
	}
	~FbxRenderSet()
	{
		for (size_t i = 0; i < m_FbxVtx.size(); i++)
		{
			delete m_FbxVtx[i];
		}

		for (size_t i = 0; i < m_FbxIdx.size(); i++)
		{
			delete m_FbxIdx[i];
		}
	}
};

struct FbxExNodeInfo
{
	const char* ObjectName;
	unsigned __int64 UniqueId;
	fbxsdk::FbxAMatrix Transform;
	fbxsdk::FbxVector4 RotationPivot;
	fbxsdk::FbxVector4 ScalePivot;

	const char* AttributeName;
	unsigned __int64 AttributeUniqueId;
	const char* AttributeType;

	const char* ParentName;
	unsigned __int64 ParentUniqueId;

	FbxExNodeInfo() :
		ObjectName(nullptr),
		AttributeName(nullptr),
		AttributeType(nullptr),
		ParentName(nullptr)
	{
		UniqueId = 0;
		Transform.SetIdentity();
		RotationPivot.Set(0, 0, 0);
		ScalePivot.Set(1, 1, 1);

		AttributeUniqueId = 0;

		ParentUniqueId = 0;
	}
};

struct FbxExMeshInfo
{
	GJHGameEngineString Name;
	fbxsdk::FbxMesh* pMesh;
	bool bTriangulated;

	unsigned __int64 UniqueId;
	int FaceNum;
	int VertexNum;
	int MaterialNum;
	bool bIsSkelMesh;
	GJHGameEngineString SkeletonRoot;
	int SkeletonElemNum;
	bool bIsLodGroup;
	GJHGameEngineString LODGroup;
	int LODLevel;
	int MorphNum;

	FbxExMeshInfo()
	{
		Name = L"";
		UniqueId = 0;
		FaceNum = 0;
		VertexNum = 0;
		bTriangulated = false;
		MaterialNum = 0;
		bIsSkelMesh = false;
		SkeletonRoot = L"";
		SkeletonElemNum = 0;
		bIsLodGroup = false;
		LODGroup = L"";
		LODLevel = 0;
		MorphNum = 0;
	}
};



struct FbxExSceneInfo
{
	int NonSkinnedMeshNum;

	int SkinnedMeshNum;

	int TotalGeometryNum;
	int TotalMaterialNum;
	int TotalTextureNum; 

	std::vector<FbxExMeshInfo> MeshInfo;
	std::vector<FbxExNodeInfo> HierarchyInfo;

	bool bHasAnimation;
	double FrameRate;
	double TotalTime;

	FbxExSceneInfo()
	{
		Reset();
	}

	void Reset()
	{
		NonSkinnedMeshNum = 0;
		SkinnedMeshNum = 0;
		TotalGeometryNum = 0;
		TotalMaterialNum = 0;
		TotalTextureNum = 0;
		MeshInfo.clear();
		HierarchyInfo.clear();
		bHasAnimation = false;
		FrameRate = 0.0;
		TotalTime = 0.0;
	}
};


enum class FbxImportType
{
	Failed,
	Geometry,
	Deformers,
	Otherwise,
};

class FbxExBoneFrameData
{
public:
	float4 S;
	float4 Q;
	float4 T;
	double Time;
	float4x4 FrameMat;

	fbxsdk::FbxAMatrix GlobalAnimation;
	fbxsdk::FbxAMatrix LocalAnimation;
};


class FbxExBoneFrame
{
public:
	int BoneIndex;
	int BoneParentIndex;
	std::vector<FbxExBoneFrameData> m_Data;
};

class FbxClusterData
{
public:
	fbxsdk::FbxCluster* m_Cluster;
	fbxsdk::FbxMesh* m_Mesh;
	GJHGameEngineString LinkName;
};

class FbxExAniData
{
public:
	GJHGameEngineString AniName;
	fbxsdk::FbxTime StartTime;
	fbxsdk::FbxTime EndTime; 
	fbxsdk::FbxLongLong TimeStartCount;
	fbxsdk::FbxLongLong TimeEndCount;
	fbxsdk::FbxLongLong FrameCount;
	fbxsdk::FbxTime::EMode TimeMode;
	__int64 FbxModeCount;
	double FbxModeRate;

	std::vector<FbxExBoneFrame> m_AniFrameData;

public:
	float FrameTime(int _Frame)
	{
		fbxsdk::FbxTime tTime = {};
		tTime.SetFrame(_Frame, TimeMode);

		return (float)tTime.GetSecondDouble();
	}

public:
	__int64 FrameModeRateInt()
	{
		return static_cast<__int64>(fbxsdk::FbxGetFrameRate(TimeMode));
	}
	double FrameModeRateDouble()
	{
		return fbxsdk::FbxGetFrameRate(TimeMode);
	}

	float FrameModeRateFloat()
	{
		return static_cast<float>(fbxsdk::FbxGetFrameRate(TimeMode));
	}


public:
	FbxExAniData() : AniName(L""), StartTime(0), EndTime(0), TimeStartCount(0), TimeEndCount(0), FrameCount(0), FbxModeCount(0), FbxModeRate(0.0)
	{}
	~FbxExAniData() {}
};

class GJHGameEngineFile;
class GJHDirectFbx : public GJHResBase, public GJHDirectResourcesManager<GJHDirectFbx>
{
public:
	static std::shared_ptr<GJHDirectFbx> Load(const GJHGameEngineString& _Path, std::function<void(float)> _MeshProgress = nullptr)
	{
		GJHGameEngineString FileName = GJHGameEnginePath::GetFileName(_Path);
		if (Find(FileName) != nullptr)
		{
			return Find(FileName);
		}

		std::shared_ptr<GJHDirectFbx> NewRes = std::make_shared<GJHDirectFbx>();
		if (NewRes->LoadFbx(_Path, _MeshProgress) == false)
		{
			NewRes = nullptr;
			return nullptr;
		}

		InsertResSheard(FileName, NewRes);

		return NewRes;
	}

	static std::shared_ptr<GJHDirectFbx> LoadAsync(const GJHGameEngineString& _Path, std::function<void(float)> _MeshProgress = nullptr)
	{
		GJHGameEngineString FileName = GJHGameEnginePath::GetFileName(_Path);
		if (FindAsync(FileName) != nullptr)
		{
			return FindAsync(FileName);
		}

		std::shared_ptr<GJHDirectFbx> NewRes = std::make_shared<GJHDirectFbx>();
		if (NewRes->LoadFbx(_Path, _MeshProgress) == false)
		{
			NewRes = nullptr;
			return nullptr;
		}

		InsertResSheardAsync(FileName, NewRes);
		return NewRes;
	}

	static std::shared_ptr<GJHDirectFbx> LoadUserMesh(const GJHGameEngineString& _UserPath)
	{
		GJHGameEngineString FileName = GJHGameEnginePath::GetFileName(_UserPath);
		if (Find(FileName) != nullptr)
		{
			return Find(FileName);
		}

		std::shared_ptr<GJHDirectFbx> NewRes = std::make_shared<GJHDirectFbx>();
		if (NewRes->LoadUserMeshData(_UserPath) == false)
		{
			NewRes = nullptr;
			return nullptr;
		}

		InsertResSheard(FileName, NewRes);
		return NewRes;
	}


	static std::shared_ptr<GJHDirectFbx> LoadUserAnimation(const GJHGameEngineString& _UserPath)
	{
		GJHGameEngineString FileName = GJHGameEnginePath::GetFileName(_UserPath);
		if (Find(FileName) != nullptr)
		{
			return Find(FileName);
		}

		std::shared_ptr<GJHDirectFbx> NewRes = std::make_shared<GJHDirectFbx>();
		if (NewRes->LoadUserAniData(_UserPath) == false)
		{
			NewRes = nullptr;
			return nullptr;
		}

		InsertResSheard(FileName, NewRes);
		return NewRes;
	}

public:
	GJHDirectFbx();
	~GJHDirectFbx();

public:
	GJHDirectFbx(const GJHDirectFbx& _Other) = delete;
	GJHDirectFbx(const GJHDirectFbx&& _Other) = delete;

public:
	std::function<void(float)> MeshProgress;

public:
	fbxsdk::FbxAMatrix m_AxisConversionMatrix;
	fbxsdk::FbxAMatrix m_JointPostConversionMatrix;
	fbxsdk::FbxVector4 m_ModifyRotationAxis;

	fbxsdk::FbxManager* m_Manager;
	fbxsdk::FbxScene* m_Scene;
	fbxsdk::FbxNode* m_RootNode;

	FbxImportType m_ImportType;
	FbxExSceneInfo m_SceneInfo;
	std::map<int, std::vector<FbxClusterData>> m_mapClusterData;

	std::map<GJHGameEngineString, std::vector<Bone*>> m_MapRefBones;
	std::vector<Bone> m_vecRefBones;
	std::map<int, std::shared_ptr<FbxRenderSet>> m_mapRenderSet;
	std::vector<FbxExAniData> m_UserAniData;

	unsigned int AllAniMatirixCount;
	unsigned int CurMatrixCount;
	std::mutex ThreadLock;
	

private:
	static unsigned int AniThreadCal(std::shared_ptr<GJHDirectFbx> _ThisFbx, std::shared_ptr<GJHDirectFbx> _Fbx, fbxsdk::FbxNode* pNode, int Count, int ThreadCount);

private:
	bool LoadFbx(const GJHGameEngineString& _Path, std::function<void(float)> _MeshProgress = nullptr, float4 _Axis = float4::ZERO);
	bool ImportScene(const GJHGameEngineString& _Path);
	bool ConvertScene();
	void AniCheck();
	void ImportSceneInfo();
	void ImportCluster();
	void ImportControlPoints();
	void FbxRenderSetMeshesCreate();
	void LoadSkinAndCluster();

	void ImportBone();

public:
	Bone* FindBone(const GJHGameEngineString& _BoneName);

private:
	fbxsdk::FbxNode* RecursiveFindParentLodGroup(fbxsdk::FbxNode* parentNode);
	fbxsdk::FbxNode* RecursiveGetFirstMeshNode(fbxsdk::FbxNode* Node, fbxsdk::FbxNode* NodeToFind);
	fbxsdk::FbxNode* FindLODGroupNode(fbxsdk::FbxNode* NodeLodGroup, int LodIndex, fbxsdk::FbxNode* NodeToFind = nullptr);
	void TraverseHierarchyNodeRecursively(FbxExSceneInfo& SceneInfo, fbxsdk::FbxNode* ParentNode, FbxExNodeInfo& ParentInfo);
	fbxsdk::FbxAMatrix ComputeTotalMatrix(fbxsdk::FbxNode* Node);
	bool IsOddNegativeScale(const fbxsdk::FbxAMatrix& TotalMatrix);

	bool IsNull(fbxsdk::FbxNode* Link);
	bool IsBone(fbxsdk::FbxNode* Link);
	bool RetrievePoseFromBindPose(fbxsdk::FbxScene* pScene, const std::vector<fbxsdk::FbxNode*>& NodeArray, fbxsdk::FbxArray<fbxsdk::FbxPose*>& PoseArray);
	void BuildSkeletonSystem(fbxsdk::FbxScene* pScene, std::vector<fbxsdk::FbxCluster*>& ClusterArray, std::vector<fbxsdk::FbxNode*>& OutSortedLinks);
	void RecursiveBuildSkeleton(fbxsdk::FbxNode* Link, std::vector<fbxsdk::FbxNode*>& OutSortedLinks);
	fbxsdk::FbxNode* GetRootSkeleton(fbxsdk::FbxScene* pScene, fbxsdk::FbxNode* Link);
	void LoadAnimationVertexData(std::shared_ptr<FbxRenderSet> _DrawData, const std::vector<FbxClusterData>& vecClusterData);
	void DrawSetWeightAndIndexSetting(std::shared_ptr<FbxRenderSet> _DrawSet, fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxCluster* _Cluster, int _BoneIndex);
	void CalAnimationVertexData(std::shared_ptr<FbxRenderSet> _RenderSet);

	fbxsdk::FbxAMatrix GetGeometryTransformation(fbxsdk::FbxNode* pNode);
	void MeshAnimationLoad(std::shared_ptr<GJHDirectFbx> _Fbx, fbxsdk::FbxNode* pNode, std::function<void(float)> _AniProgress);
	void ProcessAnimationLoad(std::shared_ptr<GJHDirectFbx> _Fbx, fbxsdk::FbxNode* pNode, std::function<void(float)> _AniProgress);
	void ProcessAnimationCheckState(std::shared_ptr<GJHDirectFbx> _Fbx);

public:
	size_t GetBoneCount();
	void CalFbxExBoneFrameTransMatrix(std::shared_ptr<GJHDirectFbx> _Mesh, std::function<void(float)> _AniProgress = nullptr);

public:
	static float4 FbxVecTofloat4(const fbxsdk::FbxVector4& _BaseVector);
	static float4 FbxVecToTransform(const fbxsdk::FbxVector4& _BaseVector);
	static float4 FbxQuaternionTofloat4(const fbxsdk::FbxQuaternion& _BaseQ);
	static fbxsdk::FbxAMatrix float4x4ToFbxAMatrix(const float4x4& _MATRIX);
	static float4x4 FbxMatTofloat4x4(const fbxsdk::FbxAMatrix& _BaseTrans);

public:
	GJHGameEngineString MaterialTex(fbxsdk::FbxSurfaceMaterial* pMtrl, const char* _FactorName);
	float4 MaterialColor(fbxsdk::FbxSurfaceMaterial* pMtrl, const char* _ColorName, const char* _FactorName);
	float MaterialFactor(fbxsdk::FbxSurfaceMaterial* pMtrl, const char* _FactorName);
	void RenderSetMaterialSetting(fbxsdk::FbxNode* _Node, std::shared_ptr<FbxRenderSet> _Ptr);

	void LoadUv(fbxsdk::FbxMesh* _Mesh, std::vector<VertexAnimation>& _ArrVtx, int VtxId, int _Index);
	void LoadNormal(fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxAMatrix _MeshMatrix, std::vector<VertexAnimation>& _ArrVtx, int VtxId, int _Index);
	void LoadTangent(fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxAMatrix _MeshMatrix, std::vector<VertexAnimation>& _ArrVtx, int VtxId, int _Index);
	void LoadBinormal(fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxAMatrix _MeshMatrix, std::vector<VertexAnimation>& _ArrVtx, int VtxId, int _Index);

public:
	void FbxRenderSetMeshCreate();
	void FbxRenderSetMeshCreate(std::shared_ptr<FbxRenderSet> _RenderData);

public:
	FbxExFaceSettingData* GetMaterialData(int _RenderSetIndex, int _MatSetIndex, int _MatIndex);

	int GetRenderSetCount();
	int GetMeshSetCount(int _Index);
	int GetMeshCount(int _SetIndex, int _Index);

	std::shared_ptr<GJHDirectMesh> GetMesh(int _MeshSetIndex, int _MeshIndex);
	std::shared_ptr<GJHDirectMesh> GetMesh(int _RenderSetIndex, int _MeshSetIndex, unsigned int _MeshIndex);

	FbxExAniData* GetAniData(int _Index);

	Bone* FindBone(int _Index);

public:
	bool SaveUserMesh(const GJHGameEngineString& _File);
	bool LoadUserMeshData(const GJHGameEngineString& _UserPath);

	bool SaveUserAni(const GJHGameEngineString& _FbxName);
	bool LoadUserAniData(const GJHGameEngineString& _UserPath);

public:
	GJHDirectFbx& operator=(const GJHDirectFbx& _Other) = delete;
	GJHDirectFbx& operator=(const GJHDirectFbx&& _Other) = delete;
};