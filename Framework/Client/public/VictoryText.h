#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class SpriteComponent;
NS_END

NS_BEGIN(Client)

class VictoryText :
    public UIObject
{
private:
    VictoryText();
    VictoryText(const VictoryText& prototype);
    virtual ~VictoryText() = default;

public:
    static VictoryText* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    SpriteComponent* m_pWhiteText = nullptr;

    /*ring*/
    SpriteComponent* m_pRing = nullptr;
    TransformComponent* m_pRingTransform = nullptr;

    /*rock*/
    SpriteComponent* m_pRock = nullptr;
    TransformComponent* m_pRockTransform = nullptr;

    class VictoryTextAppear : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fDuration = 0.1f;
        _float m_fElapsedTime{};
        _float m_fWhiteElapsedTime{};
        _float m_fWhiteDuration = 0.03f;

        _float3 m_StartScale{};
        _float3 m_EndScale{};
        _float3 m_RockStartScale{};
        _float3 m_RockEndScale{};
    };
    class VictoryTextIdle : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fDuration = 1.5f;
        _float m_fElapsedTime{};
        _float3 m_StartScale{};
        _float3 m_EndScale{};

        _float3 m_RingStartScale{};
        _float3 m_RingEndScale{};
        _float3 m_RockStartScale{};
        _float3 m_RockEndScale{};
    };
    class VictoryTextDisappear : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fDuration = 0.5f;
        _float m_fElapsedTime{};
        _float3 m_StartScale{};
        _float3 m_EndScale{};
        _float3 m_RockStartScale{};
        _float3 m_RockEndScale{};
    };

    VictoryTextAppear m_VictoryTextAppear;
    VictoryTextIdle m_VictoryTextIdle;
    VictoryTextDisappear m_VictoryTextDisappear;
};

NS_END