#pragma once
#include <math.h>
#include <Windows.h>
#include <assert.h>
#include <DirectXPackedVector.h>

class GJHGameEngineMath
{
public:
	static double DPI;
	static float FPI;
	static float D2R;
	static float R2D;
};

class Color255
{
public:
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

public:
	Color255() : r(0), g(0), b(0), a(0)
	{

	}
};

class GJHGameEngineMatrix;
class GJHGameEngineVector
{
public:
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};

		struct
		{
			float r;
			float g;
			float b;
			float a;
		};

		DirectX::XMFLOAT3 DXXmFloat3;
		DirectX::XMFLOAT4 DXXmFloat4;

		DirectX::XMVECTOR DirectVector;
		float Arr[4];
	};

public:
	static GJHGameEngineVector QIden;
	static GJHGameEngineVector ZERO;
	static GJHGameEngineVector NONE;
	static GJHGameEngineVector ONE;

	static GJHGameEngineVector RED;
	static GJHGameEngineVector GREEN;
	static GJHGameEngineVector BLUE;
	static GJHGameEngineVector CYAN;
	static GJHGameEngineVector WHITE;
	static GJHGameEngineVector BLACK;

	static GJHGameEngineVector LEFT;
	static GJHGameEngineVector RIGHT;
	static GJHGameEngineVector UP;
	static GJHGameEngineVector DOWN;
	static GJHGameEngineVector FOWARD;
	static GJHGameEngineVector BACK;

