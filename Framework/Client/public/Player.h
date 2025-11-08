#pragma once
#include "ContainerObject.h"

NS_BEGIN(Client)

class WeaponPanel;
class PlayerPanel;
class CrosshairController;
class Weapon;
class Player final:
    public ContainerObject
{
public:
    enum class Parts { PlayerCam, Hand, RightHandSocket, Weapon, Count };
    enum class WeaponSlot { Slot1, Slot2, Slot3, Count };
    typedef struct tagChangeWeaponEventParam
    {
        _uint slotNum{};
        WeaponID weaponID{};
    }CHANGE_WEAPON_EVENT_PARAM;

private:
    Player();
    Player(const Player& prototype);
    virtual ~Player() = default;

public:
    static Player* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)override;

    /*API*/
    void PickUpWeapon(WeaponID id);
    void AddRecoil(_float power);
    _float3 GetAimPosition();
    void SetShotState(_bool shot) { m_IsShot = shot; }

    void OnCollisionEnter(ColliderComponent* otherCollider)override;
    void OnCollisionStay(ColliderComponent* otherCollider)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    HRESULT CreatePartObjects();
    void KeyInput(_float dt);
    void MakeHandState();
    void EquipCurrSlot();
    void DropCurrSlotWeapon();

    CrosshairController* m_pCrosshairController = nullptr;
    std::vector<Weapon*> m_Weapons;
    WeaponSlot m_eCurrWeaponSlot = WeaponSlot::Slot1;

    _bool m_IsJump = false;
    _bool m_IsWalk = false;
    _bool m_IsDash = false;
    _bool m_IsShot = false;

    _float m_fDashElapsedTime = 0.f;
    _float m_fDashCoolDown = 2.5f;

    _uint m_iLastShield{};
    _uint m_iLastHp{};

private:
    class PlayerIdle : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override {};
        void TestForExit(Object* object) override {};
    };
    class PlayerDash : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;

        _float m_fStartSpeed = 180.f;
        _float m_fEndSpeed = 80.f;
        _float3 m_DashDirection{};

        _float m_fElapsedTime = 0.f;
        _float m_fDuration = 0.3f;
    };
    class PlayerStartEquip : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;

        _float m_fElapsedTime = 0.f;
        _float m_fDuration = 0.3f;
    };
    class PlayerEndEquip : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;

        _float m_fElapsedTime = 0.f;
        _float m_fDuration = 0.3f;
    };

    PlayerIdle m_PlayerIdle;
    PlayerDash m_PlayerDash;
    PlayerStartEquip m_PlayerStartEquip;
    PlayerEndEquip m_PlayerEndEquip;
};

NS_END