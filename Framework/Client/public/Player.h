#pragma once
#include "ContainerObject.h"

NS_BEGIN(Client)

class Weapon;
class Player final:
    public ContainerObject
{
public:
    enum class Parts { PlayerCam, Hand, RightHandSocket, Weapon, Count };
    enum class WeaponSlot { None, Slot1, Slot2, Slot3, Count };
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

    /*API*/
    void AddRecoil(_float power);
    _float3 GetAimPosition();

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    HRESULT CreatePartObjects();
    void KeyInput(_float dt);
    void MakeHandState();
    void Equip();

    std::vector<Weapon*> m_Weapons;
    WeaponSlot m_eCurrWeaponSlot = WeaponSlot::None;

    _bool m_IsJump = false;
    _bool m_IsWalk = false;
    _bool m_IsDash = false;

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