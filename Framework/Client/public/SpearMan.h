#pragma once
#include "Enemy.h"

NS_BEGIN(Client)

class SpearMan :
    public Enemy
{
    enum class AnimationState
    {
        StandInCombat,
        Patrol,
        Stand,
        Guard,
        HitHead,
        Run1,
        Run2,
        Scatterin,
        Combine,
        Show,
        Jump,
        Land,
        HitBody,
        HideRight,
        HideLeft,
        Attack,
        ChargeAttack,
        Throw,
        LightHit,
        Count
    };
    enum class Parts { Head, Head_Socket, Spear, SpearSocket, Count };
private:
    SpearMan();
    SpearMan(const SpearMan& prototype);
    virtual ~SpearMan() = default;

public:
    static SpearMan* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void OnCollisionEnter(ColliderComponent* otherCollider)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    HRESULT CreatePartObjects();

    class SpearManShow : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };
    class SpearManIdle : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };
    class SpearManPatrol : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };
    class SpearManRun : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };
    class SpearManAttack : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };
    class SpearManChargeAttack : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;

        _float m_fAttackSpeed = 100.f;
        _bool m_IsStartAttacked = false;
        _bool m_IsEndAttacked = false;
    };
    class SpearManHitBody : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };
    class SpearManHitHead : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };
    class SpearManDead : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };

    SpearManShow m_SpearManShow;
    SpearManIdle m_SpearManIdle;
    SpearManPatrol m_SpearManPatrol;
    SpearManRun m_SpearManRun;
    SpearManAttack m_SpearManAttack;
    SpearManChargeAttack m_SpearManChargeAttack;
    SpearManHitBody m_SpearManHitBody;
    SpearManHitHead m_SpearManHitHead;
    SpearManDead m_SpearManDead;
};

NS_END