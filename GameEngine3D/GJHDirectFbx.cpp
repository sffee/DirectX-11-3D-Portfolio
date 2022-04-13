#include "GJHDirectFbx.h"
#include <GJHGameEngineThread.h>

GJHDirectFbx::GJHDirectFbx() :
	m_Scene(nullptr),
	m_Manager(nullptr)
{
}

GJHDirectFbx::~GJHDirectFbx()
{
	m_mapRenderSet.clear();

	if (m_Scene != nullptr)
	{
		m_Scene->Destroy();
		m_Scene = nullptr;
	}

	if (m_Manager != nullptr)
	{
		m_Manager->Destroy();
		m_Manager = nullptr;
	}
}

bool GJHDirectFbx::LoadFbx(const GJHGameEngineString& _Path, std::function<void(float)> _MeshProgress, float4 _Axis)
{
	m_ModifyRotationAxis = { _Axis.x,_Axis.y,_Axis.z, 1 };
	//m_ModifyRotationAxis = { -90.f, _Axis.y,_Axis.z, 1 };

	MeshProgress = _MeshProgress;

	if (ImportScene(_Path) == false)
	{
		return false;
	}

	ConvertScene();
	if (MeshProgress != nullptr)
	{
		MeshProgress(1.0f);
	}

	AniCheck();
	if (MeshProgress != nullptr)
	{
		MeshProgress(5.0f);
	}

	ImportSceneInfo();
	if (MeshProgress != nullptr)
	{
		MeshProgress(30.0f);
	}

	ImportBone();

	ImportCluster();
	if (MeshProgress != nullptr)
	{
		MeshProgress(40.0f);
	}

	ImportControlPoints();
	if (MeshProgress != nullptr)
	{
		MeshProgress(70.0f);
	}

		if (MeshProgress != nullptr)
		{
			MeshProgress(80.0f);
		}

		LoadSkinAndCluster();
		if (MeshProgress != nullptr)
		{
			MeshProgress(90.0f);
		}

	FbxRenderSetMeshCreate();

	if (m_mapRenderSet.size() != 0)
	{
		for (auto RenderSet : m_mapRenderSet)
		{
			auto MatData = RenderSet.second->m_MatialData;
			auto VtxData = RenderSet.second->m_FbxVtx;
			auto IdxData = RenderSet.second->m_FbxIdx;
		}

	}

	if (MeshProgress != nullptr)
	{
		MeshProgress(100.0f);
	}

	m_mapClusterData;
	m_MapRefBones;
	m_vecRefBones;
	m_mapRenderSet;
	m_UserAniData;


	return true;
}

GJHGameEngineString GJHDirectFbx::MaterialTex(fbxsdk::FbxSurfaceMaterial* pMtrl, const char* _FactorName)
{
	FbxProperty TexturePro = pMtrl->FindProperty(_FactorName);
	GJHGameEngineString Str;
	if (true == TexturePro.IsValid())
	{
		int iTexCount = TexturePro.GetSrcObjectCount<FbxFileTexture>();

		if (iTexCount > 0)
		{
			FbxFileTexture* pFileTex = TexturePro.GetSrcObject<FbxFileTexture>(0);

			if (nullptr != pFileTex)
			{
				Str = GJHGameEngineString::ConvertMultiByteString(pFileTex->GetFileName());
			}
		}
		else
		{
			return L"";
		}
	}
	else
	{
		return L"";
	}

	return Str;
}

float GJHDirectFbx::MaterialFactor(fbxsdk::FbxSurfaceMaterial* pMtrl, const char* _FactorName)
{
	double dFactor = 0;
	FbxProperty FactorPro = pMtrl->FindProperty(_FactorName);

	if (true == FactorPro.IsValid())
	{
		dFactor = FactorPro.Get<FbxDouble>();
	}

	return (float)dFactor;
}

float4 GJHDirectFbx::MaterialColor(fbxsdk::FbxSurfaceMaterial* pMtrl, const char* _ColorName, const char* _FactorName)
{
	FbxDouble3 vResult(0, 0, 0);
	double dFactor = 0;
	FbxProperty ColorPro = pMtrl->FindProperty(_ColorName);
	FbxProperty FactorPro = pMtrl->FindProperty(_FactorName);

	if (true == ColorPro.IsValid() && true == FactorPro.IsValid())
	{
		vResult = ColorPro.Get<FbxDouble3>();
		dFactor = FactorPro.Get<FbxDouble>();

		if (dFactor != 1)
		{
			vResult[0] *= dFactor;
			vResult[1] *= dFactor;
			vResult[2] *= dFactor;
		}
	}

	return float4((float)vResult[0], (float)vResult[1], (float)vResult[2]);
}


float4x4 GJHDirectFbx::FbxMatTofloat4x4(const fbxsdk::FbxAMatrix& _BaseTrans)
{
	float4x4 Mat;

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			Mat.Arr2D[y][x] = (float)_BaseTrans.Get(y, x);
		}
	}

	return Mat;
}

fbxsdk::FbxAMatrix GJHDirectFbx::float4x4ToFbxAMatrix(const float4x4& _MATRIX)
{
	fbxsdk::FbxAMatrix mat;
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			mat.mData[y].mData[x] = _MATRIX.Arr2D[y][x];
		}
	}

	return mat;
}

float4 GJHDirectFbx::FbxVecTofloat4(const fbxsdk::FbxVector4& _BaseVector)
{
	float4 Vec;
	Vec.Arr[0] = (float)_BaseVector.mData[0];
	Vec.Arr[1] = (float)_BaseVector.mData[1];
	Vec.Arr[2] = (float)_BaseVector.mData[2];
	Vec.Arr[3] = (float)_BaseVector.mData[3];

	return Vec;

}

float4 GJHDirectFbx::FbxVecToTransform(const fbxsdk::FbxVector4& _BaseVector)
{
	float4 Vec;
	Vec.Arr[0] = (float)_BaseVector.mData[0];
	Vec.Arr[1] = (float)_BaseVector.mData[1];
	Vec.Arr[2] = -(float)_BaseVector.mData[2];
	Vec.Arr[3] = (float)_BaseVector.mData[3];
	return Vec;
}

float4 GJHDirectFbx::FbxQuaternionTofloat4(const fbxsdk::FbxQuaternion& _BaseQ)
{
	float4 Vec;
	Vec.Arr[0] = (float)_BaseQ.mData[0];
	Vec.Arr[1] = (float)_BaseQ.mData[1];
	Vec.Arr[2] = -(float)_BaseQ.mData[2];
	Vec.Arr[3] = -(float)_BaseQ.mData[3];
	return Vec;
}

bool GJHDirectFbx::ImportScene(const GJHGameEngineString& _Path)
{
	m_Manager = fbxsdk::FbxManager::Create();
	if (m_Manager == nullptr)
	{
		GJHGameEngineDebug::AssertMsg("m_Manager create fail");
		return false;
	}

	fbxsdk::FbxIOSettings* PIO = fbxsdk::FbxIOSettings::Create(m_Manager, IOSROOT);
	fbxsdk::FbxImporter* pImporter = fbxsdk::FbxImporter::Create(m_Manager, "");

	if (pImporter->Initialize(_Path.GetConvertMultiByteStringUTF8().c_str(), -1, PIO) == false)
	{
		GJHGameEngineString Error = pImporter->GetStatus().GetErrorString();
		GJHGameEngineDebug::AssertMsg("Fbx 파일 이니셜라이즈 실패");
		return false;
	}

	m_Scene = fbxsdk::FbxScene::Create(m_Manager, "");

	pImporter->Import(m_Scene);

	PIO->Destroy();
	pImporter->Destroy();

	return true;

}

bool GJHDirectFbx::ConvertScene()
{
	fbxsdk::FbxAxisSystem::ECoordSystem CoordSystem = fbxsdk::FbxAxisSystem::ECoordSystem::eRightHanded;
	fbxsdk::FbxAxisSystem::EUpVector UpVector = fbxsdk::FbxAxisSystem::eYAxis;
	fbxsdk::FbxAxisSystem::EFrontVector FrontVector = (fbxsdk::FbxAxisSystem::EFrontVector)-fbxsdk::FbxAxisSystem::eParityOdd;

	fbxsdk::FbxAxisSystem JKEngineAxisSystem(UpVector, FrontVector, CoordSystem);

	fbxsdk::FbxAxisSystem SourceAxisSystem = m_Scene->GetGlobalSettings().GetAxisSystem();

	if (SourceAxisSystem != JKEngineAxisSystem)
	{
		fbxsdk::FbxRootNodeUtility::RemoveAllFbxRoots(m_Scene);
		JKEngineAxisSystem.ConvertScene(m_Scene);

		fbxsdk::FbxAMatrix SourceMatrix;
		SourceAxisSystem.GetMatrix(SourceMatrix);

		fbxsdk::FbxAMatrix JKEngineMatrix;
		JKEngineAxisSystem.GetMatrix(JKEngineMatrix);

		m_AxisConversionMatrix = SourceMatrix.Inverse() * JKEngineMatrix;
	}

	int originUpSign = 0;
	int originFrontSign = 0;
	int engineUpSign = 0;
	int engineFrontSign = 0;

	fbxsdk::FbxAxisSystem::EFrontVector originFrontVector = SourceAxisSystem.GetFrontVector(originFrontSign);
	fbxsdk::FbxAxisSystem::EFrontVector engineFrontVector = JKEngineAxisSystem.GetFrontVector(engineFrontSign);
	fbxsdk::FbxAxisSystem::EUpVector originUpvector = SourceAxisSystem.GetUpVector(originUpSign);
	fbxsdk::FbxAxisSystem::EUpVector engineUpVector = JKEngineAxisSystem.GetUpVector(engineUpSign);
	fbxsdk::FbxAxisSystem::ECoordSystem originCoordSystem = SourceAxisSystem.GetCoorSystem();
	fbxsdk::FbxAxisSystem::ECoordSystem engineCoordSystem = JKEngineAxisSystem.GetCoorSystem();

	if (originUpvector == engineUpVector && originUpSign == engineUpSign)
	{
		if (originFrontSign != engineFrontSign && originCoordSystem == engineCoordSystem)
		{
			m_ModifyRotationAxis.mData[static_cast<int>(originUpvector) - 1] += 180;
		}
	}
	else if (originUpvector == engineUpVector && originUpSign != engineUpSign)
	{
		if (originUpvector == fbxsdk::FbxAxisSystem::EUpVector::eXAxis)
		{
			m_ModifyRotationAxis.mData[1] += 180;
		}
		else
		{
			m_ModifyRotationAxis.mData[0] += 180;
		}

		if (originFrontSign != engineFrontSign && originCoordSystem == engineCoordSystem)
		{
			m_ModifyRotationAxis.mData[static_cast<int>(originUpvector) - 1] += 180;
		}
	}
	else if (originUpvector == fbxsdk::FbxAxisSystem::EUpVector::eXAxis)
	{
		m_ModifyRotationAxis.mData[1] += originUpSign * 90;
	}
	else
	{
		m_ModifyRotationAxis.mData[0] += originUpSign * 90;
		if (originUpSign != engineFrontSign)
		{
			m_ModifyRotationAxis.mData[static_cast<int>(originUpvector) - 1] += originUpSign * 180;
		}
	}

	m_JointPostConversionMatrix.SetR(m_ModifyRotationAxis);

	if (m_JointPostConversionMatrix.IsIdentity() == true)
	{
		m_JointPostConversionMatrix = m_AxisConversionMatrix;
	}

	m_Scene->GetAnimationEvaluator()->Reset();
	m_RootNode = m_Scene->GetRootNode();

	return true;
}

void GJHDirectFbx::AniCheck()
{
	fbxsdk::FbxArray<FbxString*> AniNameArray;
	m_Scene->FillAnimStackNameArray(AniNameArray);

	if (0 == AniNameArray.Size())
	{
		return;
	}

	m_UserAniData.resize(AniNameArray.Size());

	for (int i = 0; i < AniNameArray.GetCount(); i++)
	{
		fbxsdk::FbxAnimStack* pAniStatic = m_Scene->FindMember<fbxsdk::FbxAnimStack>(AniNameArray[i]->Buffer());

		if (nullptr == pAniStatic)
		{
			GJHGameEngineDebug::AssertMsg("if (nullptr == pAniStatic)");
			continue;
		}

		m_UserAniData[i].AniName = CA2W(pAniStatic->GetName(), CP_UTF8).m_psz;

		FbxTakeInfo* TakeInfo = m_Scene->GetTakeInfo(pAniStatic->GetName());

		m_UserAniData[i].StartTime = TakeInfo->mLocalTimeSpan.GetStart();
		m_UserAniData[i].EndTime = TakeInfo->mLocalTimeSpan.GetStop();

		m_UserAniData[i].TimeMode = m_Scene->GetGlobalSettings().GetTimeMode();
		m_UserAniData[i].TimeStartCount = m_UserAniData[i].StartTime.GetFrameCount(m_UserAniData[i].TimeMode);

		if (0 >= m_UserAniData[i].TimeStartCount)
		{
			m_UserAniData[i].TimeStartCount *= (FbxLongLong)-1;
		}

		m_UserAniData[i].TimeEndCount = m_UserAniData[i].EndTime.GetFrameCount(m_UserAniData[i].TimeMode);
		m_UserAniData[i].FrameCount = m_UserAniData[i].TimeEndCount - m_UserAniData[i].TimeStartCount;

		m_UserAniData[i].FbxModeCount = (long long)fbxsdk::FbxTime::GetFrameRate(m_UserAniData[i].TimeMode);
		m_UserAniData[i].FbxModeRate = (double)fbxsdk::FbxTime::GetFrameRate(m_UserAniData[i].TimeMode);

		double Test = fbxsdk::FbxTime::GetFrameRate(m_UserAniData[i].TimeMode);

	}

	for (int i = 0; i < AniNameArray.GetCount(); i++)
	{
		delete AniNameArray[i];
	}

}

