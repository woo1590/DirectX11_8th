#pragma once
#include "Enemy.h"

NS_BEGIN(Client)

class Bomber :
    public Enemy
{
    enum class AnimationState
    {
        StandInCombat,
        HideRight,
        HideLeft,
        Attack1,
        Run,
        Patrol,
        Die,
        JumpStart,
        JumpEnd
    };
private:
    Bomber();
    Bomber(const Bomber& prototype);
    virtual ~Bomber() = default;

public:
    static Bomber* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    class BomberIdle : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class BomberPatrol : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class BomberRun : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fSpeed = 30.f;
    };
    class BomberAttack : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };

    BomberIdle m_BomberIdle;
    BomberPatrol m_BomberPatrol;
    BomberRun m_BomberRun;
    BomberAttack m_BomberAttack;

};

NS_END