public:
	bool IsZeroUnderX() const
	{
		return x <= 0.f;
	}

	bool IsZeroUnderY() const
	{
		return y <= 0.f;
	}

	float hx() const
	{
		return x * 0.5f;
	}

	float hy() const
	{
		return y * 0.5f;
	}
	
	float hz() const
	{
		return z * 0.5f;
	}

	int ix() const
	{
		return static_cast<int>(x);
	}

	int iy() const
	{
		return static_cast<int>(y);
	}

	int uix() const
	{
		return static_cast<unsigned int>(x);
	}

	int uiy() const
	{
		return static_cast<unsigned int>(y);
	}

	static float CalLen(const GJHGameEngineVector& _Value)
	{
		GJHGameEngineVector Len = DirectX::XMVector3Length(_Value);

		return Len.x;
	}

	static GJHGameEngineVector CalNormalize(const GJHGameEngineVector& _Value)
	{
		GJHGameEngineVector NormalVector;

		NormalVector.DirectVector = DirectX::XMVector3Normalize(_Value);

		return NormalVector;
	}

	static float CalDot(const GJHGameEngineVector& _Left, const GJHGameEngineVector& _Right)
	{
		GJHGameEngineVector Result = DirectX::XMVector3Dot(_Left.DirectVector, _Right.DirectVector);

		return Result.x;
	}

	void Normalize()
	{
		*this = CalNormalize(*this);
	}

	GJHGameEngineVector GetNormalize()
	{
		return CalNormalize(*this);
	}

	static float CalVectorAngleDeg(const GJHGameEngineVector& _Left, const GJHGameEngineVector& _Right)
	{
		float Angle = CalVectorAngleRad(_Left, _Right) * GJHGameEngineMath::R2D;
		return Angle;
	}

	// 가장 무조건 y축을 기준으로한 벡터 2개를 넣어줬을거다.
	static float CalVectorAngleDegY360(const GJHGameEngineVector& _Left, const GJHGameEngineVector& _Right)
	{
		float Angle = CalVectorAngleRad(_Left, _Right) * GJHGameEngineMath::R2D;

		GJHGameEngineVector CalVector = CalCross3D(_Left, _Right);

		if (0 == CalVector.y)
		{
			// 180.0도 이거나
			// 0도
			if (_Left == _Right)
			{
				return 0.0f;
			}
			else {
				return 180.0f;
			}
		}
		else if (0 > CalVector.y)
		{
			return Angle;
		}

		return -Angle;
	}


	static float CalVectorAngleRad(const GJHGameEngineVector& _Left, const GJHGameEngineVector& _Right)
	{
		GJHGameEngineVector LeftVector = CalNormalize(_Left);
		GJHGameEngineVector RightVector = CalNormalize(_Right);

		float CosS = CalDot(LeftVector, RightVector);

		return acosf(CosS);
	}

	static float CalVectorAngle360(int _Dir, GJHGameEngineVector& _Left, GJHGameEngineVector& _Right)
	{
		GJHGameEngineVector DirVec = _Left - _Right;
		float Angle = CalVectorAngleDeg(GJHGameEngineVector((float)_Dir, 0.f), DirVec);

		if (DirVec.y < 0.f)
		{
			Angle = 360.f - Angle;
		}

		return Angle;
	}

	static GJHGameEngineVector Lerp(const GJHGameEngineVector& _Left, const GJHGameEngineVector& _Right, float _Ratio)
	{
		if (1.0f <= _Ratio)
		{
			_Ratio = 1.0f;
		}

		return DirectX::XMVectorLerp(_Left.DirectVector, _Right.DirectVector, _Ratio);
	}

	static float Lerp(const float _Left, const float _Right, float _Ratio)
	{
		if (1.0f <= _Ratio)
		{
			_Ratio = 1.0f;
		}

		return _Left * (1 - _Ratio) + _Right * _Ratio;
	}

	static GJHGameEngineVector SLerp(const GJHGameEngineVector& _Left, const GJHGameEngineVector& _Right, float _Ratio)
	{
		if (1.0f <= _Ratio)
		{
			_Ratio = 1.0f;
		}

		return DirectX::XMQuaternionSlerp(_Left.DirectVector, _Right.DirectVector, _Ratio);
	}

	static GJHGameEngineVector CalCross3D(const GJHGameEngineVector& _Left, const GJHGameEngineVector& _Right)
	{
		GJHGameEngineVector Result;

		Result.DirectVector = DirectX::XMVector3Cross(_Left, _Right);

		return Result;
	}

	static GJHGameEngineVector DegToQuaternion(GJHGameEngineVector _RotValue)
	{
		return RadToQuaternion(_RotValue * GJHGameEngineMath::D2R);
	}

	static GJHGameEngineVector RadToQuaternion(GJHGameEngineVector _RotValue)
	{
		return DirectX::XMQuaternionRotationRollPitchYaw(_RotValue.x, _RotValue.y, _RotValue.z);
	}

	GJHGameEngineVector DegToQuaternion()
	{
		return RadToQuaternion(*this * GJHGameEngineMath::D2R);
	}

	GJHGameEngineVector ReturnRotationDeg3D(GJHGameEngineVector _Rot)
	{
		GJHGameEngineVector ReturnVector = DirectVector;

		ReturnVector.RotationRad3D(_Rot * GJHGameEngineMath::D2R);
		ReturnVector.w = 1.f;
		return ReturnVector;
	}

	void RotationDeg3D(GJHGameEngineVector _Rot)
	{
		RotationRad3D(_Rot * GJHGameEngineMath::D2R);
	}

	void RotationRad3D(GJHGameEngineVector _Rot)
	{
		DirectVector = DirectX::XMVector3Rotate(DirectVector, RadToQuaternion(_Rot));
	}

	static GJHGameEngineVector GlobalRotationZDeg(const GJHGameEngineVector& _Vector, float _Angle)
	{
		GJHGameEngineVector Rot = _Vector;
		Rot.RotationZDeg(_Angle);

		return Rot;
	}

	void RotationZDeg(float _Angle)
	{
		return RotationZRad(_Angle * GJHGameEngineMath::D2R);
	}

	void RotationZRad(float _Angle)
	{
		GJHGameEngineVector Copy = *this;

		x = (Copy.x * cosf(_Angle)) - (Copy.y * sinf(_Angle));
		y = (Copy.x * sinf(_Angle)) + (Copy.y * cosf(_Angle));
	}

	static GJHGameEngineVector GlobalRotationXDeg(const GJHGameEngineVector& _Vector, float _Angle)
	{
		GJHGameEngineVector Rot = _Vector;
		Rot.RotationXDeg(_Angle);

		return Rot;
	}

	void RotationXDeg(float _Angle)
	{
		return RotationXRad(_Angle * GJHGameEngineMath::D2R);
	}

	void RotationXRad(float _Angle)
	{
		GJHGameEngineVector Copy = *this;

		z = (Copy.z * cosf(_Angle)) - (Copy.y * sinf(_Angle));
		y = (Copy.z * sinf(_Angle)) + (Copy.y * cosf(_Angle));
	}

	static GJHGameEngineVector GlobalRotationYDeg(const GJHGameEngineVector& _Vector, float _Angle)
	{
		GJHGameEngineVector Rot = _Vector;
		Rot.RotationYDeg(_Angle);

		return Rot;
	}

	void RotationYDeg(float _Angle)
	{
		return RotationYRad(_Angle * GJHGameEngineMath::D2R);
	}

	void RotationYRad(float _Angle)
	{
		GJHGameEngineVector Copy = *this;
		z = (Copy.z * cosf(_Angle)) - (Copy.x * sinf(_Angle));
		x = (Copy.z * sinf(_Angle)) + (Copy.x * cosf(_Angle));
	}

	POINT ToPoint()
	{
		return POINT{ ix(), iy() };
	}

	GJHGameEngineVector HalfVector2D()
	{
		return GJHGameEngineVector(hx(), hy());
	}

	GJHGameEngineVector HalfVector3D()
	{
		return GJHGameEngineVector(hx(), hy(), hz());
	}

	static GJHGameEngineVector GlobalDirToDeg(float _Angle)
	{
		GJHGameEngineVector Rot = GJHGameEngineVector();
		Rot.DirToRadZ(_Angle * GJHGameEngineMath::D2R);
		return Rot;
	}

	GJHGameEngineVector& DirToDegZ(float _Angle)
	{
		return DirToRadZ(_Angle * GJHGameEngineMath::D2R);
	}

	GJHGameEngineVector& DirToRadZ(float _Angle)
	{
		x = cosf(_Angle);
		y = sinf(_Angle);

		return *this;
	}