fbxsdk::FbxNode* GJHDirectFbx::RecursiveGetFirstMeshNode(fbxsdk::FbxNode* Node, fbxsdk::FbxNode* NodeToFind)
{
	if (Node == nullptr)
	{
		return nullptr;
	}

	if (Node->GetMesh() != nullptr)
	{
		return Node;
	}

	for (int ChildIndex = 0; ChildIndex < Node->GetChildCount(); ++ChildIndex)
	{
		fbxsdk::FbxNode* MeshNode = RecursiveGetFirstMeshNode(Node->GetChild(ChildIndex), NodeToFind);
		if (NodeToFind == nullptr)
		{
			if (MeshNode != nullptr)
			{
				return MeshNode;
			}
		}
		else if (MeshNode == NodeToFind)
		{
			return MeshNode;
		}
	}
	return nullptr;
}


fbxsdk::FbxNode* GJHDirectFbx::FindLODGroupNode(fbxsdk::FbxNode* NodeLodGroup, int LodIndex, fbxsdk::FbxNode* NodeToFind)
{
	if (NodeLodGroup->GetChildCount() < LodIndex)
	{
		GJHGameEngineDebug::AssertMsg("잘못된 인덱스");
		return nullptr;
	}
	fbxsdk::FbxNode* childNode = NodeLodGroup->GetChild(LodIndex);
	if (nullptr == childNode)
	{
		return nullptr;
	}
	return RecursiveGetFirstMeshNode(childNode, NodeToFind);
}


fbxsdk::FbxNode* GJHDirectFbx::RecursiveFindParentLodGroup(fbxsdk::FbxNode* parentNode)
{
	if (nullptr == parentNode)
	{
		return nullptr;
	}
	fbxsdk::FbxNodeAttribute* attribute = parentNode->GetNodeAttribute();
	if (nullptr != attribute && attribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eLODGroup)
	{
		return parentNode;
	}
	return RecursiveFindParentLodGroup(parentNode->GetParent());
}


void GJHDirectFbx::TraverseHierarchyNodeRecursively(FbxExSceneInfo& SceneInfo, fbxsdk::FbxNode* ParentNode, FbxExNodeInfo& ParentInfo)
{
	int NodeCount = ParentNode->GetChildCount();
	for (int NodeIndex = 0; NodeIndex < NodeCount; ++NodeIndex)
	{
		fbxsdk::FbxNode* ChildNode = ParentNode->GetChild(NodeIndex);
		FbxExNodeInfo ChildInfo;
		ChildInfo.ObjectName = ChildNode->GetName();
		ChildInfo.UniqueId = ChildNode->GetUniqueID();
		ChildInfo.ParentName = ParentInfo.ObjectName;
		ChildInfo.ParentUniqueId = ParentInfo.UniqueId;
		ChildInfo.RotationPivot = ChildNode->RotationPivot.Get();
		ChildInfo.ScalePivot = ChildNode->ScalingPivot.Get();
		ChildInfo.Transform = ChildNode->EvaluateLocalTransform();
		if (ChildNode->GetNodeAttribute())
		{
			fbxsdk::FbxNodeAttribute* ChildAttribute = ChildNode->GetNodeAttribute();
			ChildInfo.AttributeUniqueId = ChildAttribute->GetUniqueID();
			if (ChildAttribute->GetName()[0] != '\0')
			{
				ChildInfo.AttributeName = ChildAttribute->GetName();
			}
			else
			{
				ChildInfo.AttributeName = ChildAttribute->GetNode()->GetName();
			}

			switch (ChildAttribute->GetAttributeType())
			{
			case fbxsdk::FbxNodeAttribute::eUnknown:
				ChildInfo.AttributeType = "eUnknown";
				break;
			case fbxsdk::FbxNodeAttribute::eNull:
				ChildInfo.AttributeType = "eNull";
				break;
			case fbxsdk::FbxNodeAttribute::eMarker:
				ChildInfo.AttributeType = "eMarker";
				break;
			case fbxsdk::FbxNodeAttribute::eSkeleton:
				ChildInfo.AttributeType = "eSkeleton";
				break;
			case fbxsdk::FbxNodeAttribute::eMesh:
				ChildInfo.AttributeType = "eMesh";
				break;
			case fbxsdk::FbxNodeAttribute::eNurbs:
				ChildInfo.AttributeType = "eNurbs";
				break;
			case fbxsdk::FbxNodeAttribute::ePatch:
				ChildInfo.AttributeType = "ePatch";
				break;
			case fbxsdk::FbxNodeAttribute::eCamera:
				ChildInfo.AttributeType = "eCamera";
				break;
			case fbxsdk::FbxNodeAttribute::eCameraStereo:
				ChildInfo.AttributeType = "eCameraStereo";
				break;
			case fbxsdk::FbxNodeAttribute::eCameraSwitcher:
				ChildInfo.AttributeType = "eCameraSwitcher";
				break;
			case fbxsdk::FbxNodeAttribute::eLight:
				ChildInfo.AttributeType = "eLight";
				break;
			case fbxsdk::FbxNodeAttribute::eOpticalReference:
				ChildInfo.AttributeType = "eOpticalReference";
				break;
			case fbxsdk::FbxNodeAttribute::eOpticalMarker:
				ChildInfo.AttributeType = "eOpticalMarker";
				break;
			case fbxsdk::FbxNodeAttribute::eNurbsCurve:
				ChildInfo.AttributeType = "eNurbsCurve";
				break;
			case fbxsdk::FbxNodeAttribute::eTrimNurbsSurface:
				ChildInfo.AttributeType = "eTrimNurbsSurface";
				break;
			case fbxsdk::FbxNodeAttribute::eBoundary:
				ChildInfo.AttributeType = "eBoundary";
				break;
			case fbxsdk::FbxNodeAttribute::eNurbsSurface:
				ChildInfo.AttributeType = "eNurbsSurface";
				break;
			case fbxsdk::FbxNodeAttribute::eShape:
				ChildInfo.AttributeType = "eShape";
				break;
			case fbxsdk::FbxNodeAttribute::eLODGroup:
				ChildInfo.AttributeType = "eLODGroup";
				break;
			case fbxsdk::FbxNodeAttribute::eSubDiv:
				ChildInfo.AttributeType = "eSubDiv";
				break;
			case fbxsdk::FbxNodeAttribute::eCachedEffect:
				ChildInfo.AttributeType = "eCachedEffect";
				break;
			case fbxsdk::FbxNodeAttribute::eLine:
				ChildInfo.AttributeType = "eLine";
				break;
			}
		}
		else
		{
			ChildInfo.AttributeUniqueId = 0xFFFFFFFFFFFFFFFF;
			ChildInfo.AttributeType = "eNull";
			ChildInfo.AttributeName = NULL;
		}

		SceneInfo.HierarchyInfo.push_back(ChildInfo);
		TraverseHierarchyNodeRecursively(SceneInfo, ChildNode, ChildInfo);
	}
}

void GJHDirectFbx::ImportSceneInfo()
{
	fbxsdk::FbxGeometryConverter Con(m_Manager);
	
	if (Con.Triangulate(m_Scene, true) == false)
	{
		GJHGameEngineDebug::AssertMsg("if (false == Con.Triangulate(m_Scene, true))");
	}

	fbxsdk::FbxTimeSpan GlobalTimeSpan(fbxsdk::FBXSDK_TIME_INFINITE, fbxsdk::FBXSDK_TIME_MINUS_INFINITE);

	m_SceneInfo.TotalMaterialNum = m_Scene->GetMaterialCount();
	m_SceneInfo.TotalTextureNum = m_Scene->GetTextureCount();

	int geometryCount = m_Scene->GetGeometryCount();
	for (int geometryIndex = 0; geometryIndex < geometryCount; ++geometryIndex)
	{
		fbxsdk::FbxGeometry* geoMetry = m_Scene->GetGeometry(geometryIndex);

		if (geoMetry->GetAttributeType() == fbxsdk::FbxNodeAttribute::eMesh)
		{
			fbxsdk::FbxNode* geoMetryNode = geoMetry->GetNode();
			fbxsdk::FbxMesh* mesh = reinterpret_cast<fbxsdk::FbxMesh*>(geoMetry);

			if (geoMetryNode != nullptr&&
				mesh->GetDeformerCount(fbxsdk::FbxDeformer::eSkin) <= 0)
			{
				fbxsdk::FbxNode* ParentNode = RecursiveFindParentLodGroup(geoMetryNode->GetParent());
				if (ParentNode != nullptr &&
					ParentNode->GetNodeAttribute() != nullptr &&
					fbxsdk::FbxNodeAttribute::eLODGroup == ParentNode->GetNodeAttribute()->GetAttributeType())
				{
					bool isLodRoot = false;
					for (int ChildIndex = 0; ChildIndex < ParentNode->GetChildCount(); ++ChildIndex)
					{
						fbxsdk::FbxNode* MeshNode = FindLODGroupNode(ParentNode, ChildIndex);
						if (geoMetryNode == MeshNode)
						{
							isLodRoot = true;
							break;
						}
					}
					if (isLodRoot == false)
					{
						continue;
					}
				}
			}

			++m_SceneInfo.TotalGeometryNum;

			m_SceneInfo.MeshInfo.push_back(FbxExMeshInfo());
			FbxExMeshInfo& meshInfo = m_SceneInfo.MeshInfo.at(m_SceneInfo.MeshInfo.size() - 1);
			if (geoMetry->GetName()[0] != '\0')
			{
				meshInfo.Name = GJHGameEngineString::ConvertMultiByteString(geoMetry->GetName(), CP_ACP);
			}
			else
			{
				meshInfo.Name = nullptr != geoMetryNode ? GJHGameEngineString::ConvertMultiByteString(geoMetryNode->GetName(), CP_ACP) : L"None";
			}

			meshInfo.pMesh = mesh;
			meshInfo.bTriangulated = mesh->IsTriangleMesh();
			meshInfo.MaterialNum = geoMetryNode ? geoMetryNode->GetMaterialCount() : 0;
			meshInfo.FaceNum = mesh->GetPolygonCount();
			meshInfo.VertexNum = mesh->GetControlPointsCount();

			meshInfo.LODGroup = L"";
			if (geoMetryNode != nullptr)
			{
				fbxsdk::FbxNode* ParentNode = RecursiveFindParentLodGroup(geoMetryNode->GetParent());
				if (ParentNode != nullptr && ParentNode->GetNodeAttribute() && ParentNode->GetNodeAttribute()->GetAttributeType() == fbxsdk::FbxNodeAttribute::eLODGroup)
				{
					meshInfo.LODGroup = GJHGameEngineString::ConvertMultiByteString(ParentNode->GetName(), CP_ACP);
					meshInfo.bIsLodGroup = true;
					for (int LODIndex = 0; LODIndex < ParentNode->GetChildCount(); LODIndex++)
					{
						fbxsdk::FbxNode* MeshNode = FindLODGroupNode(ParentNode, LODIndex, geoMetryNode);
						if (geoMetryNode == MeshNode)
						{
							meshInfo.LODLevel = LODIndex;
							break;
						}
					}
				}
			}

			if (0 < mesh->GetDeformerCount(FbxDeformer::eSkin))
			{
				m_SceneInfo.SkinnedMeshNum++;
				meshInfo.bIsSkelMesh = true;
				meshInfo.MorphNum = mesh->GetShapeCount();
				fbxsdk::FbxSkin* Skin = (fbxsdk::FbxSkin*)mesh->GetDeformer(0, FbxDeformer::eSkin);
				int ClusterCount = Skin->GetClusterCount();
				fbxsdk::FbxNode* Link = NULL;
				for (int ClusterId = 0; ClusterId < ClusterCount; ++ClusterId)
				{
					fbxsdk::FbxCluster* Cluster = Skin->GetCluster(ClusterId);
					Link = Cluster->GetLink();
					while (Link && Link->GetParent() && Link->GetParent()->GetSkeleton())
					{
						Link = Link->GetParent();
					}

					if (Link != NULL)
					{
						break;
					}
				}

				meshInfo.SkeletonRoot = Link != nullptr? GJHGameEngineString::ConvertMultiByteString(Link->GetName()) : L"None";
				meshInfo.SkeletonElemNum = Link != nullptr ? Link->GetChildCount(true) : 0;

				if (Link != nullptr)
				{
					fbxsdk::FbxTimeSpan AnimTimeSpan(fbxsdk::FBXSDK_TIME_INFINITE, fbxsdk::FBXSDK_TIME_MINUS_INFINITE);
					Link->GetAnimationInterval(AnimTimeSpan);
					GlobalTimeSpan.UnionAssignment(AnimTimeSpan);
				}
			}
			else
			{
				m_SceneInfo.NonSkinnedMeshNum++;
				meshInfo.bIsSkelMesh = false;
				meshInfo.SkeletonRoot = L"";
			}
			meshInfo.UniqueId = mesh->GetUniqueID();
		}
	}

	m_SceneInfo.bHasAnimation = false;
	int AnimCurveNodeCount = m_Scene->GetSrcObjectCount<fbxsdk::FbxAnimCurveNode>();

	for (int AnimCurveNodeIndex = 2; AnimCurveNodeIndex < AnimCurveNodeCount; AnimCurveNodeIndex++)
	{
		fbxsdk::FbxAnimCurveNode* CurAnimCruveNode = m_Scene->GetSrcObject<fbxsdk::FbxAnimCurveNode>(AnimCurveNodeIndex);
		if (CurAnimCruveNode->IsAnimated(true))
		{
			m_SceneInfo.bHasAnimation = true;
			break;
		}
	}

	m_SceneInfo.FrameRate = fbxsdk::FbxTime::GetFrameRate(m_Scene->GetGlobalSettings().GetTimeMode());

	int dir = GlobalTimeSpan.GetDirection();
	if (GlobalTimeSpan.GetDirection() == FBXSDK_TIME_FORWARD)
	{
		m_SceneInfo.TotalTime = (GlobalTimeSpan.GetDuration().GetMilliSeconds()) / 1000.f * m_SceneInfo.FrameRate;
	}
	else
	{
		m_SceneInfo.TotalTime = 0;
	}

	fbxsdk::FbxNode* RootNode = m_Scene->GetRootNode();
	FbxExNodeInfo RootInfo;
	RootInfo.ObjectName = RootNode->GetName();
	RootInfo.UniqueId = RootNode->GetUniqueID();
	RootInfo.Transform = RootNode->EvaluateGlobalTransform();

	RootInfo.AttributeName = NULL;
	RootInfo.AttributeUniqueId = 0;
	RootInfo.AttributeType = NULL;

	RootInfo.ParentName = NULL;
	RootInfo.ParentUniqueId = 0;

	m_SceneInfo.HierarchyInfo.push_back(RootInfo);

	TraverseHierarchyNodeRecursively(m_SceneInfo, RootNode, RootInfo);

	if (m_SceneInfo.SkinnedMeshNum > 0)
	{
		m_ImportType = FbxImportType::Deformers;
	}
	else if (m_SceneInfo.TotalGeometryNum > 0)
	{
		m_ImportType = FbxImportType::Geometry;
	}

	if (true == m_SceneInfo.bHasAnimation)
	{
		if (FbxImportType::Failed == m_ImportType)
		{
			m_ImportType = FbxImportType::Otherwise;
		}
		else if (FbxImportType::Geometry == m_ImportType)
		{
			m_ImportType = FbxImportType::Deformers;
		}
	}
}

