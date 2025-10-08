#pragma once
#include "ContainerObject.h"

NS_BEGIN(Client)

class Weapon;
class Player final:
    public ContainerObject
{
public:
    enum class Parts { Hand, RightHandSocket, Weapon, PlayerCam, Count };
    enum class WeaponSlot { None, Weapon1, Weapon2, Weapon3, Count };
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

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    HRESULT CreatePartObjects();
    void KeyInput(_float dt);
    void Equip();


    std::vector<Weapon*> m_Weapons;
    WeaponSlot m_eCurrWeaponSlot = WeaponSlot::None;

private:
    class PlayerIdle : public State
    {
        void Enter(Object* object) override {};
        void Update(Object* object, _float dt) override {};
        void TestForExit(Object* object) override {};
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
    PlayerStartEquip m_PlayerStartEquip;
    PlayerEndEquip m_PlayerEndEquip;
};

NS_END