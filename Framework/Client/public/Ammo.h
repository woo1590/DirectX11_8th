#pragma once
#include "Item.h"

NS_BEGIN(Client)

class Ammo :
    public Item
{
private:
    Ammo();
    Ammo(const Ammo& prototype);
    virtual ~Ammo() = default;

public:
    static Ammo* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void Interaction(PlayerInteractionComponent* interaction)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    class AmmoSpawn : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fElapsedTime = 0.f;
        _float m_fDuration = 1.5f;
    };
    class AmmoIdle : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fMagneticDistance = 30.f;
        _float m_fElapsedTime = 0.f;
        _float3 m_CurrPosition{};
    };
    class AmmoMagnetic : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
        
        _float m_fMagneticDistance = 30.f;
        _float m_fSpeed = 4.f;
    };

    AmmoSpawn m_AmmoSpawn;
    AmmoIdle m_AmmoIdle;
    AmmoMagnetic m_AmmoMagnetic;
};

NS_END