public:
	operator DirectX::XMVECTOR& ()
	{
		return DirectVector;
	}

	operator DirectX::XMVECTOR() const
	{
		return DirectVector;
	}

	GJHGameEngineVector& operator=(const GJHGameEngineVector& _Other)
	{
		x = _Other.x;
		y = _Other.y;
		z = _Other.z;
		w = _Other.w;

		return *this;
	}

	GJHGameEngineVector operator+(const GJHGameEngineVector& _Other) const
	{
		GJHGameEngineVector ReturnVector;
		ReturnVector.x = x + _Other.x;
		ReturnVector.y = y + _Other.y;
		ReturnVector.z = z + _Other.z;

		return ReturnVector;
	}

	GJHGameEngineVector& operator +=(const GJHGameEngineVector& _Other)
	{
		x += _Other.x;
		y += _Other.y;
		z += _Other.z;

		return *this;
	}

	bool operator ==(const GJHGameEngineVector _Other) const
	{
		return x == _Other.x && y == _Other.y && z == _Other.z;
	}

	GJHGameEngineVector operator-() const
	{
		GJHGameEngineVector ReturnVector;

		ReturnVector.x = -x;
		ReturnVector.y = -y;
		ReturnVector.z = -z;

		return ReturnVector;
	}

	GJHGameEngineVector operator-(const GJHGameEngineVector& _Other) const
	{
		GJHGameEngineVector ReturnVector;

		ReturnVector.x = x - _Other.x;
		ReturnVector.y = y - _Other.y;
		ReturnVector.z = z - _Other.z;

		return ReturnVector;
	}

	GJHGameEngineVector& operator-=(const GJHGameEngineVector& _Other)
	{
		x -= _Other.x;
		y -= _Other.y;
		z -= _Other.z;

		return *this;
	}

	GJHGameEngineVector& operator *=(const GJHGameEngineVector& _Other)
	{
		x *= _Other.x;
		y *= _Other.y;
		z *= _Other.z;

		return *this;
	}

	GJHGameEngineVector operator *(const GJHGameEngineVector& _Other) const
	{
		GJHGameEngineVector Result;

		Result.x = x * _Other.x;
		Result.y = y * _Other.y;
		Result.z = z * _Other.z;

		return Result;
	}

	GJHGameEngineVector operator *(const float _Other) const
	{
		GJHGameEngineVector Result;

		Result.x = x * _Other;
		Result.y = y * _Other;
		Result.z = z * _Other;

		return Result;
	}

	GJHGameEngineVector operator/(const GJHGameEngineVector& _Other) const
	{
		GJHGameEngineVector Result;

		Result.x = x / _Other.x;
		Result.y = y / _Other.y;
		Result.z = z / _Other.z;

		return Result;
	}

	GJHGameEngineVector& operator/=(const float _Other)
	{
		x /= _Other;
		y /= _Other;
		z /= _Other;

		return *this;
	}

	GJHGameEngineVector operator /(float _Other) const
	{
		GJHGameEngineVector Result;

		Result.x = x / _Other;
		Result.y = y / _Other;
		Result.z = z / _Other;
		return Result;
	}

	unsigned int ColorToUint()
	{
		unsigned int Color = 0;

		unsigned char* PtrColor = reinterpret_cast<unsigned char*>(&Color);
		PtrColor[0] = static_cast<unsigned char>(255 * x);
		PtrColor[1] = static_cast<unsigned char>(255 * y);
		PtrColor[2] = static_cast<unsigned char>(255 * z);
		PtrColor[3] = static_cast<unsigned char>(255 * w);

		return Color;
	}

