#pragma once
#include "Item.h"
class Coin :
    public Item
{
private:
    Coin();
    Coin(const Coin& prototype);
    virtual ~Coin() = default;

public:
    static Coin* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void Interaction(PlayerInteractionComponent* interaction)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    class CoinSpawn : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fElapsedTime = 0.f;
        _float m_fDuration = 1.5f;
    };
    class CoinIdle : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fMagneticDistance = 30.f;
        _float m_fElapsedTime = 0.f;
        _float3 m_CurrPosition{};
    };
    class CoinMagnetic : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fMagneticDistance = 30.f;
        _float m_fSpeed = 4.f;
    };

    CoinSpawn m_CoinSpawn;
    CoinIdle m_CoinIdle;
    CoinMagnetic m_CoinMagnetic;
};

