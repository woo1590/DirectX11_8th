#pragma once
#include "Enemy.h"

NS_BEGIN(Client)

class HorseHead :
    public Enemy
{
    enum class AnimationState
    {
        StandInCombat,
        Patrol,
        Run,
        Stand,
        Walk_F,
        Walk_R,
        Walk_B,
        Walk_L,
        Turn_L_180,
        Turn_L_90,
        Turn_R_90,
        Turn_R_180,
        Show,
        JumpStart,
        JumpEnd,
        HitHead,
        Attack1,
        ShieldFire,
        Attack2,
        Down,
        Rise,
        Die
    };
    enum class Parts { Head, Head_Socket, Shield, Shield_Socket, Sword, Sword_Socket, Count };
private:
    HorseHead();
    HorseHead(const HorseHead& prototype);
    virtual ~HorseHead() = default;

public:
    static HorseHead* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    HRESULT CreatePartObjects();

    class HorseHeadShow : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };
    class HorseHeadIdle : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };
    class HorseHeadPatrol : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };
    class HorseHeadWalk_F : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };
    class HorseHeadAttack1 : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };

    HorseHeadShow m_HorseHeadShow;
    HorseHeadIdle m_HorseHeadIdle;
    HorseHeadPatrol  m_HorseHeadPatrol;
    HorseHeadWalk_F m_HorseHeadWalk_F;
    HorseHeadAttack1 m_HorseHeadAttack1;

};

NS_END