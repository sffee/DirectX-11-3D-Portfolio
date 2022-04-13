#include "GeoPre.h"
#include "GJHGameEngineCollision.h"
#include "GJHGameEngineTransForm.h"

GJHGameEngineCollision::StarterClass GJHGameEngineCollision::Starter = GJHGameEngineCollision::StarterClass();

bool(__stdcall* GJHGameEngineCollision::ColCheckFunc[(int)COLTYPE::NONE][(int)COLTYPE::NONE])(GJHGameEngineTransform&, GJHGameEngineTransform&) = { nullptr, };

void GJHGameEngineCollision::Init()
{
	ColCheckFunc[(int)COLTYPE::AABB][(int)COLTYPE::AABB] = &GJHGameEngineCollision::AABBToAABB;
	ColCheckFunc[(int)COLTYPE::AABB][(int)COLTYPE::OBB] = &GJHGameEngineCollision::AABBToOBB;
	ColCheckFunc[(int)COLTYPE::AABB][(int)COLTYPE::SPHERE] = &GJHGameEngineCollision::AABBToSPHERE;
	ColCheckFunc[(int)COLTYPE::AABB][(int)COLTYPE::POINT] = &GJHGameEngineCollision::AABBToPOINT;
	ColCheckFunc[(int)COLTYPE::OBB][(int)COLTYPE::AABB] = &GJHGameEngineCollision::OBBToAABB;
	ColCheckFunc[(int)COLTYPE::OBB][(int)COLTYPE::OBB] = &GJHGameEngineCollision::OBBToOBB;
	ColCheckFunc[(int)COLTYPE::OBB][(int)COLTYPE::SPHERE] = &GJHGameEngineCollision::OBBToSPHERE;
	ColCheckFunc[(int)COLTYPE::OBB][(int)COLTYPE::POINT] = &GJHGameEngineCollision::OBBToPOINT;
	ColCheckFunc[(int)COLTYPE::SPHERE][(int)COLTYPE::AABB] = &GJHGameEngineCollision::SPHEREToAABB;
	ColCheckFunc[(int)COLTYPE::SPHERE][(int)COLTYPE::OBB] = &GJHGameEngineCollision::SPHEREToOBB;
	ColCheckFunc[(int)COLTYPE::SPHERE][(int)COLTYPE::SPHERE] = &GJHGameEngineCollision::SPHEREToSPHERE;
	ColCheckFunc[(int)COLTYPE::SPHERE][(int)COLTYPE::POINT] = &GJHGameEngineCollision::SPHEREToPOINT;
	ColCheckFunc[(int)COLTYPE::POINT][(int)COLTYPE::AABB] = &GJHGameEngineCollision::POINTToAABB;
	ColCheckFunc[(int)COLTYPE::POINT][(int)COLTYPE::OBB] = &GJHGameEngineCollision::POINTToOBB;
	ColCheckFunc[(int)COLTYPE::POINT][(int)COLTYPE::SPHERE] = &GJHGameEngineCollision::POINTToSPHERE;
	ColCheckFunc[(int)COLTYPE::POINT][(int)COLTYPE::POINT] = &GJHGameEngineCollision::POINTToPOINT;
}

bool GJHGameEngineCollision::OBBToOBB(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	return _LeftTransform.OBB().Intersects(_RightTransform.OBB());
}

bool GJHGameEngineCollision::OBBToAABB(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	return _LeftTransform.OBB().Intersects(_RightTransform.AABB());
}
bool GJHGameEngineCollision::OBBToSPHERE(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	return _LeftTransform.OBB().Intersects(_RightTransform.SPHERE());
}
bool GJHGameEngineCollision::OBBToPOINT(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	DirectX::BoundingSphere BS = _RightTransform.SPHERE();
	BS.Radius = 0.001f;

	return _LeftTransform.OBB().Intersects(BS);
}

bool GJHGameEngineCollision::AABBToAABB(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	return _LeftTransform.AABB().Intersects(_RightTransform.AABB());
}
bool GJHGameEngineCollision::AABBToOBB(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	return _LeftTransform.AABB().Intersects(_RightTransform.OBB());
}
bool GJHGameEngineCollision::AABBToSPHERE(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	return _LeftTransform.AABB().Intersects(_RightTransform.SPHERE());
}
bool GJHGameEngineCollision::AABBToPOINT(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	DirectX::BoundingSphere BS = _RightTransform.SPHERE();
	BS.Radius = 0.001f;

	return _LeftTransform.AABB().Intersects(BS);
}

bool GJHGameEngineCollision::SPHEREToSPHERE(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	return _LeftTransform.SPHERE().Intersects(_RightTransform.SPHERE());
}