void GJHDirectFbx::ImportCluster()
{
	size_t meshCount = m_SceneInfo.MeshInfo.size();
	if (0 == meshCount)
	{
		return;
	}

	for (size_t n = 0; n < meshCount; ++n)
	{
		FbxExMeshInfo& meshInfo = m_SceneInfo.MeshInfo.at(n);
		fbxsdk::FbxNode* pNode = meshInfo.pMesh->GetNode();
		fbxsdk::FbxMesh* FbxMesh = meshInfo.pMesh;
		const int SkinDeformerCount = FbxMesh->GetDeformerCount(fbxsdk::FbxDeformer::eSkin);

		if (0 == SkinDeformerCount)
		{
			continue;
		}
		std::vector<FbxClusterData>& vecClusterData = m_mapClusterData[meshInfo.LODLevel];

		if (m_mapClusterData.end() == m_mapClusterData.find(meshInfo.LODLevel))
		{
			m_mapClusterData.insert(std::make_pair(meshInfo.LODLevel, std::vector<FbxClusterData>()));
		}

		for (int DeformerIndex = 0; DeformerIndex < SkinDeformerCount; DeformerIndex++)
		{
			fbxsdk::FbxSkin* Skin = (fbxsdk::FbxSkin*)FbxMesh->GetDeformer(DeformerIndex, fbxsdk::FbxDeformer::eSkin);
			for (int ClusterIndex = 0; ClusterIndex < Skin->GetClusterCount(); ClusterIndex++)
			{
				vecClusterData.push_back(FbxClusterData());
				FbxClusterData& clusterData = vecClusterData[vecClusterData.size() - 1];
				clusterData.m_Cluster = Skin->GetCluster(ClusterIndex);
				clusterData.m_Mesh = FbxMesh;
				clusterData.LinkName = GJHGameEngineString::ConvertMultiByteString(clusterData.m_Cluster->GetLink()->GetName());
			}
		}
	}
}


fbxsdk::FbxAMatrix GJHDirectFbx::ComputeTotalMatrix(fbxsdk::FbxNode* Node)
{
	fbxsdk::FbxAMatrix Geometry;
	fbxsdk::FbxVector4 Translation, Rotation, Scaling;
	Translation = Node->GetGeometricTranslation(fbxsdk::FbxNode::eSourcePivot);
	Rotation = Node->GetGeometricRotation(fbxsdk::FbxNode::eSourcePivot);
	Scaling = Node->GetGeometricScaling(fbxsdk::FbxNode::eSourcePivot);
	Geometry.SetT(Translation);
	Geometry.SetR(Rotation);
	Geometry.SetS(Scaling);

	fbxsdk::FbxAMatrix& GlobalTransform = m_Scene->GetAnimationEvaluator()->GetNodeGlobalTransform(Node);
	return GlobalTransform * Geometry;
}

bool GJHDirectFbx::IsOddNegativeScale(const fbxsdk::FbxAMatrix& TotalMatrix)
{
	fbxsdk::FbxVector4 Scale = TotalMatrix.GetS();
	int NegativeNum = 0;

	if (Scale[0] < 0) NegativeNum++;
	if (Scale[1] < 0) NegativeNum++;
	if (Scale[2] < 0) NegativeNum++;

	return NegativeNum == 1 || NegativeNum == 3;
}


void GJHDirectFbx::RenderSetMaterialSetting(fbxsdk::FbxNode* _Node, std::shared_ptr<FbxRenderSet> _RenderData)
{
	int MtrlCount = _Node->GetMaterialCount();

	if (MtrlCount > 0)
	{
		_RenderData->m_MatialData.push_back(std::vector<FbxExFaceSettingData>());

		std::vector<FbxExFaceSettingData>& MatrialSet = _RenderData->m_MatialData[_RenderData->m_MatialData.size() - 1];

		for (int i = 0; i < MtrlCount; i++)
		{
			fbxsdk::FbxSurfaceMaterial* pMtrl = _Node->GetMaterial(i);

			if (nullptr == pMtrl)
			{
				GJHGameEngineDebug::AssertMsg("if (nullptr == pMtrl) 머티리얼 정보 에러");
			}

			MatrialSet.push_back(FbxExFaceSettingData());
			FbxExFaceSettingData& MatData = MatrialSet[MatrialSet.size() - 1];
			MatData.Name = GJHGameEngineString::ConvertMultiByteString(pMtrl->GetName());
			MatData.DifColor = MaterialColor(pMtrl, fbxsdk::FbxSurfaceMaterial::sDiffuse, fbxsdk::FbxSurfaceMaterial::sDiffuseFactor);
			MatData.AmbColor = MaterialColor(pMtrl, fbxsdk::FbxSurfaceMaterial::sAmbient, fbxsdk::FbxSurfaceMaterial::sAmbientFactor);
			MatData.SpcColor = MaterialColor(pMtrl, fbxsdk::FbxSurfaceMaterial::sSpecular, fbxsdk::FbxSurfaceMaterial::sSpecularFactor);
			MatData.EmvColor = MaterialColor(pMtrl, fbxsdk::FbxSurfaceMaterial::sEmissive, fbxsdk::FbxSurfaceMaterial::sEmissiveFactor);
			MatData.SpecularPower = MaterialFactor(pMtrl, fbxsdk::FbxSurfaceMaterial::sSpecularFactor);
			MatData.Shininess = MaterialFactor(pMtrl, fbxsdk::FbxSurfaceMaterial::sShininess);
			MatData.TransparencyFactor = MaterialFactor(pMtrl, fbxsdk::FbxSurfaceMaterial::sTransparencyFactor);
			MatData.DifTexturePath = MaterialTex(pMtrl, fbxsdk::FbxSurfaceMaterial::sDiffuse);
			MatData.BmpTexturePath = MaterialTex(pMtrl, fbxsdk::FbxSurfaceMaterial::sNormalMap);
			MatData.SpcTexturePath = MaterialTex(pMtrl, fbxsdk::FbxSurfaceMaterial::sSpecular);

			if (MatData.DifTexturePath != L"")
			{
				if (true == GJHGameEnginePath::IsExist(MatData.DifTexturePath))
				{
					GJHDirectTexture2D::Load(MatData.DifTexturePath);
				}

				MatData.DifTextureName = GJHGameEnginePath::GetFileName(MatData.DifTexturePath);
			}

			if (MatData.BmpTexturePath != L"")
			{
				if (true == GJHGameEnginePath::IsExist(MatData.BmpTexturePath))
				{
					GJHDirectTexture2D::Load(MatData.BmpTexturePath);
				}

				MatData.BmpTextureName = GJHGameEnginePath::GetFileName(MatData.BmpTexturePath);
			}

			if (MatData.SpcTexturePath != L"")
			{
				if (true == GJHGameEnginePath::IsExist(MatData.SpcTexturePath))
				{
					GJHDirectTexture2D::Load(MatData.SpcTexturePath);
				}

				MatData.SpcTextureName = GJHGameEnginePath::GetFileName(MatData.SpcTexturePath);
			}

		}
	}
	else
	{
		GJHGameEngineDebug::AssertMsg("매쉬는 존재하지만 재질은 존재하지 않습니다.");
	}
}

void GJHDirectFbx::LoadUv(fbxsdk::FbxMesh* _Mesh, std::vector<VertexAnimation>& _ArrVtx, int VtxId, int _Index)
{
	std::map<GJHGameEngineString, int> UVSets;
	int layerCount = _Mesh->GetLayerCount();
	for (int uvLayerIndex = 0; uvLayerIndex < layerCount; ++uvLayerIndex)
	{
		fbxsdk::FbxLayer* pLayer = _Mesh->GetLayer(uvLayerIndex);
		int uvSetCount = pLayer->GetUVSetCount();
		if (0 != uvSetCount)
		{
			fbxsdk::FbxArray<const fbxsdk::FbxLayerElementUV*> EleUVs = pLayer->GetUVSets();
			for (int UVIndex = 0; UVIndex < uvSetCount; ++UVIndex)
			{
				const fbxsdk::FbxLayerElementUV* elementUv = EleUVs[UVIndex];
				if (nullptr != elementUv)
				{
					GJHGameEngineString uvName = GJHGameEngineString::ConvertMultiByteString(elementUv->GetName());
					if (true == uvName.IsEmpty())
					{
						uvName.Appendf("UVmap_%d", uvLayerIndex);
					}
					std::map<GJHGameEngineString, int>::iterator findIter = UVSets.find(uvName);
					if (findIter == UVSets.end())
					{
						UVSets.insert(std::make_pair(uvName, 0));
					}
				}
			}
		}
	}

	if (UVSets.size() != 0)
	{
		for (size_t UVIndex = 0; UVIndex < UVSets.size(); ++UVIndex)
		{
			fbxsdk::FbxLayer* pLayer = _Mesh->GetLayer(static_cast<int>(UVIndex));
			int UVSetCount = pLayer->GetUVSetCount();
			if (UVSetCount != 0)
			{
				fbxsdk::FbxArray<const fbxsdk::FbxLayerElementUV*> EleUVs = pLayer->GetUVSets();
				for (int FbxUVIndex = 0; FbxUVIndex < UVSetCount; ++FbxUVIndex)
				{
					const fbxsdk::FbxLayerElementUV* elementUv = EleUVs[FbxUVIndex];
					if (nullptr != elementUv)
					{
						fbxsdk::FbxLayerElementUV* uvuv = const_cast<fbxsdk::FbxLayerElementUV*>(elementUv);
						auto rere = uvuv->GetReferenceMode();
						auto asad = uvuv->GetMappingMode();
					}
				}
			}
		}
	}

	int iCount = _Mesh->GetElementUVCount();

	if (0 == iCount)
	{
		GJHGameEngineDebug::AssertMsg("UV가 여러개 입니다.");
	}

	fbxsdk::FbxGeometryElementUV* pElement = _Mesh->GetElementUV();
	int iDataIndex = VtxId;

	if (pElement->GetMappingMode() == fbxsdk::FbxGeometryElement::eByPolygonVertex)
	{
		if (fbxsdk::FbxGeometryElement::eIndexToDirect == pElement->GetReferenceMode())
		{
			iDataIndex = pElement->GetIndexArray().GetAt(VtxId);
		}
		else if (fbxsdk::FbxGeometryElement::eDirect == pElement->GetReferenceMode())
		{
			iDataIndex = _Index;
		}
		else
		{
			iDataIndex = VtxId;
		}
	}

	else if (pElement->GetMappingMode() == fbxsdk::FbxGeometryElement::eByControlPoint)
	{
		if (fbxsdk::FbxGeometryElement::eDirect == pElement->GetReferenceMode())
		{
			iDataIndex = _Index;
		}
		else if (fbxsdk::FbxGeometryElement::eIndexToDirect == pElement->GetReferenceMode())
		{
			iDataIndex = pElement->GetIndexArray().GetAt(_Index);
		}
		else
		{
			GJHGameEngineDebug::AssertMsg("맵핑 방식중 처리할수 없는 방식입니다.");
		}
	}

	fbxsdk::FbxVector2 vUV = pElement->GetDirectArray().GetAt(iDataIndex);

	_ArrVtx[_Index].Uv.x = (float)(vUV.mData[0] - (int)vUV.mData[0]);
	_ArrVtx[_Index].Uv.y = (float)(1.0f - (vUV.mData[1] - (int)vUV.mData[1]));

}

void GJHDirectFbx::LoadBinormal(fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxAMatrix _MeshMatrix, std::vector<VertexAnimation>& _ArrVtx, int VtxId, int _Index)
{
	int iCount = _Mesh->GetElementBinormalCount();

	if (0 == iCount)
	{
		return;
	}

	FbxGeometryElementBinormal* pElement = _Mesh->GetElementBinormal();
	int iDataIndex = VtxId;

	if (pElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (FbxGeometryElement::eIndexToDirect == pElement->GetReferenceMode())
		{
			iDataIndex = pElement->GetIndexArray().GetAt(VtxId);
		}
		else
		{
			iDataIndex = VtxId;
		}
	}

	else if (pElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (FbxGeometryElement::eDirect == pElement->GetReferenceMode())
		{
			iDataIndex = _Index;
		}
		else if (FbxGeometryElement::eIndexToDirect == pElement->GetReferenceMode())
		{
			iDataIndex = pElement->GetIndexArray().GetAt(_Index);
		}
		else
		{
			GJHGameEngineDebug::AssertMsg("맵핑 방식중 처리할수 없는 방식입니다.");
		}
	}

	FbxVector4 BiNormal = pElement->GetDirectArray().GetAt(iDataIndex);

	fbxsdk::FbxAMatrix conversionMeshMatrix = _MeshMatrix.Transpose();
	BiNormal = conversionMeshMatrix.MultT(BiNormal);

	_ArrVtx[_Index].BiNormal.x = (float)BiNormal.mData[0];
	_ArrVtx[_Index].BiNormal.y = (float)BiNormal.mData[1];
	_ArrVtx[_Index].BiNormal.z = -(float)BiNormal.mData[2];
	_ArrVtx[_Index].BiNormal.w = (float)BiNormal.mData[3];
	_ArrVtx[_Index].BiNormal.Normalize();
}