public:
	bool IsNearlyZero() const
	{
		return fabsf(x) <= 1.e-4f && fabsf(y) <= 1.e-4f && fabsf(z) <= 1.e-4f;
	}

	static float InvSqrt(float f)
	{
		const __m128 fOneHalf = _mm_set_ss(0.5f);
		__m128 Y0, X0, X1, X2, FOver2;
		float temp;

		Y0 = _mm_set_ss(f);
		X0 = _mm_rsqrt_ss(Y0);
		FOver2 = _mm_mul_ss(Y0, fOneHalf);

		X1 = _mm_mul_ss(X0, X0);
		X1 = _mm_sub_ss(fOneHalf, _mm_mul_ss(FOver2, X1));
		X1 = _mm_add_ss(X0, _mm_mul_ss(X0, X1));

		X2 = _mm_mul_ss(X1, X1);
		X2 = _mm_sub_ss(fOneHalf, _mm_mul_ss(FOver2, X2));
		X2 = _mm_add_ss(X1, _mm_mul_ss(X1, X2));

		_mm_store_ss(&temp, X2);
		return temp;
	}

	static GJHGameEngineVector MatrixToQuaternion(const GJHGameEngineMatrix& M);

	GJHGameEngineVector& operator *=(const float _Other)
	{
		x *= _Other;
		y *= _Other;
		z *= _Other;

		return* this;
	}

	bool operator!=(const GJHGameEngineVector& _Other)
	{
		return x != _Other.x || y != _Other.y || z != _Other.z || w != _Other.w;
	}

	void ReverseY() 
	{
		y *= -1.f;
	}

	static GJHGameEngineVector ConvertColor(const GJHGameEngineVector& _Color)
	{
		GJHGameEngineVector ReturnColor;

		ReturnColor.r = static_cast<float>(_Color.r) / 255.f;
		ReturnColor.g = static_cast<float>(_Color.g) / 255.f;
		ReturnColor.b = static_cast<float>(_Color.b) / 255.f;
		ReturnColor.a = static_cast<float>(_Color.a) / 255.f;

		return ReturnColor;
	}

