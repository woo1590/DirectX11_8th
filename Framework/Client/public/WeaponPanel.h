#pragma once
#include "UIObject.h"

NS_BEGIN(Client)

class WeaponPanel :
    public UIObject
{
    enum class Parts { Slot1, Slot2, Slot3, WeaponIcon, Count };
private:
    WeaponPanel();
    WeaponPanel(const WeaponPanel& prototype);
    virtual ~WeaponPanel() = default;

public:
    static WeaponPanel* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    /*Event*/
    void ChangeWeapon(std::any weaponID);
    void ChangeCurrSlot(std::any slotNum);
    void OnJump(std::any param);
    void OnLand(std::any param);
    
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    HRESULT CreateChildren();

    class WeaponPanelIdle : public State
    {
        void Enter(Object* object)override {}
        void Update(Object* object, _float dt)override {}
        void TestForExit(Object* object)override {}
    };
    class WeaponPanelOnJump : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fDuration = 0.5f;
        _float m_fElapsedTime = 0.f;
        _float3 m_StartPosition{};
        _float3 m_TargetPosition{};
    };
    class WeaponPanelOnLand : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fDuration = 0.3f;
        _float m_fElapsedTime = 0.f;
        _float3 m_StartPosition{};
        _float3 m_TargetPosition{};
    };

    WeaponPanelIdle m_WeaponPanelIdle;
    WeaponPanelOnJump m_WeaponPanelOnJump;
    WeaponPanelOnLand m_WeaponPanelOnLand;
};

NS_END