void GJHDirectFbx::LoadTangent(fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxAMatrix _MeshMatrix, std::vector<VertexAnimation>& _ArrVtx, int VtxId, int _Index)
{
	int iCount = _Mesh->GetElementTangentCount();

	if (0 == iCount)
	{
		return;
	}
	FbxGeometryElementTangent* pElement = _Mesh->GetElementTangent();
	int iDataIndex = VtxId;

	if (pElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (FbxGeometryElement::eIndexToDirect == pElement->GetReferenceMode())
		{
			iDataIndex = pElement->GetIndexArray().GetAt(VtxId);
		}
		else
		{
			iDataIndex = VtxId;
		}
	}
	else if (pElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (FbxGeometryElement::eDirect == pElement->GetReferenceMode())
		{
			iDataIndex = _Index;
		}
		else if (FbxGeometryElement::eIndexToDirect == pElement->GetReferenceMode())
		{
			iDataIndex = pElement->GetIndexArray().GetAt(_Index);
		}
		else
		{
			GJHGameEngineDebug::AssertMsg("맵핑 방식중 처리할수 없는 방식입니다.");
		}
	}

	FbxVector4 Tangent = pElement->GetDirectArray().GetAt(iDataIndex);

	fbxsdk::FbxAMatrix conversionMeshMatrix = _MeshMatrix.Transpose();
	Tangent = conversionMeshMatrix.MultT(Tangent);

	_ArrVtx[_Index].Tangent.x = (float)Tangent.mData[0];
	_ArrVtx[_Index].Tangent.y = (float)Tangent.mData[1];
	_ArrVtx[_Index].Tangent.z = -(float)Tangent.mData[2];
	_ArrVtx[_Index].Tangent.w = (float)Tangent.mData[3];
	_ArrVtx[_Index].Tangent.Normalize();
}

void GJHDirectFbx::LoadNormal(fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxAMatrix _MeshMatrix, std::vector<VertexAnimation>& _ArrVtx, int VtxId, int _Index)
{
	int iCount = _Mesh->GetElementNormalCount();

	if (0 == iCount)
	{
		GJHGameEngineDebug::AssertMsg("GetElementNormalCount가 여러개 입니다.");
	}

	FbxGeometryElementNormal* pElement = _Mesh->GetElementNormal();
	int iDataIndex = VtxId;

	if (pElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (FbxGeometryElement::eIndexToDirect == pElement->GetReferenceMode())
		{
			iDataIndex = pElement->GetIndexArray().GetAt(VtxId);
		}
		else
		{
			iDataIndex = VtxId;
		}
	}
	else if (pElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (FbxGeometryElement::eDirect == pElement->GetReferenceMode())
		{
			iDataIndex = _Index;
		}
		else if (FbxGeometryElement::eIndexToDirect == pElement->GetReferenceMode())
		{
			iDataIndex = pElement->GetIndexArray().GetAt(_Index);
		}
		else
		{
			GJHGameEngineDebug::AssertMsg("맵핑 방식중 처리할수 없는 방식입니다.");
		}
	}

	FbxVector4 Normal = pElement->GetDirectArray().GetAt(iDataIndex);

	fbxsdk::FbxAMatrix conversionMeshMatrix = _MeshMatrix.Transpose();
	Normal = conversionMeshMatrix.MultT(Normal);

	_ArrVtx[_Index].Normal.x = (float)Normal.mData[0];
	_ArrVtx[_Index].Normal.y = (float)Normal.mData[1];
	_ArrVtx[_Index].Normal.z = -(float)Normal.mData[2];
	_ArrVtx[_Index].Normal.w = (float)Normal.mData[3];
	_ArrVtx[_Index].Normal.Normalize();
}

void GJHDirectFbx::ImportControlPoints()
{
	size_t meshInfoSize = m_SceneInfo.MeshInfo.size();
	for (size_t meshInfoIndex = 0; meshInfoIndex < meshInfoSize; ++meshInfoIndex)
	{
		FbxExMeshInfo& meshInfo = m_SceneInfo.MeshInfo.at(meshInfoIndex);
		fbxsdk::FbxNode* pMeshNode = meshInfo.pMesh->GetNode();
		fbxsdk::FbxMesh* pMesh = meshInfo.pMesh;

		if (m_mapRenderSet.end() == m_mapRenderSet.find(meshInfo.LODLevel))
		{
			m_mapRenderSet.insert(std::make_pair(meshInfo.LODLevel, std::make_shared<FbxRenderSet>()));
		}

		std::shared_ptr<FbxRenderSet> pDrawSet = m_mapRenderSet[meshInfo.LODLevel];

		if (pDrawSet->m_MapWI.end() == pDrawSet->m_MapWI.find(pMesh))
		{
			pDrawSet->m_MapWI.insert(std::make_pair(pMesh, std::map<int, std::vector<FbxExIW>>()));
		}

		pDrawSet->IsLod = meshInfo.bIsLodGroup;
		pDrawSet->IsLodLv = meshInfo.LODLevel;
		pDrawSet->m_FbxVtx.push_back(new std::vector<VertexAnimation>());
		pDrawSet->m_FbxIdx.push_back(new std::vector<std::vector<UINT>>());
		std::vector<VertexAnimation>& VtxData = *(pDrawSet->m_FbxVtx[pDrawSet->m_FbxVtx.size() - 1]);
		std::vector<std::vector<UINT>>& IdxData = *(pDrawSet->m_FbxIdx[pDrawSet->m_FbxVtx.size() - 1]);

		int controlPointsCount = pMesh->GetControlPointsCount();
		fbxsdk::FbxVector4* pControlPoints = pMesh->GetControlPoints();

		VtxData.resize(controlPointsCount);

		fbxsdk::FbxAMatrix meshMatrix;
		meshMatrix = m_AxisConversionMatrix;

		meshMatrix = ComputeTotalMatrix(pMeshNode);

		if (false == meshInfo.bIsSkelMesh)
		{
			meshMatrix = m_JointPostConversionMatrix * meshMatrix;
		}

		bool isOddNegativeScale = IsOddNegativeScale(meshMatrix);

		for (int controlPointIndex = 0; controlPointIndex < controlPointsCount; ++controlPointIndex)
		{
			fbxsdk::FbxVector4 controlPoint = pControlPoints[controlPointIndex];
			fbxsdk::FbxVector4 calculateControlPoint = meshMatrix.MultT(controlPoint);
			VtxData[controlPointIndex].Pos = FbxVecToTransform(calculateControlPoint);
			VtxData[controlPointIndex].Pos.w = 1.f;
		}

		RenderSetMaterialSetting(pMeshNode, pDrawSet);

		fbxsdk::FbxStringList UVSetNameList;
		pMesh->GetUVSetNames(UVSetNameList);
		int uvSetCount = UVSetNameList.GetCount();
		for (int uvSetIndex = 0; uvSetIndex < uvSetCount; ++uvSetIndex)
		{
			const char* uvSetName = UVSetNameList.GetStringAt(uvSetIndex);
			const fbxsdk::FbxGeometryElementUV* pUVElement = pMesh->GetElementUV(uvSetName);
			if (nullptr == pUVElement)
			{
				continue;
			}

			fbxsdk::FbxGeometryElement::EMappingMode eMappingMode = pUVElement->GetMappingMode();
			fbxsdk::FbxGeometryElement::EReferenceMode eReferenceMode = pUVElement->GetReferenceMode();
			bool useIndex = fbxsdk::FbxGeometryElement::EReferenceMode::eDirect != eReferenceMode;
			int IndexCount = true == useIndex ? pUVElement->GetIndexArray().GetCount() : 0;

			int nPolygonCount = pMesh->GetPolygonCount();
			for (int PolygonIndex = 0; PolygonIndex < nPolygonCount; ++PolygonIndex)
			{
				const int PolygonSize = pMesh->GetPolygonSize(PolygonIndex);
				if (3 != PolygonSize)
				{
					GJHGameEngineDebug::AssertMsg("삼각형이 아닌 면이 발견됬습니다.");
				}

				for (int PositionInPolygon = 0; PositionInPolygon < PolygonSize; ++PositionInPolygon)
				{
					int convertUvIndex = isOddNegativeScale ? 2 - PositionInPolygon : PositionInPolygon;
					int ControlPointIndex = pMesh->GetPolygonVertex(PolygonIndex, PositionInPolygon);
					int UVMapIndex = (fbxsdk::FbxGeometryElement::EMappingMode::eByControlPoint == eMappingMode) ?
						ControlPointIndex : PolygonIndex * 3 + PositionInPolygon;

					int UvIndex = useIndex ? pUVElement->GetIndexArray().GetAt(UVMapIndex) : UVMapIndex;
					fbxsdk::FbxVector2 uvValue = pUVElement->GetDirectArray().GetAt(UvIndex);

					VtxData[ControlPointIndex].Uv.x = static_cast<float>(uvValue.mData[0]);
					VtxData[ControlPointIndex].Uv.y = 1.f - static_cast<float>(uvValue.mData[1]);
				}
			}
		}

		pMesh->GetElementMaterialCount();
		fbxsdk::FbxGeometryElementMaterial* pGeometryElementMaterial = pMesh->GetElementMaterial();
		fbxsdk::FbxGeometryElementNormal* pGeometryElementNormal = pMesh->GetElementNormal();

		int materialCount = pMeshNode->GetMaterialCount();
		IdxData.resize(materialCount);

		UINT VtxId = 0;

		int nPolygonCount = pMesh->GetPolygonCount();
		for (int PolygonIndex = 0; PolygonIndex < nPolygonCount; ++PolygonIndex)
		{
			int PolygonSize = pMesh->GetPolygonSize(PolygonIndex);
			if (3 != PolygonSize)
			{
				GJHGameEngineDebug::AssertMsg("삼각형이 아닌 면이 발견됬습니다.");
			}

			int IndexArray[3] = { 0, };
			for (int PositionInPolygon = 0; PositionInPolygon < PolygonSize; ++PositionInPolygon)
			{
				int ControlPointIndex = pMesh->GetPolygonVertex(PolygonIndex, PositionInPolygon);
				IndexArray[PositionInPolygon] = ControlPointIndex;

				LoadUv(pMesh, VtxData, VtxId, ControlPointIndex);
				LoadNormal(pMesh, meshMatrix, VtxData, VtxId, ControlPointIndex);
				LoadTangent(pMesh, meshMatrix, VtxData, VtxId, ControlPointIndex);
				LoadBinormal(pMesh, meshMatrix, VtxData, VtxId, ControlPointIndex);

				++VtxId;
			}

			int materialId = pGeometryElementMaterial->GetIndexArray().GetAt(PolygonIndex);
			IdxData[materialId].push_back(IndexArray[0]);
			IdxData[materialId].push_back(IndexArray[2]);
			IdxData[materialId].push_back(IndexArray[1]);
		}

		pDrawSet->m_FbxVtxMap.insert(std::make_pair(pMesh, &VtxData));
	}
}

void GJHDirectFbx::FbxRenderSetMeshCreate()
{
	std::map<int, std::shared_ptr<FbxRenderSet>>::iterator iter = m_mapRenderSet.begin();
	std::map<int, std::shared_ptr<FbxRenderSet>>::iterator iterEnd = m_mapRenderSet.end();
	for (; iter != iterEnd; ++iter)
	{
		FbxRenderSetMeshCreate((*iter).second);
	}
}


int GJHDirectFbx::GetRenderSetCount()
{
	return static_cast<int>(m_mapRenderSet.size());
}

int GJHDirectFbx::GetMeshSetCount(int _Index)
{
	if (m_mapRenderSet.end() == m_mapRenderSet.find(_Index))
	{
		return -1;
	}

	return static_cast<int>(m_mapRenderSet[_Index]->m_Mesh.size());
}

int GJHDirectFbx::GetMeshCount(int _SetIndex, int _Index)
{
	if (m_mapRenderSet.end() == m_mapRenderSet.find(_SetIndex))
	{
		return -1;
	}

	return static_cast<int>(m_mapRenderSet[_SetIndex]->m_Mesh[_Index].size());
}

