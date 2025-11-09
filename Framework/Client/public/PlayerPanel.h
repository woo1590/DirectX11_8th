#pragma once
#include "UIObject.h"

NS_BEGIN(Client)

class PlayerPanel :
    public UIObject
{
public:
    typedef struct tagPlayerShieldParam
    {
        _float ratio;
        _uint currShield;
    }PLAYER_SHIELD_PARAM;
    typedef struct tagPlayerHealthParam
    {
        _float ratio;
        _uint currHealth;
    }PLAYER_HEALTH_PARAM;
private:
    enum class Parts 
    { 
        PlayerIcon, 
        Bar_Shield_Background, 
        Bar_Shield_Decrease, 
        Bar_Shield, 
        Bar_Shield_Hurt,
        Bar_Health_Background, 
        Bar_Health_Decrease,
        Bar_Health,
        Bar_Health_Hurt,
        Number_Shield,
        Number_Shield_Cross,
        Number_MaxShield,
        Number_Health,
        Number_Health_Cross,
        Number_MaxHealth,
        Count 
    };
    PlayerPanel();
    PlayerPanel(const PlayerPanel& prototype);
    virtual ~PlayerPanel() = default;

public:
    static PlayerPanel* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    /*Event*/
    void OnJump(std::any param);
    void OnLand(std::any param);
    void OnDash(std::any param);
    void PlayerHealthDecrease(std::any param);
    void PlayerHealthIncrease(std::any param);
    void PlayerShieldDecrease(std::any param);
    void PlayerShieldIncrease(std::any param);

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    HRESULT CreateChildren();

    class PlayerPanelIdle : public State
    {
        void Enter(Object* object)override{}
        void Update(Object* object, _float dt)override{}
        void TestForExit(Object* object)override{}
    };
    class PlayerPanelOnJump : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fDuration = 0.5f;
        _float m_fElapsedTime = 0.f;
        _float3 m_StartPosition{};
        _float3 m_TargetPosition{};
    };
    class PlayerPanelOnLand : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fDuration = 0.3f;
        _float m_fElapsedTime = 0.f;
        _float3 m_StartPosition{};
        _float3 m_TargetPosition{};
    };
    class PlayerPanelOnDash : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fDuration = 0.5f;
        _float m_fElapsedTime = 0.f;
        _float3 m_StartPosition{};
        _float3 m_TargetPosition{};
    };

    PlayerPanelIdle m_PlayerPanelIdle;
    PlayerPanelOnJump m_PlayerPanelOnJump;
    PlayerPanelOnLand m_PlayerPanelOnLand;
    PlayerPanelOnDash m_PlayerPanelOnDash;
};

NS_END