bool GJHGameEngineCollision::SPHEREToOBB(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	return _LeftTransform.SPHERE().Intersects(_RightTransform.OBB());
}

bool GJHGameEngineCollision::SPHEREToAABB(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	return _LeftTransform.SPHERE().Intersects(_RightTransform.AABB());
}

bool GJHGameEngineCollision::SPHEREToPOINT(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	DirectX::BoundingSphere BS = _RightTransform.SPHERE();
	BS.Radius = 0.001f;

	return _LeftTransform.SPHERE().Intersects(BS);
}

bool GJHGameEngineCollision::POINTToPOINT(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	float4 LEFT = { _LeftTransform.SPHERE().Center.x, _LeftTransform.SPHERE().Center.y, _LeftTransform.SPHERE().Center.z };
	float4 RIGHT = { _RightTransform.SPHERE().Center.x, _RightTransform.SPHERE().Center.y, _RightTransform.SPHERE().Center.z };

	return LEFT == RIGHT;
}

bool GJHGameEngineCollision::POINTToOBB(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	return OBBToPOINT(_RightTransform, _LeftTransform);
}

bool GJHGameEngineCollision::POINTToAABB(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	return AABBToPOINT(_RightTransform, _LeftTransform);
}

bool GJHGameEngineCollision::POINTToSPHERE(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform)
{
	return SPHEREToPOINT(_RightTransform, _LeftTransform);
}

GJHGameEngineCollision::GJHGameEngineCollision(GJHGameEngineTransform& _Transform) :
	m_Transform(_Transform),
	Type(COLTYPE::NONE),
	m_Data(nullptr),
	m_IsColCheck(true)
{

}

GJHGameEngineCollision::~GJHGameEngineCollision()
{

}

void GJHGameEngineCollision::ColUpdate()
{
	m_Transform.ColUpdate(Type);
}

bool GJHGameEngineCollision::ColCheck(GJHGameEngineTransform& _Other, COLTYPE _Type) 
{
	// 복사본 만들면 원본은 안바뀌니까 이러려고 했는데.

	return ColCheckFunc[(int)Type][(int)_Type](m_Transform, _Other);
}

bool GJHGameEngineCollision::ColCheck(GJHGameEngineCollision& _Other)
{
	bool Check;

	if (m_IsColCheck == false
		|| _Other.GetColCheck() == false)
	{
		Check = false;
	}
	else
	{
		Check = ColCheckFunc[(int)Type][(int)_Other.Type](m_Transform, _Other.m_Transform);
	}

	if (Check == true)
	{
		if (m_ColTrueEvent != nullptr)
		{
			m_ColTrueEvent(&_Other);
		}
	}
	else
	{
		if (m_ColFalseEvent != nullptr)
		{
			m_ColFalseEvent(&_Other);
		}
	}

	return Check;
}

bool GJHGameEngineCollision::TriToRayContaine(float4 _Tri0, float4 _Tri1, float4 _Tri2, float4 _RayStart, float4 _RayDir)
{
	{
		float4 StartRayPos = _RayStart;
		float4 RayDir = _RayDir;
		float Dist = 0.f;
		DirectX::TriangleTests::Intersects(StartRayPos.DirectVector, RayDir.DirectVector,
			_Tri0.DirectVector, _Tri1.DirectVector, _Tri2.DirectVector,
			Dist);

		if (Dist != 0.f)
		{
			return true;
		}
	}

	{
		float4 StartRayPos = _RayStart + _RayDir.GetNormalize();
		float4 RayDir = -_RayDir;
		float Dist = 0.f;
		DirectX::TriangleTests::Intersects(StartRayPos.DirectVector, RayDir.DirectVector,
			_Tri0.DirectVector, _Tri1.DirectVector, _Tri2.DirectVector,
			Dist);

		if (Dist != 0.f)
		{
			return true;
		}
	}

	{
		float4 StartRayPos = _RayStart - _RayDir.GetNormalize();
		float4 RayDir = _RayDir;
		float Dist = 0.f;
		DirectX::TriangleTests::Intersects(StartRayPos.DirectVector, RayDir.DirectVector,
			_Tri0.DirectVector, _Tri1.DirectVector, _Tri2.DirectVector,
			Dist);

		if (Dist != 0.f)
		{
			return true;
		}
	}

	return false;
}

void GJHGameEngineCollision::TriToRayDis(float4 _Tri0, float4 _Tri1, float4 _Tri2, float4 _RayStart, float4 _RayEnd, float& _ResultY)
{
	float Dist = 0.0f;
	DirectX::TriangleTests::Intersects(_RayStart.DirectVector, _RayEnd.DirectVector,
		_Tri0.DirectVector, _Tri1.DirectVector, _Tri2.DirectVector,
		Dist);

	_ResultY = Dist;

	return;
}