public:
	GJHGameEngineVector() :
		x(0.f), y(0.f), z(0.f), w(1.f)
	{

	}

	GJHGameEngineVector(float* _x) :
		x(_x[0]), y(_x[1]), z(_x[2]), w(1.f)
	{

	}

	GJHGameEngineVector(float _x, float _y) :
		x(_x), y(_y), z(0.f), w(1.f)
	{

	}

	GJHGameEngineVector(float _x, float _y, float _z) :
		x(_x), y(_y), z(_z), w(1.f)
	{

	}

	GJHGameEngineVector(float _x, float _y, float _z, float _w) :
		x(_x), y(_y), z(_z), w(_w)
	{

	}

	GJHGameEngineVector(const GJHGameEngineVector& _Other) :
		x(_Other.x), y(_Other.y), z(_Other.z), w(_Other.w)
	{

	}

	GJHGameEngineVector(const DirectX::XMVECTOR& _DirectVector) :
		DirectVector(_DirectVector)
	{

	}
};

typedef GJHGameEngineVector float4;

class GJHGameEngineMatrix
{
public:
	union
	{
		float Arr2D[4][4];
		struct
		{
			GJHGameEngineVector ArrVector[4];
		};

		DirectX::XMMATRIX DxMat;
		float Arr1D[16];
	};

public:
	static GJHGameEngineMatrix Affine(GJHGameEngineVector _Scale, GJHGameEngineVector _Rot, GJHGameEngineVector _Pos)
	{
		return DirectX::XMMatrixAffineTransformation(_Scale.DirectVector, float4::ZERO, _Rot.DirectVector, _Pos.DirectVector);
	}

	float4 ExtractScaling()
	{
		float4 ret = float4::ZERO;

		float Tolerance = 1.e-8f;

		const float SquareSum0 = (Arr2D[0][0] * Arr2D[0][0]) + (Arr2D[0][1] * Arr2D[0][1]) + (Arr2D[0][2] * Arr2D[0][2]);
		const float SquareSum1 = (Arr2D[1][0] * Arr2D[1][0]) + (Arr2D[1][1] * Arr2D[1][1]) + (Arr2D[1][2] * Arr2D[1][2]);
		const float SquareSum2 = (Arr2D[2][0] * Arr2D[2][0]) + (Arr2D[2][1] * Arr2D[2][1]) + (Arr2D[2][2] * Arr2D[2][2]);

		if (SquareSum0 > Tolerance)
		{
			float Scale0 = sqrtf(SquareSum0);
			ret.x = Scale0;
			float InvScale0 = 1.f / Scale0;
			Arr2D[0][0] *= InvScale0;
			Arr2D[0][1] *= InvScale0;
			Arr2D[0][2] *= InvScale0;
		}
		else
		{
			ret.x = 0;
		}

		if (SquareSum1 > Tolerance)
		{
			float Scale1 = sqrtf(SquareSum1);
			ret.y = Scale1;
			float InvScale1 = 1.f / Scale1;
			Arr2D[1][0] *= InvScale1;
			Arr2D[1][1] *= InvScale1;
			Arr2D[1][2] *= InvScale1;
		}
		else
		{
			ret.y = 0;
		}

		if (SquareSum2 > Tolerance)
		{
			float Scale2 = sqrtf(SquareSum2);
			ret.z = Scale2;
			float InvScale2 = 1.f / Scale2;
			Arr2D[2][0] *= InvScale2;
			Arr2D[2][1] *= InvScale2;
			Arr2D[2][2] *= InvScale2;
		}
		else
		{
			ret.z = 0;
		}

		return ret;
	}