void GJHDirectFbx::FbxRenderSetMeshCreate(std::shared_ptr<FbxRenderSet> _RenderData)
{
	if (_RenderData->m_FbxVtx.size() != _RenderData->m_FbxIdx.size())
	{
		GJHGameEngineDebug::AssertMsg("인덱스 버퍼 버텍스 버퍼 매칭 오류");
		return;
	}

	_RenderData->m_VB.resize(_RenderData->m_FbxVtx.size());
	_RenderData->m_IB.resize(_RenderData->m_FbxIdx.size());
	_RenderData->m_Mesh.resize(_RenderData->m_FbxVtx.size());

	for (size_t i = 0; i < _RenderData->m_FbxVtx.size(); i++)
	{
		std::shared_ptr<GJHDirectVertexBuffer> VB = std::make_shared<GJHDirectVertexBuffer>();
		VB->Create(&(*_RenderData->m_FbxVtx[i])[0], sizeof(VertexAnimation), (*_RenderData->m_FbxVtx[i]).size(), D3D11_USAGE::D3D11_USAGE_DEFAULT);
		_RenderData->m_VB[i] = VB;
		
		for (size_t j = 0; j < _RenderData->m_FbxIdx[i]->size(); j++)
		{
			if (0 == (*_RenderData->m_FbxIdx[i])[j].size())
			{
				continue;
			}

			std::shared_ptr<GJHDirectIndexBuffer> IB = std::make_shared<GJHDirectIndexBuffer>();
			IB->Create(&(*_RenderData->m_FbxIdx[i])[j][0]
				, sizeof(UINT)
				, (*_RenderData->m_FbxIdx[i])[j].size(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT
				, D3D11_USAGE::D3D11_USAGE_DEFAULT);
			_RenderData->m_IB[i].push_back(IB);

			std::shared_ptr<GJHDirectMesh> NewMesh = std::make_shared<GJHDirectMesh>();
			NewMesh->VertexBufferPush(VB);
			NewMesh->IndexBufferPush(IB);
			_RenderData->m_Mesh[i].push_back(NewMesh);
		}
	}

}


std::shared_ptr<GJHDirectMesh> GJHDirectFbx::GetMesh(int _MeshSetIndex, int _MeshIndex)
{
	return GetMesh(0, _MeshSetIndex, _MeshIndex);
}

std::shared_ptr<GJHDirectMesh> GJHDirectFbx::GetMesh(int _RenderSetIndex, int _MeshSetIndex, unsigned int _MeshIndex)
{
	if (m_mapRenderSet.end() == m_mapRenderSet.find(_RenderSetIndex))
	{
		GJHGameEngineDebug::AssertMsg("if (m_mapRenderSet.end() != m_mapRenderSet.find(_RenderSetIndex))");
		return nullptr;
	}

	std::shared_ptr<FbxRenderSet>& _RenderSet = m_mapRenderSet[_RenderSetIndex];

	if ((int)_RenderSet->m_Mesh.size() < _MeshSetIndex)
	{
		GJHGameEngineDebug::AssertMsg("if ((unsigned int)_RenderSet->m_Mesh.size() < _MeshSetIndex)");
		return nullptr;
	}

	if ((unsigned int)_RenderSet->m_Mesh[_MeshSetIndex].size() < _MeshIndex)
	{
		GJHGameEngineDebug::AssertMsg("if ((unsigned int)_RenderSet->m_Mesh[_MeshSetIndex].size() < _MeshIndex)");
		return nullptr;
	}

	if (nullptr == _RenderSet->m_Mesh[_MeshSetIndex][_MeshIndex])
	{
		GJHGameEngineDebug::AssertMsg("if (nullptr == _RenderSet->m_Mesh[_MeshSetIndex][_MeshIndex])");
		return nullptr;
	}

	return _RenderSet->m_Mesh[_MeshSetIndex][_MeshIndex];
}

bool GJHDirectFbx::RetrievePoseFromBindPose(fbxsdk::FbxScene* pScene, const std::vector<fbxsdk::FbxNode*>& NodeArray, fbxsdk::FbxArray<fbxsdk::FbxPose*>& PoseArray)
{
	const int PoseCount = pScene->GetPoseCount();
	for (int PoseIndex = 0; PoseIndex < PoseCount; PoseIndex++)
	{
		fbxsdk::FbxPose* CurrentPose = pScene->GetPose(PoseIndex);

		if (CurrentPose && CurrentPose->IsBindPose())
		{
			GJHGameEngineString PoseName = GJHGameEngineString::ConvertMultiByteString(CurrentPose->GetName());
			fbxsdk::FbxStatus Status;

			for (auto Current : NodeArray)
			{
				GJHGameEngineString CurrentName = GJHGameEngineString::ConvertMultiByteString(Current->GetName());
				fbxsdk::NodeList pMissingAncestors, pMissingDeformers, pMissingDeformersAncestors, pWrongMatrices;

				if (CurrentPose->IsValidBindPoseVerbose(Current, pMissingAncestors, pMissingDeformers, pMissingDeformersAncestors, pWrongMatrices, 0.0001, &Status))
				{
					PoseArray.Add(CurrentPose);
					break;
				}
				else
				{
					for (int i = 0; i < pMissingAncestors.GetCount(); i++)
					{
						fbxsdk::FbxAMatrix mat = pMissingAncestors.GetAt(i)->EvaluateGlobalTransform(FBXSDK_TIME_ZERO);
						CurrentPose->Add(pMissingAncestors.GetAt(i), mat);
					}

					pMissingAncestors.Clear();
					pMissingDeformers.Clear();
					pMissingDeformersAncestors.Clear();
					pWrongMatrices.Clear();

					if (CurrentPose->IsValidBindPose(Current))
					{
						PoseArray.Add(CurrentPose);
						break;
					}
					else
					{
						fbxsdk::FbxNode* ParentNode = Current->GetParent();
						while (ParentNode)
						{
							fbxsdk::FbxNodeAttribute* Attr = ParentNode->GetNodeAttribute();
							if (Attr && Attr->GetAttributeType() == fbxsdk::FbxNodeAttribute::eNull)
							{
								break;
							}

							ParentNode = ParentNode->GetParent();
						}

						if (ParentNode && CurrentPose->IsValidBindPose(ParentNode))
						{
							PoseArray.Add(CurrentPose);
							break;
						}
						else
						{
							GJHGameEngineString ErrorString = GJHGameEngineString::ConvertMultiByteString(Status.GetErrorString());
							GJHGameEngineString CurrentName = GJHGameEngineString::ConvertMultiByteString(Current->GetName());

							wchar_t ArrOutPut[1024] = L" %s   %s\n";
							swprintf_s(ArrOutPut, ErrorString.c_str(), CurrentName.c_str());
							GJHGameEngineDebug::OutPutMsg(ArrOutPut);
						}
					}
				}
			}
		}
	}

	return (PoseArray.Size() > 0);
}

fbxsdk::FbxNode* GJHDirectFbx::GetRootSkeleton(fbxsdk::FbxScene* pScene, fbxsdk::FbxNode* Link)
{
	fbxsdk::FbxNode* RootBone = Link;

	while (RootBone && RootBone->GetParent())
	{
		bool bIsBlenderArmatureBone = false;

		fbxsdk::FbxNodeAttribute* Attr = RootBone->GetParent()->GetNodeAttribute();
		if (Attr &&
			(Attr->GetAttributeType() == fbxsdk::FbxNodeAttribute::eMesh ||
				(Attr->GetAttributeType() == fbxsdk::FbxNodeAttribute::eNull && !bIsBlenderArmatureBone) ||
				Attr->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton) &&
			RootBone->GetParent() != pScene->GetRootNode())
		{
			if (Attr->GetAttributeType() == fbxsdk::FbxNodeAttribute::eMesh)
			{
				fbxsdk::FbxMesh* Mesh = (fbxsdk::FbxMesh*)Attr;
				if (Mesh->GetDeformerCount(fbxsdk::FbxDeformer::eSkin) > 0)
				{
					break;
				}
			}

			RootBone = RootBone->GetParent();
		}
		else
		{
			break;
		}
	}

	return RootBone;
}

bool GJHDirectFbx::IsBone(fbxsdk::FbxNode* Link)
{
	fbxsdk::FbxNodeAttribute* Attr = Link->GetNodeAttribute();
	if (Attr)
	{
		fbxsdk::FbxNodeAttribute::EType AttrType = Attr->GetAttributeType();
		if (AttrType == fbxsdk::FbxNodeAttribute::eSkeleton ||
			AttrType == fbxsdk::FbxNodeAttribute::eMesh ||
			AttrType == fbxsdk::FbxNodeAttribute::eNull)
		{
			return true;
		}
	}

	return false;
}

bool GJHDirectFbx::IsNull(fbxsdk::FbxNode* Link)
{
	fbxsdk::FbxNodeAttribute* Attr = Link->GetNodeAttribute();
	if (Attr)
	{
		fbxsdk::FbxNodeAttribute::EType AttrType = Attr->GetAttributeType();
		if (AttrType == fbxsdk::FbxNodeAttribute::eNull)
		{
			return true;
		}
	}
	return false;
}


void GJHDirectFbx::RecursiveBuildSkeleton(fbxsdk::FbxNode* Link, std::vector<fbxsdk::FbxNode*>& OutSortedLinks)
{
	if (IsBone(Link))
	{
		if (false == IsNull(Link))
		{
			OutSortedLinks.push_back(Link);
		}
		int ChildIndex;
		for (ChildIndex = 0; ChildIndex < Link->GetChildCount(); ChildIndex++)
		{
			RecursiveBuildSkeleton(Link->GetChild(ChildIndex), OutSortedLinks);
		}
	}
}

void GJHDirectFbx::BuildSkeletonSystem(fbxsdk::FbxScene* pScene, std::vector<fbxsdk::FbxCluster*>& ClusterArray, std::vector<fbxsdk::FbxNode*>& OutSortedLinks)
{
	fbxsdk::FbxNode* Link;
	std::vector<fbxsdk::FbxNode*> RootLinks;
	size_t ClusterIndex;
	for (ClusterIndex = 0; ClusterIndex < ClusterArray.size(); ClusterIndex++)
	{
		Link = ClusterArray[ClusterIndex]->GetLink();
		if (Link)
		{
			Link = GetRootSkeleton(pScene, Link);
			size_t LinkIndex;
			for (LinkIndex = 0; LinkIndex < RootLinks.size(); LinkIndex++)
			{
				if (Link == RootLinks[static_cast<int>(LinkIndex)])
				{
					break;
				}
			}

			if (LinkIndex == RootLinks.size())
			{
				RootLinks.push_back(Link);
			}
		}
	}

	for (size_t LinkIndex = 0; LinkIndex < RootLinks.size(); LinkIndex++)
	{
		RecursiveBuildSkeleton(RootLinks[LinkIndex], OutSortedLinks);
	}
}

void GJHDirectFbx::ImportBone()
{
	size_t meshCount = m_SceneInfo.MeshInfo.size();
	if (0 == meshCount)
	{
		return;
	}

	std::vector<fbxsdk::FbxNode*> NodeArray;
	std::vector<fbxsdk::FbxNode*> SortedLinks;
	std::vector<fbxsdk::FbxCluster*> ClusterArray;
	fbxsdk::FbxNode* Link = nullptr;
	int SkelType = 0;
	fbxsdk::FbxNode* SkeletalMeshNode = nullptr;

	for (size_t n = 0; n < meshCount; ++n)
	{
		FbxExMeshInfo& meshInfo = m_SceneInfo.MeshInfo.at(n);
		if (0 != meshInfo.LODLevel)
		{
			continue;
		}

		fbxsdk::FbxNode* pNode = meshInfo.pMesh->GetNode();
		fbxsdk::FbxMesh* FbxMesh = meshInfo.pMesh;

		NodeArray.push_back(pNode);
		const int SkinDeformerCount = FbxMesh->GetDeformerCount(fbxsdk::FbxDeformer::eSkin);
		for (int DeformerIndex = 0; DeformerIndex < SkinDeformerCount; DeformerIndex++)
		{
			fbxsdk::FbxSkin* Skin = (fbxsdk::FbxSkin*)FbxMesh->GetDeformer(DeformerIndex, fbxsdk::FbxDeformer::eSkin);
			for (int ClusterIndex = 0; ClusterIndex < Skin->GetClusterCount(); ClusterIndex++)
			{
				ClusterArray.push_back(Skin->GetCluster(ClusterIndex));
			}
		}
	}

	if (0 == ClusterArray.size())
	{
		return;
	}

	SkeletalMeshNode = NodeArray[0];

	fbxsdk::PoseList PoseArray;

	if (RetrievePoseFromBindPose(m_Scene, NodeArray, PoseArray) == false)
	{
		const int PoseCount = m_Scene->GetPoseCount();
		for (int PoseIndex = PoseCount - 1; PoseIndex >= 0; --PoseIndex)
		{
			fbxsdk::FbxPose* CurrentPose = m_Scene->GetPose(PoseIndex);

			if (CurrentPose && CurrentPose->IsBindPose())
			{
				m_Scene->RemovePose(PoseIndex);
				CurrentPose->Destroy();
			}
		}

		m_Manager->CreateMissingBindPoses(m_Scene);
		if (RetrievePoseFromBindPose(m_Scene, NodeArray, PoseArray) == false)
		{
			GJHGameEngineDebug::AssertMsg("Recreating bind pose failed.");
		}
		else
		{
		}
	}

	BuildSkeletonSystem(m_Scene, ClusterArray, SortedLinks);

	if (SortedLinks.size() == 0)
	{
		GJHGameEngineDebug::AssertMsg("연결된 링크가 없다.");
	}

	std::map<fbxsdk::FbxString, int> m_NameBoneCheck;
	size_t LinkIndex;

	for (LinkIndex = 0; LinkIndex < SortedLinks.size(); ++LinkIndex)
	{
		Link = SortedLinks[LinkIndex];
		m_NameBoneCheck.insert(std::make_pair(Link->GetName(), 0));

		for (size_t AltLinkIndex = LinkIndex + 1; AltLinkIndex < SortedLinks.size(); ++AltLinkIndex)
		{
			fbxsdk::FbxNode* AltLink = SortedLinks[AltLinkIndex];

			fbxsdk::FbxString tempLinkName = Link->GetName();
			fbxsdk::FbxString tempAltLinkName = AltLink->GetName();

			std::map<fbxsdk::FbxString, int>::iterator FindBoneNameIter = m_NameBoneCheck.find(tempAltLinkName);

			if (FindBoneNameIter != m_NameBoneCheck.end())
			{
				fbxsdk::FbxString newName = FindBoneNameIter->first;
				newName += "_";
				newName += (++FindBoneNameIter->second);

				std::map<fbxsdk::FbxString, int>::iterator RevertIter = m_NameBoneCheck.find(newName);

				while (RevertIter != m_NameBoneCheck.find(newName))
				{
					newName = FindBoneNameIter->first;
					newName += "_";
					newName += (++FindBoneNameIter->second);
					RevertIter = m_NameBoneCheck.find(newName);
				}

				AltLink->SetName(newName.Buffer());
			}
		}
	}

	fbxsdk::FbxArray<fbxsdk::FbxAMatrix> GlobalsPerLink;
	GlobalsPerLink.Grow(static_cast<int>(SortedLinks.size()));
	GlobalsPerLink[0] = m_AxisConversionMatrix;

	bool GlobalLinkFoundFlag;
	fbxsdk::FbxVector4 LocalLinkT;
	fbxsdk::FbxQuaternion LocalLinkQ;
	fbxsdk::FbxVector4	LocalLinkS;
	fbxsdk::FbxVector4 GlobalLinkT;
	fbxsdk::FbxQuaternion GlobalLinkQ;
	fbxsdk::FbxVector4	GlobalLinkS;

	bool bAnyLinksNotInBindPose = false;
	GJHGameEngineString LinksWithoutBindPoses;
	int NumberOfRoot = 0;

	int RootIdx = -1;

	for (LinkIndex = 0; LinkIndex < SortedLinks.size(); LinkIndex++)
	{
		m_vecRefBones.push_back(Bone());
		Bone& tempBoneData = m_vecRefBones.at(m_vecRefBones.size() - 1);
		tempBoneData.Index = static_cast<int>(m_vecRefBones.size() - 1);

		Link = SortedLinks[LinkIndex];

		int ParentIndex = -1;
		fbxsdk::FbxNode* LinkParent = Link->GetParent();
		if (LinkIndex)
		{
			for (int ll = 0; ll < LinkIndex; ++ll)
			{
				fbxsdk::FbxNode* Otherlink = SortedLinks[ll];
				if (Otherlink == LinkParent)
				{
					ParentIndex = ll;
					break;
				}
			}
		}

		if (ParentIndex == -1)
		{
			++NumberOfRoot;
			RootIdx = static_cast<int>(LinkIndex);
		}

		GlobalLinkFoundFlag = false;
		if (!SkelType)
		{
			if (PoseArray.GetCount())
			{
				for (int PoseIndex = 0; PoseIndex < PoseArray.GetCount(); PoseIndex++)
				{
					int PoseLinkIndex = PoseArray[PoseIndex]->Find(Link);
					if (PoseLinkIndex >= 0)
					{
						fbxsdk::FbxMatrix NoneAffineMatrix = PoseArray[PoseIndex]->GetMatrix(PoseLinkIndex);
						fbxsdk::FbxAMatrix Matrix = *(fbxsdk::FbxAMatrix*)(double*)&NoneAffineMatrix;
						GlobalsPerLink[static_cast<int>(LinkIndex)] = Matrix;
						GlobalLinkFoundFlag = true;
						break;
					}
				}
			}

			if (!GlobalLinkFoundFlag)
			{
				for (int ClusterIndex = 0; ClusterIndex < ClusterArray.size(); ClusterIndex++)
				{
					fbxsdk::FbxCluster* Cluster = ClusterArray[ClusterIndex];
					if (Link == Cluster->GetLink())
					{
						Cluster->GetTransformLinkMatrix(GlobalsPerLink[static_cast<int>(LinkIndex)]);
						GlobalLinkFoundFlag = true;
						break;
					}
				}
			}
		}

		if (!GlobalLinkFoundFlag)
		{
			GlobalsPerLink[static_cast<int>(LinkIndex)] = Link->EvaluateGlobalTransform();
		}

		GlobalsPerLink[static_cast<int>(LinkIndex)] = GlobalsPerLink[static_cast<int>(LinkIndex)];
		if (LinkIndex &&
			-1 != ParentIndex)
		{
			fbxsdk::FbxAMatrix	Matrix;
			Matrix = GlobalsPerLink[static_cast<int>(ParentIndex)].Inverse() * GlobalsPerLink[static_cast<int>(LinkIndex)];
			LocalLinkT = Matrix.GetT();
			LocalLinkQ = Matrix.GetQ();
			LocalLinkS = Matrix.GetS();
			GlobalLinkT = GlobalsPerLink[static_cast<int>(LinkIndex)].GetT();
			GlobalLinkQ = GlobalsPerLink[static_cast<int>(LinkIndex)].GetQ();
			GlobalLinkS = GlobalsPerLink[static_cast<int>(LinkIndex)].GetS();
		}
		else
		{
			GlobalLinkT = LocalLinkT = GlobalsPerLink[static_cast<int>(LinkIndex)].GetT();
			GlobalLinkQ = LocalLinkQ = GlobalsPerLink[static_cast<int>(LinkIndex)].GetQ();
			GlobalLinkS = LocalLinkS = GlobalsPerLink[static_cast<int>(LinkIndex)].GetS();
		}

		Bone& Bone = m_vecRefBones[static_cast<int>(LinkIndex)];

		Bone.Name = GJHGameEngineString::ConvertMultiByteString(Link->GetName());

		JointPos& JointMatrix = Bone.BonePos;
		fbxsdk::FbxSkeleton* Skeleton = Link->GetSkeleton();
		if (Skeleton)
		{
			JointMatrix.Length = (float)Skeleton->LimbLength.Get();
			JointMatrix.XSize = (float)Skeleton->Size.Get();
			JointMatrix.YSize = (float)Skeleton->Size.Get();
			JointMatrix.ZSize = (float)Skeleton->Size.Get();
		}
		else
		{
			JointMatrix.Length = 1.;
			JointMatrix.XSize = 100.;
			JointMatrix.YSize = 100.;
			JointMatrix.ZSize = 100.;
		}

		Bone.ParentIndex = ParentIndex;
		Bone.NumChildren = 0;
		for (int ChildIndex = 0; ChildIndex < Link->GetChildCount(); ChildIndex++)
		{
			fbxsdk::FbxNode* Child = Link->GetChild(ChildIndex);
			if (IsBone(Child))
			{
				Bone.NumChildren++;
			}
		}

		JointMatrix.SetTranslation(LocalLinkT);
		JointMatrix.SetRotation(LocalLinkQ);
		JointMatrix.SetScale(LocalLinkS);
		JointMatrix.SetGlobalTranslation(GlobalLinkT);
		JointMatrix.SetGlobalRotation(GlobalLinkQ);
		JointMatrix.SetGlobalScale(GlobalLinkS);
		JointMatrix.BuildMatrix();
	}
}


Bone* GJHDirectFbx::FindBone(const GJHGameEngineString& _boneName)
{
	if (m_MapRefBones.size() == 0)
	{
		std::vector<Bone>::iterator iter = m_vecRefBones.begin();
		std::vector<Bone>::iterator iterEnd = m_vecRefBones.end();
		for (; iter != iterEnd; ++iter)
		{
			if (m_MapRefBones.find((*iter).Name) == m_MapRefBones.end())
			{
				m_MapRefBones.insert(std::make_pair((*iter).Name, std::vector<Bone*>()));
			}

			m_MapRefBones[(*iter).Name].push_back(&(*iter));

		}
	}

	std::map<GJHGameEngineString, std::vector<Bone*>>::iterator StartIter = m_MapRefBones.find(_boneName);

	if (StartIter == m_MapRefBones.end())
	{
		return nullptr;
	}

	return StartIter->second[0];
}


void GJHDirectFbx::DrawSetWeightAndIndexSetting(std::shared_ptr<FbxRenderSet> _DrawSet, fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxCluster* _Cluster, int _BoneIndex)
{
	int iIndexCount = _Cluster->GetControlPointIndicesCount();

	for (size_t i = 0; i < iIndexCount; i++)
	{
		FbxExIW IW;
		IW.Index = _BoneIndex;

		IW.Weight = _Cluster->GetControlPointWeights()[i];
		int ControlPointIndices = _Cluster->GetControlPointIndices()[i];

		_DrawSet->m_MapWI[_Mesh][ControlPointIndices].push_back(IW);
	}
}


void GJHDirectFbx::LoadAnimationVertexData(std::shared_ptr<FbxRenderSet> _DrawData, const std::vector<FbxClusterData>& vecClusterData)
{
	for (auto& clusterData : vecClusterData)
	{
		if (nullptr == clusterData.m_Cluster->GetLink())
		{
			continue;
		}

		GJHGameEngineString StrBoneName = clusterData.LinkName;
		Bone* pBone = FindBone(StrBoneName);
		if (nullptr == pBone)
		{
			continue;
		}

		DrawSetWeightAndIndexSetting(_DrawData, clusterData.m_Mesh, clusterData.m_Cluster, pBone->Index);
	}
}


void GJHDirectFbx::CalAnimationVertexData(std::shared_ptr<FbxRenderSet> _RenderSet)
{
	for (auto& _WISet : _RenderSet->m_MapWI)
	{
		std::vector<VertexAnimation>* Ptr = _RenderSet->m_FbxVtxMap[_WISet.first];

		if (nullptr == Ptr)
		{
			GJHGameEngineDebug::AssertMsg("버텍스 데이터와 수집한 가중치 데이터가 매칭되지 않습니다.");
		}

		std::vector<VertexAnimation>& VertexData = *Ptr;

		for (auto& _WI : _WISet.second)
		{
			double dWeight = 0.0;
			for (int n = 0; n < _WI.second.size(); ++n)
			{
				dWeight += _WI.second[n].Weight;
			}

			for (int n = 0; n < _WI.second.size(); ++n)
			{
				_WI.second[n].Weight /= dWeight;
			}

			if (_WI.second.size() > 4)
			{
				std::sort(_WI.second.begin(), _WI.second.end(),
					[](const FbxExIW& _Left, const FbxExIW& _Right) {
						return _Left.Weight > _Right.Weight;
					}
				);

				double dInterPolate = 0.0;

				std::vector<FbxExIW>::iterator IterErase = _WI.second.begin() + 4;
				for (; IterErase != _WI.second.end(); ++IterErase)
				{
					dInterPolate += IterErase->Weight;
				}
				IterErase = _WI.second.begin() + 4;

				_WI.second.erase(IterErase, _WI.second.end());
				_WI.second[0].Weight += dInterPolate;
			};

			float Weight[4] = { 0.0f };
			int Index[4] = { 0 };

			for (size_t i = 0; i < _WI.second.size(); i++)
			{
				Weight[i] = (float)_WI.second[i].Weight;
				Index[i] = _WI.second[i].Index;
			}

			memcpy_s(VertexData[_WI.first].Weight.Arr, sizeof(float4), Weight, sizeof(float4));
			memcpy_s(VertexData[_WI.first].Index, sizeof(float4), Index, sizeof(float4));
		}
	}
}

void GJHDirectFbx::LoadSkinAndCluster()
{
	std::map<int, std::shared_ptr<FbxRenderSet>>::iterator iter = m_mapRenderSet.begin();
	std::map<int, std::shared_ptr<FbxRenderSet>>::iterator iterEnd = m_mapRenderSet.end();
	for (; iter != iterEnd; ++iter)
	{
		LoadAnimationVertexData((*iter).second, m_mapClusterData[(*iter).first]);
		CalAnimationVertexData((*iter).second);
	}
}

void GJHDirectFbx::CalFbxExBoneFrameTransMatrix(std::shared_ptr<GJHDirectFbx> _Fbx, std::function<void(float)> _AniProgress)
{
	AllAniMatirixCount = 0;
	CurMatrixCount = 0;

	if (m_UserAniData.size() == 0)
	{
		return;
	}

	for (size_t i = 0; i < m_UserAniData.size(); i++)
	{
		m_UserAniData[i].m_AniFrameData.resize(_Fbx->m_vecRefBones.size());
		AllAniMatirixCount += (int)m_UserAniData[i].m_AniFrameData.size() * (int)m_UserAniData[i].TimeEndCount;
	}

	AllAniMatirixCount *= _Fbx->m_RootNode->GetChildCount();

	ProcessAnimationLoad(_Fbx, _Fbx->m_RootNode, _AniProgress);

	ProcessAnimationCheckState(_Fbx);

	if (_AniProgress != nullptr)
	{
		_AniProgress(100.0f);
	}
}


size_t GJHDirectFbx::GetBoneCount()
{
	return m_vecRefBones.size();
}

unsigned int GJHDirectFbx::AniThreadCal(std::shared_ptr<GJHDirectFbx> _ThisFbx, std::shared_ptr<GJHDirectFbx> _Fbx, fbxsdk::FbxNode* pNode, int _ThreadIndex, int ThreadCount)
{
	std::vector<FbxExBoneFrame> vecAniFrameData;
	vecAniFrameData.resize(_Fbx->GetBoneCount());

	fbxsdk::FbxMesh* pCurrMesh = pNode->GetMesh();
	int deformerCount = pCurrMesh->GetDeformerCount();
	size_t userAniDataSize = _ThisFbx->m_UserAniData.size();
	fbxsdk::FbxAMatrix geometryTransform = _ThisFbx->GetGeometryTransformation(pNode);

	fbxsdk::FbxAnimStack* currAnimStack = _ThisFbx->m_Scene->GetSrcObject<fbxsdk::FbxAnimStack>(0);
	fbxsdk::FbxString animStackName = currAnimStack->GetName();
	fbxsdk::FbxTakeInfo* takeInfo = _ThisFbx->m_Scene->GetTakeInfo(animStackName);
	fbxsdk::FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
	fbxsdk::FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
	fbxsdk::FbxTime::EMode timeMode = _ThisFbx->m_Scene->GetGlobalSettings().GetTimeMode();

	fbxsdk::FbxAMatrix currentTransformOffset;
	fbxsdk::FbxAMatrix localTransform;
	fbxsdk::FbxAMatrix globalTransform;

	fbxsdk::FbxCluster* pCurrCluster;
	GJHGameEngineString currJointName;
	Bone* pBone;

	fbxsdk::FbxAMatrix transformMatrix;
	fbxsdk::FbxAMatrix transformLinkMatrix;
	fbxsdk::FbxAMatrix globalBindposeInverseMatrix;

	fbxsdk::FbxLongLong fixIndex;

	GJHGameEngineString linkName;

	fbxsdk::FbxLongLong endTime;
	fbxsdk::FbxLongLong startTime;

	fbxsdk::FbxTime currTime;

	// 254를 쓰레드 개수만큼 나누고.

	// 전체 총 쓰레드개수도 알고 있고
	// 내가 몇번째 쓰레드인지도 알고 있고.
	// start와 end도 알고 있다.



	for (int deformerIndex = 0; deformerIndex < deformerCount; ++deformerIndex)
	{
		fbxsdk::FbxSkin* pCurrSkin = reinterpret_cast<fbxsdk::FbxSkin*>(pCurrMesh->GetDeformer(deformerIndex, fbxsdk::FbxDeformer::eSkin));
		if (nullptr == pCurrSkin)
		{
			continue;
		}

		for (size_t userAniDataIndex = 0; userAniDataIndex < userAniDataSize; ++userAniDataIndex)
		{
			unsigned int numOfClusters = pCurrSkin->GetClusterCount();
			for (unsigned int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
			{
				fbxsdk::FbxNode* pLinkNode = nullptr;

				fbxsdk::FbxLongLong CurStartTime = fbxsdk::FbxLongLong();
				fbxsdk::FbxLongLong CurEndTime = fbxsdk::FbxLongLong();
				fbxsdk::FbxLongLong InterTime = fbxsdk::FbxLongLong();

				{
					std::lock_guard Lock(_ThisFbx->ThreadLock);

					pCurrCluster = pCurrSkin->GetCluster(clusterIndex);
					currJointName = GJHGameEngineString::ConvertMultiByteString(pCurrCluster->GetLink()->GetName());
					pBone = _Fbx->FindBone(currJointName);

					pCurrCluster->GetTransformMatrix(transformMatrix);
					pCurrCluster->GetTransformLinkMatrix(transformLinkMatrix);
					globalBindposeInverseMatrix = transformLinkMatrix.Inverse();

					endTime = end.GetFrameCount(timeMode);
					startTime = start.GetFrameCount(timeMode);

					FbxExAniData& CurAniData = _ThisFbx->m_UserAniData[userAniDataIndex];
					CurAniData.EndTime = endTime;
					CurAniData.StartTime = startTime;
					CurAniData.TimeMode = timeMode;
					CurAniData.m_AniFrameData[pBone->Index].m_Data.resize(endTime - startTime + 1);

					// 16개의 쓰레드가 있다.
					// 내가 몇번인지도 안다.

					fbxsdk::FbxLongLong CurStartTime = endTime;
					fbxsdk::FbxLongLong CurEndTime = startTime;
					fbxsdk::FbxLongLong InterTime = CurEndTime - CurStartTime;
					CurStartTime = CurStartTime + (InterTime * _ThreadIndex);
					CurEndTime = CurStartTime + InterTime;

					linkName = GJHGameEngineString::ConvertMultiByteString(pCurrCluster->GetLink()->GetName());
					pLinkNode = _ThisFbx->m_Scene->FindNodeByName(linkName.GetConvertMultiByteString().c_str());
				}

				for (fbxsdk::FbxLongLong i = CurStartTime; i <= CurEndTime; ++i)
				{
					fixIndex = i - CurStartTime;

					currTime.SetFrame(fixIndex, timeMode);
					currentTransformOffset = pNode->EvaluateGlobalTransform(currTime) * _ThisFbx->m_JointPostConversionMatrix * geometryTransform;

					{
						std::lock_guard Lock(_ThisFbx->ThreadLock);
						globalTransform = currentTransformOffset.Inverse() * pLinkNode->EvaluateGlobalTransform(currTime);
					}

					localTransform.SetS(pLinkNode->EvaluateLocalScaling(currTime));
					localTransform.SetR(pLinkNode->EvaluateLocalRotation(currTime));
					localTransform.SetT(pLinkNode->EvaluateLocalTranslation(currTime));

					FbxExBoneFrame& Frame = _ThisFbx->m_UserAniData[userAniDataIndex].m_AniFrameData[pBone->Index];
					FbxExBoneFrameData& FrameData = _ThisFbx->m_UserAniData[userAniDataIndex].m_AniFrameData[pBone->Index].m_Data[fixIndex];

					Frame.BoneIndex = pBone->Index;
					Frame.BoneParentIndex = pBone->ParentIndex;
					FrameData.Time = currTime.GetSecondDouble();
					FrameData.LocalAnimation = localTransform;
					FrameData.GlobalAnimation = globalTransform;
					FrameData.FrameMat = FbxMatTofloat4x4(FrameData.GlobalAnimation);
					FrameData.S = FbxVecTofloat4(FrameData.GlobalAnimation.GetS());
					FrameData.Q = FbxQuaternionTofloat4(FrameData.GlobalAnimation.GetQ());
					FrameData.T = FbxVecToTransform(FrameData.GlobalAnimation.GetT());

					//if (_AniProgress != nullptr)
					//{
					//	_AniProgress((float)++CurMatrixCount / (float)AllAniMatirixCount * 100.0f);
					//}
				}
			}
		}
	}

	return 0;
}

void GJHDirectFbx::MeshAnimationLoad(std::shared_ptr<GJHDirectFbx> _Fbx, fbxsdk::FbxNode* pNode, std::function<void(float)> _AniProgress)
{
	//int ThreadCount = GJHGameEngineThread::GetCoreCount();

	//std::vector<std::shared_ptr<std::thread>> ArrThread = std::vector<std::shared_ptr<std::thread>>();

	//for (int i = 0; i < ThreadCount; i++)
	//{
	//	std::shared_ptr<GJHDirectFbx> Fbx = std::dynamic_pointer_cast<GJHDirectFbx>(this->shared_from_this());

	//	std::shared_ptr<std::thread> NewThread = std::make_shared<std::thread>(&GJHDirectFbx::AniThreadCal, Fbx, _Fbx, pNode, i, ThreadCount);
	//	ArrThread.push_back(NewThread);
	//}

	//for (int i = 0; i < ArrThread.size(); i++)
	//{
	//	ArrThread[i]->join();
	//}

	//return;

	std::vector<FbxExBoneFrame> vecAniFrameData;
	vecAniFrameData.resize(_Fbx->GetBoneCount());

	fbxsdk::FbxMesh* pCurrMesh = pNode->GetMesh();
	int deformerCount = pCurrMesh->GetDeformerCount();
	size_t userAniDataSize = m_UserAniData.size();
	fbxsdk::FbxAMatrix geometryTransform = GetGeometryTransformation(pNode);

	fbxsdk::FbxAnimStack* currAnimStack = m_Scene->GetSrcObject<fbxsdk::FbxAnimStack>(0);
	fbxsdk::FbxString animStackName = currAnimStack->GetName();
	fbxsdk::FbxTakeInfo* takeInfo = m_Scene->GetTakeInfo(animStackName);
	fbxsdk::FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
	fbxsdk::FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
	fbxsdk::FbxTime::EMode timeMode = m_Scene->GetGlobalSettings().GetTimeMode();

	fbxsdk::FbxAMatrix currentTransformOffset;
	fbxsdk::FbxAMatrix localTransform;
	fbxsdk::FbxAMatrix globalTransform;

	fbxsdk::FbxCluster* pCurrCluster;
	GJHGameEngineString currJointName;
	Bone* pBone;

	fbxsdk::FbxAMatrix transformMatrix;
	fbxsdk::FbxAMatrix transformLinkMatrix;
	fbxsdk::FbxAMatrix globalBindposeInverseMatrix;

	fbxsdk::FbxLongLong fixIndex;

	GJHGameEngineString linkName;

	fbxsdk::FbxLongLong endTime;
	fbxsdk::FbxLongLong startTime;

	fbxsdk::FbxTime currTime;


	for (int deformerIndex = 0; deformerIndex < deformerCount; ++deformerIndex)
	{
		fbxsdk::FbxSkin* pCurrSkin = reinterpret_cast<fbxsdk::FbxSkin*>(pCurrMesh->GetDeformer(deformerIndex, fbxsdk::FbxDeformer::eSkin));
		if (nullptr == pCurrSkin)
		{
			continue;
		}

		for (size_t userAniDataIndex = 0; userAniDataIndex < userAniDataSize; ++userAniDataIndex)
		{
			unsigned int numOfClusters = pCurrSkin->GetClusterCount();

			endTime = end.GetFrameCount(timeMode);
			startTime = start.GetFrameCount(timeMode);

			FbxExAniData& CurAniData = m_UserAniData[userAniDataIndex];
			CurAniData.EndTime = endTime;
			CurAniData.StartTime = startTime;
			CurAniData.TimeMode = timeMode;

			for (unsigned int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
			{
				pCurrCluster = pCurrSkin->GetCluster(clusterIndex);
				currJointName = GJHGameEngineString::ConvertMultiByteString(pCurrCluster->GetLink()->GetName());
				pBone = _Fbx->FindBone(currJointName);

				pCurrCluster->GetTransformMatrix(transformMatrix);
				pCurrCluster->GetTransformLinkMatrix(transformLinkMatrix);
				globalBindposeInverseMatrix = transformLinkMatrix.Inverse();

				linkName = GJHGameEngineString::ConvertMultiByteString(pCurrCluster->GetLink()->GetName());
				fbxsdk::FbxNode* pLinkNode = m_Scene->FindNodeByName(linkName.GetConvertMultiByteString().c_str());

				FbxExBoneFrame& Frame = CurAniData.m_AniFrameData[pBone->Index];
				Frame.m_Data.resize(endTime - startTime + 1);
				Frame.BoneIndex = pBone->Index;
				Frame.BoneParentIndex = pBone->ParentIndex;

				for (fbxsdk::FbxLongLong i = startTime; i <= endTime; ++i)
				{
					fixIndex = i - startTime;

					FbxExBoneFrameData& FrameData = Frame.m_Data[fixIndex];

					currTime.SetFrame(fixIndex, timeMode);
					currentTransformOffset = pNode->EvaluateGlobalTransform(currTime) * m_JointPostConversionMatrix * geometryTransform;
					globalTransform = currentTransformOffset.Inverse() * pLinkNode->EvaluateGlobalTransform(currTime);

					localTransform.SetS(pLinkNode->EvaluateLocalScaling(currTime));
					localTransform.SetR(pLinkNode->EvaluateLocalRotation(currTime));
					localTransform.SetT(pLinkNode->EvaluateLocalTranslation(currTime));

					FrameData.Time = currTime.GetSecondDouble();
					FrameData.LocalAnimation = localTransform;
					FrameData.GlobalAnimation = globalTransform;
					FrameData.FrameMat = FbxMatTofloat4x4(FrameData.GlobalAnimation);
					FrameData.S = FbxVecTofloat4(FrameData.GlobalAnimation.GetS());
					FrameData.Q = FbxQuaternionTofloat4(FrameData.GlobalAnimation.GetQ());
					FrameData.T = FbxVecToTransform(FrameData.GlobalAnimation.GetT());

					if (_AniProgress != nullptr)
					{
						_AniProgress((float)++CurMatrixCount / (float)AllAniMatirixCount * 100.0f);
					}
				}
			}
		}
	}
}

void GJHDirectFbx::ProcessAnimationLoad(std::shared_ptr<GJHDirectFbx> _Fbx, fbxsdk::FbxNode* pNode, std::function<void(float)> _AniProgress)
{
	fbxsdk::FbxNodeAttribute* pNodeAttribute = pNode->GetNodeAttribute();
	if (nullptr != pNodeAttribute)
	{
		fbxsdk::FbxNodeAttribute::EType typetype = pNodeAttribute->GetAttributeType();
		switch (pNodeAttribute->GetAttributeType())
		{
		case fbxsdk::FbxNodeAttribute::eLODGroup:
		{
			if (0 != pNode->GetChildCount())
			{
				ProcessAnimationLoad(_Fbx, pNode->GetChild(0), _AniProgress);
				return;
			}
		}
		break;
		case fbxsdk::FbxNodeAttribute::eMesh:
		{
			MeshAnimationLoad(_Fbx, pNode, _AniProgress);
		}
		break;
		default:
			break;
		}
	}

	for (int n = 0; n < pNode->GetChildCount(); ++n)
	{
		ProcessAnimationLoad(_Fbx, pNode->GetChild(n), _AniProgress);
	}
}


void GJHDirectFbx::ProcessAnimationCheckState(std::shared_ptr<GJHDirectFbx> _Fbx)
{
	size_t userAniDataSize = m_UserAniData.size();
	for (size_t userAniDataIndex = 0; userAniDataIndex < userAniDataSize; ++userAniDataIndex)
	{
		FbxExAniData& userAniData = m_UserAniData.at(userAniDataIndex);
		fbxsdk::FbxLongLong fbxTime = userAniData.EndTime.Get() - userAniData.StartTime.Get() + 1;
		size_t aniFrameDataSize = userAniData.m_AniFrameData.size();
		for (size_t aniFrameDataIndex = 0; aniFrameDataIndex < aniFrameDataSize; ++aniFrameDataIndex)
		{
			FbxExBoneFrame& aniFrameData = userAniData.m_AniFrameData.at(aniFrameDataIndex);
			if (0 == aniFrameData.m_Data.size())
			{
				aniFrameData.m_Data.resize(fbxTime);
				Bone& curBone = _Fbx->m_vecRefBones[aniFrameDataIndex];
				aniFrameData.BoneIndex = curBone.Index;
				aniFrameData.BoneParentIndex = curBone.ParentIndex;
				if (-1 != curBone.ParentIndex)
				{
					FbxExBoneFrame& parentAniFrameData = userAniData.m_AniFrameData.at(curBone.ParentIndex);
					for (fbxsdk::FbxLongLong start = 0; start < fbxTime; ++start)
					{
						aniFrameData.m_Data[start].Time = parentAniFrameData.m_Data[start].Time;
						aniFrameData.m_Data[start].LocalAnimation = float4x4ToFbxAMatrix(curBone.BonePos.Local);
						aniFrameData.m_Data[start].GlobalAnimation = parentAniFrameData.m_Data[start].GlobalAnimation * aniFrameData.m_Data[start].LocalAnimation;
						aniFrameData.m_Data[start].FrameMat = FbxMatTofloat4x4(aniFrameData.m_Data[start].GlobalAnimation);
						aniFrameData.m_Data[start].S = FbxVecTofloat4(aniFrameData.m_Data[start].GlobalAnimation.GetS());
						aniFrameData.m_Data[start].Q = FbxQuaternionTofloat4(aniFrameData.m_Data[start].GlobalAnimation.GetQ());
						aniFrameData.m_Data[start].T = FbxVecToTransform(aniFrameData.m_Data[start].GlobalAnimation.GetT());
					}
				}
				else
				{
					for (fbxsdk::FbxLongLong start = 0; start < fbxTime; ++start)
					{
						aniFrameData.m_Data[start].Time = 0;
						aniFrameData.m_Data[start].LocalAnimation = float4x4ToFbxAMatrix(curBone.BonePos.Local);
						aniFrameData.m_Data[start].GlobalAnimation = aniFrameData.m_Data[start].LocalAnimation;
						aniFrameData.m_Data[start].FrameMat = FbxMatTofloat4x4(aniFrameData.m_Data[start].GlobalAnimation);
						aniFrameData.m_Data[start].S = FbxVecTofloat4(aniFrameData.m_Data[start].GlobalAnimation.GetS());
						aniFrameData.m_Data[start].Q = FbxQuaternionTofloat4(aniFrameData.m_Data[start].GlobalAnimation.GetQ());
						aniFrameData.m_Data[start].T = FbxVecToTransform(aniFrameData.m_Data[start].GlobalAnimation.GetT());
					}
				}
			}
		}
	}
}

fbxsdk::FbxAMatrix GJHDirectFbx::GetGeometryTransformation(fbxsdk::FbxNode* pNode)
{
	fbxsdk::FbxVector4 translation = pNode->GetGeometricTranslation(fbxsdk::FbxNode::eSourcePivot);
	fbxsdk::FbxVector4 rotation = pNode->GetGeometricRotation(fbxsdk::FbxNode::eSourcePivot);
	fbxsdk::FbxVector4 scale = pNode->GetGeometricScaling(fbxsdk::FbxNode::eSourcePivot);

	return fbxsdk::FbxAMatrix(translation, rotation, scale);
}


FbxExFaceSettingData* GJHDirectFbx::GetMaterialData(int _RenderSetIndex, int _MatSetIndex, int _MatIndex)
{
	if (m_mapRenderSet.end() == m_mapRenderSet.find(_RenderSetIndex))
	{
		return nullptr;
	}

	if (_MatSetIndex > m_mapRenderSet[_RenderSetIndex]->m_MatialData.size())
	{
		return nullptr;
	}

	if (_MatIndex > m_mapRenderSet[_RenderSetIndex]->m_MatialData[_MatSetIndex].size())
	{
		return nullptr;
	}

	return &m_mapRenderSet[_RenderSetIndex]->m_MatialData[_MatSetIndex][_MatIndex];
}

FbxExAniData* GJHDirectFbx::GetAniData(int _Index)
{
	if (static_cast<size_t>(_Index) >= m_UserAniData.size())
	{
		GJHGameEngineDebug::AssertMsg("존재하지 않는 애니메이션 인덱스입니다.");
	}

	return &m_UserAniData[_Index];
}

Bone* GJHDirectFbx::FindBone(int _Index)
{
	if (m_vecRefBones.size() <= _Index)
	{
		GJHGameEngineDebug::AssertMsg("본이 존재하지 않습니다.");
		return nullptr;
	}

	return &m_vecRefBones[_Index];
}

bool GJHDirectFbx::SaveUserMesh(const GJHGameEngineString& _File)
{
	if (m_mapRenderSet.size() == 0)
	{
		return true;
	}

	GJHGameEngineFile SaveFile = GJHGameEngineFile(_File);
	SaveFile.Open(_File, L"wb");

	SaveFile << (int)m_mapRenderSet.size();

	for (auto RenderSet : m_mapRenderSet)
	{
		SaveFile << RenderSet.first;
		SaveFile << (int)RenderSet.second->m_FbxVtx.size();
		SaveFile << (int)RenderSet.second->m_FbxIdx.size();
		SaveFile << (int)RenderSet.second->m_MatialData.size();

		for (size_t i = 0; i < RenderSet.second->m_FbxVtx.size(); ++i)
		{
			std::vector<VertexAnimation>& VertexList = (*RenderSet.second->m_FbxVtx[i]);
			SaveFile << (int)VertexList.size();

			for (size_t j = 0; j < VertexList.size(); j++)
			{
				SaveFile << VertexList[j];
			}
		}

		for (size_t i = 0; i < RenderSet.second->m_FbxIdx.size(); ++i)
		{
			std::vector<std::vector<UINT>>& IdxSetList = (*RenderSet.second->m_FbxIdx[i]);
			SaveFile << (int)IdxSetList.size();

			for (size_t j = 0; j < IdxSetList.size(); j++)
			{
				SaveFile << (int)IdxSetList[j].size();

				for (size_t k = 0; k < IdxSetList[j].size(); k++)
				{
					SaveFile << IdxSetList[j][k];
				}
			}
		}

		for (size_t i = 0; i < RenderSet.second->m_MatialData.size(); ++i)
		{
			SaveFile << (int)RenderSet.second->m_MatialData[i].size();
			for (size_t j = 0; j < RenderSet.second->m_MatialData[i].size(); j++)
			{
				SaveFile << RenderSet.second->m_MatialData[i][j].Name;
				SaveFile << RenderSet.second->m_MatialData[i][j].AmbColor;
				SaveFile << RenderSet.second->m_MatialData[i][j].SpcColor;
				SaveFile << RenderSet.second->m_MatialData[i][j].EmvColor;
				SaveFile << RenderSet.second->m_MatialData[i][j].SpecularPower;
				SaveFile << RenderSet.second->m_MatialData[i][j].TransparencyFactor;
				SaveFile << RenderSet.second->m_MatialData[i][j].Shininess;
				SaveFile << RenderSet.second->m_MatialData[i][j].DifTexturePath; 
				SaveFile << RenderSet.second->m_MatialData[i][j].BmpTexturePath;
				SaveFile << RenderSet.second->m_MatialData[i][j].SpcTexturePath;
				SaveFile << RenderSet.second->m_MatialData[i][j].DifTextureName; 
				SaveFile << RenderSet.second->m_MatialData[i][j].BmpTextureName;
				SaveFile << RenderSet.second->m_MatialData[i][j].SpcTextureName;
			}
		}
	}

	SaveFile << (int)m_vecRefBones.size();

	for (size_t i = 0; i < m_vecRefBones.size(); i++)
	{
		SaveFile << m_vecRefBones[i].Name;
		SaveFile << m_vecRefBones[i].Flags;
		SaveFile << m_vecRefBones[i].NumChildren;
		SaveFile << m_vecRefBones[i].Index;
		SaveFile << m_vecRefBones[i].ParentIndex;
		SaveFile << m_vecRefBones[i].BonePos;
	}

	return true;
}

bool GJHDirectFbx::LoadUserMeshData(const GJHGameEngineString& _UserPath)
{
	GJHGameEngineFile LoadFile;

	LoadFile.Open(_UserPath, L"rb");

	int Count = 0;
	LoadFile >> Count;

	for (size_t i = 0; i < Count; i++)
	{
		int Key = 0;
		int VtxSetCount = 0;
		int IdxSetCount = 0;
		int MatSetCount = 0;
		LoadFile >> Key;
		LoadFile >> VtxSetCount;
		LoadFile >> IdxSetCount;
		LoadFile >> MatSetCount;

		std::shared_ptr<FbxRenderSet> NewRenderSet = std::make_shared<FbxRenderSet>();
		m_mapRenderSet.insert(std::map<int, std::shared_ptr<FbxRenderSet>>::value_type(Key, NewRenderSet));
		NewRenderSet->m_FbxVtx.resize(VtxSetCount);

		for (size_t i = 0; i < VtxSetCount; ++i)
		{
			int VtxCount = 0;
			LoadFile >> VtxCount;
			NewRenderSet->m_FbxVtx[i] = new std::vector<VertexAnimation>();
			NewRenderSet->m_FbxVtx[i]->reserve(VtxCount);

			for (size_t j = 0; j < VtxCount; j++)
			{
				VertexAnimation LoadVtx;
				LoadFile >> LoadVtx;
				NewRenderSet->m_FbxVtx[i]->push_back(LoadVtx);
			}
		}

		NewRenderSet->m_FbxIdx.resize(IdxSetCount);
		for (size_t i = 0; i < IdxSetCount; ++i)
		{
			int IdxCount = 0;
			LoadFile >> IdxCount;
			NewRenderSet->m_FbxIdx[i] = new std::vector<std::vector<UINT>>();
			NewRenderSet->m_FbxIdx[i]->resize(IdxCount);
			for (size_t j = 0; j < IdxCount; j++)
			{
				int Count = 0;
				LoadFile >> Count;

				NewRenderSet->m_FbxIdx[i]->operator[](j).reserve(Count);

				for (size_t k = 0; k < Count; k++)
				{
					UINT Idx;
					LoadFile >> Idx;
					NewRenderSet->m_FbxIdx[i]->operator[](j).push_back(Idx);
				}
			}
		}

		NewRenderSet->m_MatialData.resize(MatSetCount);
		for (size_t i = 0; i < MatSetCount; ++i)
		{
			int MatCount = 0;
			LoadFile >> MatCount;
			NewRenderSet->m_MatialData[i].resize(MatCount);

			for (size_t j = 0; j < MatCount; j++)
			{
				LoadFile >> NewRenderSet->m_MatialData[i][j].Name;
				LoadFile >> NewRenderSet->m_MatialData[i][j].AmbColor;
				LoadFile >> NewRenderSet->m_MatialData[i][j].SpcColor;
				LoadFile >> NewRenderSet->m_MatialData[i][j].EmvColor;
				LoadFile >> NewRenderSet->m_MatialData[i][j].SpecularPower;
				LoadFile >> NewRenderSet->m_MatialData[i][j].TransparencyFactor;
				LoadFile >> NewRenderSet->m_MatialData[i][j].Shininess;
				LoadFile >> NewRenderSet->m_MatialData[i][j].DifTexturePath; 
				LoadFile >> NewRenderSet->m_MatialData[i][j].BmpTexturePath;
				LoadFile >> NewRenderSet->m_MatialData[i][j].SpcTexturePath;
				LoadFile >> NewRenderSet->m_MatialData[i][j].DifTextureName; 
				LoadFile >> NewRenderSet->m_MatialData[i][j].BmpTextureName;
				LoadFile >> NewRenderSet->m_MatialData[i][j].SpcTextureName;

				if (GJHGameEnginePath::IsExist(NewRenderSet->m_MatialData[i][j].DifTexturePath) == true)
				{
					GJHDirectTexture2D::Load(NewRenderSet->m_MatialData[i][j].DifTexturePath);
				}
			}
		}
	}

	int BoneCount = 0;
	LoadFile >> BoneCount;

	m_vecRefBones.resize(BoneCount);
	for (size_t i = 0; i < m_vecRefBones.size(); i++)
	{
		LoadFile >> m_vecRefBones[i].Name;
		LoadFile >> m_vecRefBones[i].Flags;
		LoadFile >> m_vecRefBones[i].NumChildren;
		LoadFile >> m_vecRefBones[i].Index;
		LoadFile >> m_vecRefBones[i].ParentIndex;
		LoadFile >> m_vecRefBones[i].BonePos;
	}

	FbxRenderSetMeshCreate();

	return true;
}

bool GJHDirectFbx::SaveUserAni(const GJHGameEngineString& _FbxName)
{
	if (m_UserAniData.size() == 0)
	{
		return false;
	}

	GJHGameEngineFile SaveFile = GJHGameEngineFile(_FbxName);
	SaveFile.Open(_FbxName, L"wb");

	SaveFile << (int)m_UserAniData.size();

	for (size_t i = 0; i < m_UserAniData.size(); i++)
	{
		SaveFile << m_UserAniData[i].AniName;
		SaveFile << m_UserAniData[i].StartTime;
		SaveFile << m_UserAniData[i].EndTime;
		SaveFile << m_UserAniData[i].TimeStartCount;
		SaveFile << m_UserAniData[i].TimeEndCount;
		SaveFile << m_UserAniData[i].FrameCount;
		SaveFile << m_UserAniData[i].TimeMode;
		SaveFile << m_UserAniData[i].FbxModeCount;
		SaveFile << m_UserAniData[i].FbxModeRate;

		SaveFile << (int)m_UserAniData[i].m_AniFrameData.size();

		for (size_t j = 0; j < m_UserAniData[i].m_AniFrameData.size(); j++)
		{
			SaveFile << m_UserAniData[i].m_AniFrameData[j].BoneIndex;
			SaveFile << m_UserAniData[i].m_AniFrameData[j].BoneParentIndex;
			SaveFile << m_UserAniData[i].m_AniFrameData[j].m_Data;
		}
	}

	return true;
}

bool GJHDirectFbx::LoadUserAniData(const GJHGameEngineString& _UserPath)
{
	GJHGameEngineFile LoadFile = GJHGameEngineFile(_UserPath);
	LoadFile.Open(_UserPath, L"rb");

	int AniCount;
	LoadFile >> AniCount;
	m_UserAniData.resize(AniCount);

	for (size_t i = 0; i < m_UserAniData.size(); i++)
	{
		LoadFile >> m_UserAniData[i].AniName;
		LoadFile >> m_UserAniData[i].StartTime;
		LoadFile >> m_UserAniData[i].EndTime;
		LoadFile >> m_UserAniData[i].TimeStartCount;
		LoadFile >> m_UserAniData[i].TimeEndCount;
		LoadFile >> m_UserAniData[i].FrameCount;
		LoadFile >> m_UserAniData[i].TimeMode;
		LoadFile >> m_UserAniData[i].FbxModeCount;
		LoadFile >> m_UserAniData[i].FbxModeRate;

		int FrameMatCount = 0;
		LoadFile >> FrameMatCount;
		m_UserAniData[i].m_AniFrameData.resize(FrameMatCount);

		for (size_t j = 0; j < m_UserAniData[i].m_AniFrameData.size(); j++)
		{
			LoadFile >> m_UserAniData[i].m_AniFrameData[j].BoneIndex;
			LoadFile >> m_UserAniData[i].m_AniFrameData[j].BoneParentIndex;
			LoadFile >> m_UserAniData[i].m_AniFrameData[j].m_Data;
		}
	}

	return true;
}