#pragma once
#include <GJHUpdaterBase.h>
#include <GJHGameEngineMemory.h>
#include <set>
#include <vector>
#include <functional>
#include <GJHGameEngineDebug.h>
#include "GJHGameEngineTransform.h"

class ColEventerBase
{
public:
    virtual void CalUpdate() = 0;
};

template<typename EventType>
class ColEventer : public ColEventerBase
{
public:
    std::vector<std::function<void(EventType*)>> m_EventList;

public:
    void CalUpdate() override
    {

    }
};

class GJHGameEngineTransform;
class GJHGameEngineCollision : public GJHUpdaterBase, public SharedPtrThis<GJHGameEngineCollision>
{
private:
    class StarterClass
    {
    public:
        StarterClass()
        {
            Init();
        }

        ~StarterClass() { }
    };

    static StarterClass Starter;

private:
    static bool(__stdcall* ColCheckFunc[(int)COLTYPE::NONE][(int)COLTYPE::NONE])(GJHGameEngineTransform&, GJHGameEngineTransform&);
    static void Init();

public:
    static bool OBBToOBB(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform);
    static bool OBBToAABB(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform);
    static bool OBBToSPHERE(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform);
    static bool OBBToPOINT(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform);

    static bool AABBToAABB(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform);
    static bool AABBToOBB(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform);
    static bool AABBToSPHERE(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform);
    static bool AABBToPOINT(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform);

    static bool SPHEREToSPHERE(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform);
    static bool SPHEREToOBB(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform);
    static bool SPHEREToAABB(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform);
    static bool SPHEREToPOINT(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform);

    static bool POINTToPOINT(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform);
    static bool POINTToOBB(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform);
    static bool POINTToAABB(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform);
    static bool POINTToSPHERE(GJHGameEngineTransform& _LeftTransform, GJHGameEngineTransform& _RightTransform);

    static void TriToRayDis(float4 _Tri0, float4 _Tri1, float4 _Tri2, float4 _RayStart, float4 _RayEnd, float& _ResultY);
    static bool TriToRayContaine(float4 _Tri0, float4 _Tri1, float4 _Tri2, float4 _RayStart, float4 _RayEnd);

private:
    int m_Order;
    COLTYPE Type;
    GJHGameEngineTransform& m_Transform;
    bool m_IsColCheck;

    std::function<void(GJHGameEngineCollision*)> m_ColTrueEvent;
    std::function<void(GJHGameEngineCollision*)> m_ColFalseEvent;

private:
    void* m_Data;

public:
    void SetData(void* _Data)
    {
        m_Data = _Data;
    }

    void SetColCheck(bool _Set)
    {
        m_IsColCheck = _Set;
    }

    bool GetColCheck()
    {
        return m_IsColCheck;
    }

    template<typename T>
    T* GetData()
    {
        return reinterpret_cast<T*>(m_Data);
    }

    GJHGameEngineTransform& GetTransform()
    {
        return m_Transform;
    }

public:
    template<typename EventClass>
    void ColTrueEvent(void(EventClass::* _Func)(GJHGameEngineCollision*), EventClass& _Obj)
    {
        m_ColTrueEvent = std::bind(_Func, &_Obj, std::placeholders::_1);
    }

    template<typename EventClass>
    void ColFalseEvent(void(EventClass::* _Func)(GJHGameEngineCollision*), EventClass& _Obj)
    {
        m_ColFalseEvent = std::bind(_Func, &_Obj, std::placeholders::_1);
    }

public:
    void SetType(COLTYPE _Type)
    {
        Type = _Type;
    }

    void SetOrder(int _Order)
    {
        m_Order = _Order;
    }

    int GetOrder()
    {
        return m_Order;
    }

public:
	GJHGameEngineCollision(GJHGameEngineTransform& _Transform);
	~GJHGameEngineCollision();

public:
	GJHGameEngineCollision(const GJHGameEngineCollision& _Other) = delete;
	GJHGameEngineCollision(const GJHGameEngineCollision&& _Other) = delete;

public:
	GJHGameEngineCollision operator=(const GJHGameEngineCollision& _Other) = delete;
	GJHGameEngineCollision operator=(const GJHGameEngineCollision&& _Other) = delete;

public:
    void ColUpdate();
    bool ColCheck(GJHGameEngineCollision& _Other);
    bool ColCheck(GJHGameEngineTransform& _Other, COLTYPE _Type);
};