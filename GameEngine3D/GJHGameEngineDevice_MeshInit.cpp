#include "GJHGameEngineDevice.h"
#include <vector>
#include "GJHDirectResourcesManager.h"
#include "GJHDirectVertexBuffer.h"
#include "GJHDirectIndexBuffer.h"
#include "GJHDirectMesh.h"
#include "GJHVertexHeader.h"
#include "GJHDirectConstantBuffer.h"
#include <GJHGameEngineMath.h>
#include <GJHGameEngineRandom.h>

void GJHGameEngineDevice::MeshInit()
{
    {
        VertexBase V;
        std::vector<VertexBase> VBVector;
        std::vector<DWORD> IBVector;

        float Radius = 0.5f;

        V.Pos = float4(0.0f, Radius, 0.0f, 1.0f);
        V.Uv = float4(0.5f, 0.0f);
        V.Normal = float4(0.0f, Radius, 0.0f, 1.0f);
        V.Normal = V.Normal.GetNormalize();
        V.Normal.w = 0.f;
        V.Tangent = float4(1.0f, 0.0f, 0.0f, 0.0f);
        V.BiNormal = float4(0.0f, 0.0f, 1.0f, 0.0f);

        VBVector.push_back(V);

        UINT iStackCount = 40;
        UINT iSliceCount = 40;

        float yRotAngle = GJHGameEngineMath::FPI / (float)iStackCount;
        float zRotAngle = (GJHGameEngineMath::FPI * 2) / (float)iSliceCount;

        float yUvRatio = 1.0f / (float)iStackCount;
        float zUvRatio = 1.0f / (float)iStackCount;

        for (UINT y = 1; y < iStackCount; ++y)
        {
            float phi = y * yRotAngle;
            for (UINT z = 0; z < iSliceCount + 1; ++z)
            {
                float theta = z * zRotAngle;
                V.Pos = float4{
                    Radius * sinf(y * yRotAngle) * cosf(z * zRotAngle),
                    Radius * cosf(y * yRotAngle),
                    Radius * sinf(y * yRotAngle) * sinf(z * zRotAngle),
                    1.0f
                };

                V.Uv = float4(yUvRatio * z, zUvRatio * y);
                V.Normal = V.Pos.GetNormalize();
                V.Normal.w = 0.f;

                V.Tangent.x = -Radius * sinf(phi) * sinf(theta);
                V.Tangent.y = 0.0f;
                V.Tangent.z = Radius * sinf(phi) * cosf(theta);
                V.Tangent = V.Tangent.GetNormalize();
                V.Tangent.w = 0.0f;

                V.BiNormal = float4::CalCross3D(V.Tangent, V.Normal);
                V.BiNormal = V.BiNormal.GetNormalize();
                V.BiNormal.w = 0.0f;

                VBVector.push_back(V);
            }
        }

        V.Pos = float4(0.0f, -Radius, 0.0f, 1.0f);
        V.Uv = float4(0.5f, 1.0f);
        V.Normal = float4(0.0f, -Radius, 0.0f, 1.0f);
        V.Normal = V.Normal.GetNormalize();
        V.Normal.w = 0.f;
        V.Tangent = float4(-1.0f, 0.0f, 0.0f, 0.0f);
        V.BiNormal = float4(0.0f, 0.0f, -1.0f, 0.0f);

        VBVector.push_back(V);

        IBVector.clear();

        for (UINT i = 0; i < iSliceCount; i++)
        {
            IBVector.push_back(0);
            IBVector.push_back(i + 2);
            IBVector.push_back(i + 1);
        }

        for (UINT y = 0; y < iStackCount - 2; y++)
        {
            for (UINT z = 0; z < iSliceCount; z++)
            {
                IBVector.push_back((iSliceCount + 1) * y + z + 1);
                IBVector.push_back((iSliceCount + 1) * (y + 1) + (z + 1) + 1);
                IBVector.push_back((iSliceCount + 1) * (y + 1) + z + 1);

                IBVector.push_back((iSliceCount + 1) * y + z + 1);
                IBVector.push_back((iSliceCount + 1) * y + (z + 1) + 1);
                IBVector.push_back((iSliceCount + 1) * (y + 1) + (z + 1) + 1);
            }
        }

        UINT iBotIndex = (UINT)VBVector.size() - 1;
        for (UINT i = 0; i < iSliceCount; i++)
        {
            IBVector.push_back(iBotIndex);
            IBVector.push_back(iBotIndex - (i + 2));
            IBVector.push_back(iBotIndex - (i + 1));
        }

        GJHDirectVertexBuffer::Create(L"Sphere", VBVector);
        GJHDirectIndexBuffer::Create(L"Sphere", IBVector);
        GJHDirectMesh::Create(L"Sphere");
    }

    {
        std::vector<VertexBase> VBVector;
        VBVector.resize(4 * 6);

        VBVector[0] = VertexBase{ {0.5f, 0.5f, -0.5f },  {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[1] = VertexBase{ {-0.5f, 0.5f, -0.5f }, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[2] = VertexBase{ {-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[3] = VertexBase{ {0.5f, -0.5f, -0.5f }, {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };

        VBVector[4] = VertexBase{ VBVector[0].Pos.ReturnRotationDeg3D({ 180.0f, 0.0f, 0.0f}),  {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[5] = VertexBase{ VBVector[1].Pos.ReturnRotationDeg3D({ 180.0f, 0.0f, 0.0f}),  {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[6] = VertexBase{ VBVector[2].Pos.ReturnRotationDeg3D({ 180.0f, 0.0f, 0.0f}),  {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[7] = VertexBase{ VBVector[3].Pos.ReturnRotationDeg3D({ 180.0f, 0.0f, 0.0f}),  {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };

        VBVector[8] = VertexBase{ VBVector[0].Pos.ReturnRotationDeg3D({ 0.0f, 90.0f, 0.0f }),  {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[9] = VertexBase{ VBVector[1].Pos.ReturnRotationDeg3D({ 0.0f, 90.0f, 0.0f }),  {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[10] = VertexBase{ VBVector[2].Pos.ReturnRotationDeg3D({ 0.0f, 90.0f, 0.0f }),  {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[11] = VertexBase{ VBVector[3].Pos.ReturnRotationDeg3D({ 0.0f, 90.0f, 0.0f }),  {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };

        VBVector[12] = VertexBase{ VBVector[0].Pos.ReturnRotationDeg3D({ 0.0f, -90.0f, 0.0f }),  {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[13] = VertexBase{ VBVector[1].Pos.ReturnRotationDeg3D({ 0.0f, -90.0f, 0.0f }),  {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[14] = VertexBase{ VBVector[2].Pos.ReturnRotationDeg3D({ 0.0f, -90.0f, 0.0f }),  {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[15] = VertexBase{ VBVector[3].Pos.ReturnRotationDeg3D({ 0.0f, -90.0f, 0.0f }),  {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };

        VBVector[16] = VertexBase{ VBVector[0].Pos.ReturnRotationDeg3D({ 90.0f, 0.f, 0.0f }),  {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[17] = VertexBase{ VBVector[1].Pos.ReturnRotationDeg3D({ 90.0f, 0.f, 0.0f }),  {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[18] = VertexBase{ VBVector[2].Pos.ReturnRotationDeg3D({ 90.0f, 0.f, 0.0f }),  {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[19] = VertexBase{ VBVector[3].Pos.ReturnRotationDeg3D({ 90.0f, 0.f, 0.0f }),  {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };

        VBVector[20] = VertexBase{ VBVector[0].Pos.ReturnRotationDeg3D({ -90.0f, 0.f, 0.0f }),  {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[21] = VertexBase{ VBVector[1].Pos.ReturnRotationDeg3D({ -90.0f, 0.f, 0.0f }),  {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[22] = VertexBase{ VBVector[2].Pos.ReturnRotationDeg3D({ -90.0f, 0.f, 0.0f }),  {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[23] = VertexBase{ VBVector[3].Pos.ReturnRotationDeg3D({ -90.0f, 0.f, 0.0f }),  {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };

        GJHDirectVertexBuffer::Create(L"BoxTest", VBVector);

        for (UINT i = 0; i < 6; i++)
        {
            std::vector<unsigned int> IBVector;

            int N = i * 4;
            IBVector.push_back(N + 1);
            IBVector.push_back(N);
            IBVector.push_back(N + 3);
            IBVector.push_back(N + 1);
            IBVector.push_back(N + 3);
            IBVector.push_back(N + 2);

            GJHGameEngineString Name;
            Name.Appendf(L"BoxTest%d", i);

            GJHDirectIndexBuffer::Create(Name, IBVector);
            GJHDirectMesh::Create(Name, L"BoxTest", Name);
        }
    }

    {
        std::vector<VertexBase> VBVector;
        VBVector.resize(4 * 6);

        VBVector[0] = VertexBase{ {0.5f, 0.5f, -0.5f },  {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[1] = VertexBase{ {-0.5f, 0.5f, -0.5f }, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[2] = VertexBase{ {-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[3] = VertexBase{ {0.5f, -0.5f, -0.5f }, {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };

        VBVector[4] = VertexBase{ VBVector[0].Pos.ReturnRotationDeg3D({ 180.0f, 0.0f, 0.0f}),  {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[5] = VertexBase{ VBVector[1].Pos.ReturnRotationDeg3D({ 180.0f, 0.0f, 0.0f}),  {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[6] = VertexBase{ VBVector[2].Pos.ReturnRotationDeg3D({ 180.0f, 0.0f, 0.0f}),  {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[7] = VertexBase{ VBVector[3].Pos.ReturnRotationDeg3D({ 180.0f, 0.0f, 0.0f}),  {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };

        VBVector[8] = VertexBase{ VBVector[0].Pos.ReturnRotationDeg3D({ 0.0f, 90.0f, 0.0f }),  {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[9] = VertexBase{ VBVector[1].Pos.ReturnRotationDeg3D({ 0.0f, 90.0f, 0.0f }),  {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[10] = VertexBase{ VBVector[2].Pos.ReturnRotationDeg3D({ 0.0f, 90.0f, 0.0f }),  {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[11] = VertexBase{ VBVector[3].Pos.ReturnRotationDeg3D({ 0.0f, 90.0f, 0.0f }),  {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };

        VBVector[12] = VertexBase{ VBVector[0].Pos.ReturnRotationDeg3D({ 0.0f, -90.0f, 0.0f }),  {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[13] = VertexBase{ VBVector[1].Pos.ReturnRotationDeg3D({ 0.0f, -90.0f, 0.0f }),  {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[14] = VertexBase{ VBVector[2].Pos.ReturnRotationDeg3D({ 0.0f, -90.0f, 0.0f }),  {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[15] = VertexBase{ VBVector[3].Pos.ReturnRotationDeg3D({ 0.0f, -90.0f, 0.0f }),  {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };

        VBVector[16] = VertexBase{ VBVector[0].Pos.ReturnRotationDeg3D({ 90.0f, 0.f, 0.0f }),  {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[17] = VertexBase{ VBVector[1].Pos.ReturnRotationDeg3D({ 90.0f, 0.f, 0.0f }),  {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[18] = VertexBase{ VBVector[2].Pos.ReturnRotationDeg3D({ 90.0f, 0.f, 0.0f }),  {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[19] = VertexBase{ VBVector[3].Pos.ReturnRotationDeg3D({ 90.0f, 0.f, 0.0f }),  {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };

        VBVector[20] = VertexBase{ VBVector[0].Pos.ReturnRotationDeg3D({ -90.0f, 0.f, 0.0f }),  {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[21] = VertexBase{ VBVector[1].Pos.ReturnRotationDeg3D({ -90.0f, 0.f, 0.0f }),  {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[22] = VertexBase{ VBVector[2].Pos.ReturnRotationDeg3D({ -90.0f, 0.f, 0.0f }),  {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[23] = VertexBase{ VBVector[3].Pos.ReturnRotationDeg3D({ -90.0f, 0.f, 0.0f }),  {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };

        std::vector<unsigned int> IBVector = { 1, 0, 3, 1, 3, 2 };

        for (UINT i = 1; i < 6; i++)
        {
            int N = i * 4;
            IBVector.push_back(N + 1);
            IBVector.push_back(N);
            IBVector.push_back(N + 3);
            IBVector.push_back(N + 1);
            IBVector.push_back(N + 3);
            IBVector.push_back(N + 2);
        }

        GJHDirectVertexBuffer::Create(L"Box", VBVector);
        GJHDirectIndexBuffer::Create(L"Box", IBVector);
        GJHDirectMesh::Create(L"Box");
    }

	{
		std::vector<VertexBase> VBVector;
		VBVector.resize(5);
		VBVector[0] = VertexBase{ { -0.5f, 0.5f, 0.f }, { 1.f, 0.f }, { 0.f, 0.f, 0.f } };
		VBVector[1] = VertexBase{ { 0.5f, 0.5f, 0.f }, { 0.f, 0.f }, { 0.f, 0.f, 0.f } };
		VBVector[2] = VertexBase{ { 0.5f, -0.5f, 0.f }, { 0.f, 1.f }, { 0.f, 0.f, 0.f } };
		VBVector[3] = VertexBase{ { -0.5f, -0.5f, 0.f }, { 1.f, 1.f }, { 0.f, 0.f, 0.f } };
		VBVector[4] = VertexBase{ { -0.5f, 0.5f, 0.f }, { 1.f, 0.f }, { 0.f, 0.f, 0.f } };
		GJHDirectVertexBuffer::Create(L"RectDebug", VBVector);
		std::vector<unsigned int> IBVector = { 0, 1, 2, 3, 4, 5 };
		GJHDirectIndexBuffer::Create(L"RectDebug", IBVector);
		GJHDirectMesh::Create(L"RectDebug");
	}
	
	{
		std::vector<VertexBase> VBVector;
		VBVector.resize(4);
		VBVector[0] = VertexBase{ { 0.5f, 0.5f, 0.f }, { 1.f, 0.f }, { 0.f, 0.f, 0.f } };
		VBVector[1] = VertexBase{ { -0.5f, 0.5f, 0.f }, { 0.f, 0.f }, { 0.f, 0.f, 0.f } };
		VBVector[2] = VertexBase{ { -0.5f, -0.5f, 0.f }, { 0.f, 1.f }, { 0.f, 0.f, 0.f } };
		VBVector[3] = VertexBase{ { 0.5f, -0.5f, 0.f }, { 1.f, 1.f }, { 0.f, 0.f, 0.f } };
		GJHDirectVertexBuffer::Create(L"Rect", VBVector);

		std::vector<unsigned int> IBVector = { 1, 0, 3, 1, 3, 2 };
		GJHDirectIndexBuffer::Create(L"Rect", IBVector);
		GJHDirectMesh::Create(L"Rect");
	}

    {
        std::vector<VertexBase> VBVector;
        VBVector.resize(4);
        VBVector[0] = VertexBase{ {0.5f, 0.5f, 0.0f }, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[1] = VertexBase{ {-0.5f, 0.5f, 0.0f }, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[2] = VertexBase{ {-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };
        VBVector[3] = VertexBase{ {0.5f, -0.5f, 0.0f }, {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f} };

        for (size_t i = 0; i < 4; i++)
        {
            VBVector[i].Pos.RotationDeg3D({ 90.0f, 0.0f, 0.0f });
            VBVector[i].Pos.y = 0.0f;
            VBVector[i].Pos.w = 1.0f;
        }

        GJHDirectVertexBuffer::Create(L"GridRect", VBVector);
        std::vector<unsigned int> IBVector = { 1, 0, 3, 1, 3, 2 };
        GJHDirectIndexBuffer::Create(L"GridRect", IBVector);
        GJHDirectMesh::Create(L"GridRect");
    }

	{
		std::vector<VertexBase> VBVector;
		VBVector.resize(4);
		VBVector[0] = VertexBase{ { 1.f, 0.f, 0.f }, { 1.f, 0.f }, { 0.f, 0.f, 0.f } };
		VBVector[1] = VertexBase{ { 0.f, 0.f, 0.f }, { 0.f, 0.f }, { 0.f, 0.f, 0.f } };
		VBVector[2] = VertexBase{ { 0.f, -1.f, 0.f }, { 0.f, 1.f }, { 0.f, 0.f, 0.f } };
		VBVector[3] = VertexBase{ { 1.f, -1.f, 0.f }, { 1.f, 1.f }, { 0.f, 0.f, 0.f } };
		GJHDirectVertexBuffer::Create(L"Ray", VBVector);

		std::vector<unsigned int> IBVector = { 1, 0, 3, 1, 3, 2 };
		GJHDirectIndexBuffer::Create(L"Ray", IBVector);
		GJHDirectMesh::Create(L"Ray");
	}

	{
		std::vector<VertexBase> VBVector;
		VBVector.resize(4);
		VBVector[0] = VertexBase{ { 1.f, 0.5f, 0.f }, { 1.f, 0.f }, { 0.f, 0.f, 0.f } };
		VBVector[1] = VertexBase{ { 0.f, 0.5f, 0.f }, { 0.f, 0.f }, { 0.f, 0.f, 0.f } };
		VBVector[2] = VertexBase{ { 0.f, -0.5f, 0.f }, { 0.f, 1.f }, { 0.f, 0.f, 0.f } };
		VBVector[3] = VertexBase{ { 1.f, -0.5f, 0.f }, { 1.f, 1.f }, { 0.f, 0.f, 0.f } };
		GJHDirectVertexBuffer::Create(L"RayCenter", VBVector);

		std::vector<unsigned int> IBVector = { 1, 0, 3, 1, 3, 2 };
		GJHDirectIndexBuffer::Create(L"RayCenter", IBVector);
		GJHDirectMesh::Create(L"RayCenter");
	}

	{
		std::vector<ParticleVTX> VBVector;
		VBVector.resize(1);
		VBVector[0] = ParticleVTX{ {0.0f, 0.0f, 0.0f, 1.0f }, {1.0f, 0.0f}, 0 };
		GJHDirectVertexBuffer::Create(L"Point", VBVector);

		std::vector<unsigned int> IBVector = { 0 };
		GJHDirectIndexBuffer::Create(L"Point", IBVector);

		GJHDirectMesh::Create(L"Point");
	}

	{
		std::vector<VertexBase> VBVector;
		VBVector.resize(4);
		VBVector[0] = VertexBase{ { 1.f, 1.f, 0.f }, { 1.f, 0.f }, { 0.f, 0.f, 0.f } };
		VBVector[1] = VertexBase{ { -1.f, 1.f, 0.f }, { 0.f, 0.f }, { 0.f, 0.f, 0.f } };
		VBVector[2] = VertexBase{ { -1.f, -1.f, 0.f }, { 0.f, 1.f }, { 0.f, 0.f, 0.f } };
		VBVector[3] = VertexBase{ { 1.f, -1.f, 0.f }, { 1.f, 1.f }, { 0.f, 0.f, 0.f } };
		GJHDirectVertexBuffer::Create(L"FullRect", VBVector);

		std::vector<unsigned int> IBVector = { 1, 0, 3, 1, 3, 2 };
		GJHDirectIndexBuffer::Create(L"FullRect", IBVector);
		GJHDirectMesh::Create(L"FullRect");
	}
}