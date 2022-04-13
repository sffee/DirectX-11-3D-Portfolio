#pragma once
#include <vector>
#include "GJHContentsComponent.h"
#include <GJHLightData.h>

class GJHDirectRenderTarget;
class GJHRenderManagerComponent;
class GJHLight : public GJHContentsComponent
{
private:
    friend GJHRenderManagerComponent;

private:
    static GJHLightData DefaultData;

private:
    GJHLightData Data;
    GJHCamera* m_CurCamera;

public:
    int m_Order;
    bool m_ChangeOrder;
    std::vector<int> m_ViewList;

public:
    GJHLightData& GetLightData()
    {
        return Data;
    }

    void SetOrder(int _Order)
    {
        m_Order = _Order;
        m_ChangeOrder = true;
    }

    std::vector<int>& GetViewList()
    {
        return m_ViewList;
    }

    int GetOrder()
    {
        return m_Order;
    }

public:
    void InitLight(int _Order);

public:
    template<typename ... REST >
    void Start(int _Order, REST... _Arg)
    {
        PushViewList(_Arg...);
        InitLight(_Order);
    }

    template<typename ... REST >
    void PushViewList(int _ViewOrder, REST ... _Arg)
    {
        m_ViewList.push_back(_ViewOrder);
        PushViewList(_Arg...);
    }

    void PushViewList()
    {

    }

private:
    float m_Near;
    float m_Far;
    float4 m_ViewSize;
    float4 m_View;
    float4 m_Proj;

    std::shared_ptr<GJHDirectRenderTarget> m_ShadowTarget;

public:
    std::shared_ptr<GJHDirectRenderTarget> GetShadowTarget()
    {
        return m_ShadowTarget;
    }

    inline GJHRenderingTransformData& GetLightTransformMatrix()
    {
        return GetActorTrans()->GetRenderingData();
    }

public:
    GJHLight();
    ~GJHLight();

public:
    GJHLight(const GJHLight& _Other) = delete;
    GJHLight(const GJHLight&& _Other) = delete;

public:
    GJHLight& operator=(const GJHLight& _Other) = delete;
    GJHLight& operator=(const GJHLight&& _Other) = delete;

public:
    void CalLightData();
};

