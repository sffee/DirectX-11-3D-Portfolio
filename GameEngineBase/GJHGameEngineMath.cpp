#include "BasePre.h"
#include "GJHGameEngineMath.h"

double GJHGameEngineMath::DPI = 3.141592653589793238462643383279502884197169399375105820974944;
float GJHGameEngineMath::FPI = 3.141592653589793238462643383279502884197169399375105820974944f;
float GJHGameEngineMath::D2R = FPI / 180.0f;
float GJHGameEngineMath::R2D = 180.0f / FPI;

GJHGameEngineVector GJHGameEngineVector::QIden = { 0.0f, 0.0f, 0.0f, 1.0f };
GJHGameEngineVector GJHGameEngineVector::ZERO = { 0.f, 0.f, 0.f, 1.f };
GJHGameEngineVector GJHGameEngineVector::NONE = { 0.f, 0.f, 0.f, 0.f };

GJHGameEngineVector GJHGameEngineVector::ONE = { 1.f, 1.f, 1.f, 1.f };

GJHGameEngineVector GJHGameEngineVector::RED = { 1.f, 0.f, 0.f, 1.f };
GJHGameEngineVector GJHGameEngineVector::GREEN = { 0.f, 1.f, 0.f, 1.f };
GJHGameEngineVector GJHGameEngineVector::BLUE = { 0.f, 0.f, 1.f, 1.f };
GJHGameEngineVector GJHGameEngineVector::CYAN = { 0.f, 1.f, 1.f, 1.f };
GJHGameEngineVector GJHGameEngineVector::WHITE = { 1.f, 1.f, 1.f, 1.f };
GJHGameEngineVector GJHGameEngineVector::BLACK = { 0.f, 0.f, 0.f, 1.f };

GJHGameEngineVector GJHGameEngineVector::LEFT = { -1.f, 0.f, 0.f, 1.f };
GJHGameEngineVector GJHGameEngineVector::RIGHT = { 1.f, 0.f, 0.f, 1.f };
GJHGameEngineVector GJHGameEngineVector::UP = { 0.f, 1.f, 0.f, 1.f };
GJHGameEngineVector GJHGameEngineVector::DOWN = { 0.f, -1.f, 0.f, 1.f };
GJHGameEngineVector GJHGameEngineVector::FOWARD = { 0.f, 0.f, 1.f, 1.f };
GJHGameEngineVector GJHGameEngineVector::BACK = { 0.f, 0.f, -1.f, 1.f };

GJHGameEngineVector operator*(const GJHGameEngineVector& _Left, const GJHGameEngineMatrix& _Right)
{
	GJHGameEngineVector Result;

	for (size_t i = 0; i < 4; i++)
	{
		Result.Arr[i] = _Left.Arr[0] * _Right.Arr2D[0][i] +
			_Left.Arr[1] * _Right.Arr2D[1][i] +
			_Left.Arr[2] * _Right.Arr2D[2][i] +
			_Left.Arr[3] * _Right.Arr2D[3][i];
	}

	return Result;
}

GJHGameEngineVector GJHGameEngineVector::MatrixToQuaternion(const GJHGameEngineMatrix& M)
{
    float4 Return;

    if (M.ArrVector[0].IsNearlyZero() || M.ArrVector[1].IsNearlyZero() || M.ArrVector[2].IsNearlyZero())
    {
        Return.x = 0.0f;
        Return.y = 0.0f;
        Return.z = 0.0f;
        Return.w = 1.0f;
        return Return;
    }

    float s;

    const float tr = M.Arr2D[0][0] + M.Arr2D[1][1] + M.Arr2D[2][2];

    if (tr > 0.0f)
    {
        float InvS = InvSqrt(tr + 1.f);
        Return.w = 0.5f * (1.f / InvS);
        s = 0.5f * InvS;

        Return.x = (M.Arr2D[1][2] - M.Arr2D[2][1]) * s;
        Return.y = (M.Arr2D[2][0] - M.Arr2D[0][2]) * s;
        Return.z = (M.Arr2D[0][1] - M.Arr2D[1][0]) * s;
    }
    else
    {
        int i = 0;

        if (M.Arr2D[1][1] > M.Arr2D[0][0])
            i = 1;

        if (M.Arr2D[2][2] > M.Arr2D[i][i])
            i = 2;

        static const int nxt[3] = { 1, 2, 0 };
        const int j = nxt[i];
        const int k = nxt[j];

        s = M.Arr2D[i][i] - M.Arr2D[j][j] - M.Arr2D[k][k] + 1.0f;

        float InvS = InvSqrt(s);

        float qt[4];
        qt[i] = 0.5f * (1.f / InvS);

        s = 0.5f * InvS;

        qt[3] = (M.Arr2D[j][k] - M.Arr2D[k][j]) * s;
        qt[j] = (M.Arr2D[i][j] + M.Arr2D[j][i]) * s;
        qt[k] = (M.Arr2D[i][k] + M.Arr2D[k][i]) * s;

        Return.x = qt[0];
        Return.y = qt[1];
        Return.z = qt[2];
        Return.w = qt[3];
    }

    return Return;
}