	inline float Determinant() const
	{
		return	Arr2D[0][0] * (
			Arr2D[1][1] * (Arr2D[2][2] * Arr2D[3][3] - Arr2D[2][3] * Arr2D[3][2]) -
			Arr2D[2][1] * (Arr2D[1][2] * Arr2D[3][3] - Arr2D[1][3] * Arr2D[3][2]) +
			Arr2D[3][1] * (Arr2D[1][2] * Arr2D[2][3] - Arr2D[1][3] * Arr2D[2][2])
			) -
			Arr2D[1][0] * (
				Arr2D[0][1] * (Arr2D[2][2] * Arr2D[3][3] - Arr2D[2][3] * Arr2D[3][2]) -
				Arr2D[2][1] * (Arr2D[0][2] * Arr2D[3][3] - Arr2D[0][3] * Arr2D[3][2]) +
				Arr2D[3][1] * (Arr2D[0][2] * Arr2D[2][3] - Arr2D[0][3] * Arr2D[2][2])
				) +
			Arr2D[2][0] * (
				Arr2D[0][1] * (Arr2D[1][2] * Arr2D[3][3] - Arr2D[1][3] * Arr2D[3][2]) -
				Arr2D[1][1] * (Arr2D[0][2] * Arr2D[3][3] - Arr2D[0][3] * Arr2D[3][2]) +
				Arr2D[3][1] * (Arr2D[0][2] * Arr2D[1][3] - Arr2D[0][3] * Arr2D[1][2])
				) -
			Arr2D[3][0] * (
				Arr2D[0][1] * (Arr2D[1][2] * Arr2D[2][3] - Arr2D[1][3] * Arr2D[2][2]) -
				Arr2D[1][1] * (Arr2D[0][2] * Arr2D[2][3] - Arr2D[0][3] * Arr2D[2][2]) +
				Arr2D[2][1] * (Arr2D[0][2] * Arr2D[1][3] - Arr2D[0][3] * Arr2D[1][2])
				);
	}

	GJHGameEngineMatrix InverseReturn()
	{
		GJHGameEngineMatrix ReturnMat = DirectX::XMMatrixInverse(nullptr, DxMat);
		return ReturnMat;
	}

	static GJHGameEngineMatrix Transformation(float4 _Scale, float4 _RotQ, float4 _Pos)
	{
		return DirectX::XMMatrixTransformation(float4::ZERO, float4::ZERO, _Scale.DirectVector, float4::ZERO, _RotQ, _Pos);
	}

	void Transpose()
	{
		DxMat = DirectX::XMMatrixTranspose(DxMat);
	}

	void View(const GJHGameEngineVector& _Pos, const GJHGameEngineVector& _Foward, const GJHGameEngineVector& _Up)
	{
		Iden();

		DxMat = DirectX::XMMatrixLookToLH(_Pos, _Foward, _Up);
	}

	void ViewPort(float _StartX, float _StartY, float _W, float _H, float _MinZ, float _MaxZ)
	{
		Iden();

		Arr2D[0][0] = _W / 2;
		Arr2D[1][1] = -(_H / 2);
		Arr2D[2][2] = _MaxZ - _MinZ;

		Arr2D[3][0] = _StartX + Arr2D[0][0];
		Arr2D[3][1] = _StartY + -Arr2D[1][1];
		Arr2D[3][2] = _MinZ;
	}

	void OrthographicLH(float _W, float _H, float _Near, float _Far)
	{
		Iden();

		DxMat = DirectX::XMMatrixOrthographicLH(_W, _H, _Near, _Far);
	}

	void PerspectiveFovDegLH(float _Fov, float _W, float _H, float _Near, float _Far)
	{
		Iden();

		DxMat = DirectX::XMMatrixPerspectiveFovLH(_Fov * GJHGameEngineMath::D2R, _W / _H, _Near, _Far);
	}

