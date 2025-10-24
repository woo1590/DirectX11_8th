#pragma once
#include "Enemy.h"

NS_BEGIN(Client)

class Soldier :
    public Enemy
{
    enum class AnimationState
    {
        StandInCombat,
        Run1,
        Run2,
        Stand,
        HitHead,
        StandUp1,
        StandUp2,
        Show,
        Jump1,
        Jump2,
        HitBody,
        HideRight,
        HideLeft,
        Attack1,
        Attack2,
        Throw,
        LightHit,
        Down,
        Rise,
        Die
    };
    enum class Parts { Head, Head_Socket, Sword, Sword_Socket, Count };
private:
    Soldier();
    Soldier(const Soldier& prototype);
    virtual ~Soldier() = default;

public:
    static Soldier* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    HRESULT CreatePartObjects();

    class SoldierShow : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class SoldierIdle : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class SoldierPatrol : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class SoldierRun : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class SoldierAttack : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };

    SoldierShow m_SoldierShow;
    SoldierIdle m_SoldierIdle;
    SoldierPatrol m_SolderPatrol;
    SoldierRun m_SoldierRun;
    SoldierAttack m_SoldierAttack;
};

NS_END