	void Iden()
	{
		DxMat = DirectX::XMMatrixIdentity();
	}

	void Scale(GJHGameEngineVector _Scale)
	{
		Iden();

		DxMat = DirectX::XMMatrixScaling(_Scale.x, _Scale.y, _Scale.z);
	}

	void RotXDeg(float _Angle)
	{
		RotXRad(_Angle * GJHGameEngineMath::D2R);
	}

	void RotXRad(float _Angle)
	{
		Iden();

		DxMat = DirectX::XMMatrixRotationX(_Angle);
	}

	void RotYDef(float _Angle)
	{
		RotYRad(_Angle * GJHGameEngineMath::D2R);
	}

	void RotYRad(float _Angle)
	{
		Iden();

		DxMat = DirectX::XMMatrixRotationY(_Angle);
	}

	void RotZDeg(float _Angle)
	{
		RotZRad(_Angle * GJHGameEngineMath::D2R);
	}

	void RotZRad(float _Angle)
	{
		Iden();

		DxMat = DirectX::XMMatrixRotationZ(_Angle);
	}

	void RotationXYZDeg(const GJHGameEngineVector& _Angle)
	{
		RotationXYZRad(_Angle * GJHGameEngineMath::D2R);
	}

	void RotationXYZRad(const GJHGameEngineVector& _Angle)
	{
		Iden();
		
		DxMat = DirectX::XMMatrixRotationRollPitchYaw(_Angle.x, _Angle.y, _Angle.z);
	}

	void Position(GJHGameEngineVector _Pos)
	{
		Iden();

		Arr2D[3][0] = _Pos.x;
		Arr2D[3][1] = _Pos.y;
		Arr2D[3][2] = _Pos.z;
	}

public:
	GJHGameEngineMatrix& operator=(const GJHGameEngineMatrix& _Other)
	{
		DxMat = _Other.DxMat;

		return *this;
	}

	GJHGameEngineMatrix operator*(const GJHGameEngineMatrix& _Other)
	{
		GJHGameEngineMatrix Result = DirectX::XMMatrixMultiply(DxMat, _Other.DxMat);

		return Result;
	}

public:
	GJHGameEngineMatrix()
	{
		Iden();
	}

	GJHGameEngineMatrix(const GJHGameEngineMatrix& _Other) :
		DxMat(_Other.DxMat)
	{

	}

	GJHGameEngineMatrix(const DirectX::XMMATRIX& _Other) :
		DxMat(_Other)
	{

	}
};

GJHGameEngineVector operator*(const GJHGameEngineVector& _Left, const GJHGameEngineMatrix& _Right);

typedef GJHGameEngineMatrix float4x4;

enum class MatrixType
{
	Matrix_Position,
	Matrix_Scale,
	Matrix_Rotation,
	Matrix_Revolve,
	Matrix_Parent,
	Matrix_LocalWorld,
	Matrix_World,
	Matrix_View,
	Matrix_Projection,
	Matrix_WorldViow,
	Matrix_ViewProjection,
	Matrix_WorldViewProjection,
	Matrix_Max
};

class TransformData
{
public:
	union
	{
		struct
		{
			float4x4 Scale;
			float4x4 Rotation;
			float4x4 Position;
			float4x4 Revolve;
			float4x4 Parent;
			float4x4 LocalWorld;
			float4x4 World;
		};
	};

public:
	void CalWorld()
	{
		LocalWorld = Scale * Rotation * Position * Revolve;
		World = LocalWorld * Parent;
	}

	void Iden()
	{
		Position.Iden();
		Scale.Iden();
		Rotation.Iden();
		Revolve.Iden();
		Parent.Iden();
		LocalWorld.Iden();
		World.Iden();
	}

	TransformData()
	{
		Iden();
	}

	~TransformData()
	{
		Iden();
	}
};