#pragma once
#include "Weapon.h"

NS_BEGIN(Client)

class PoisonousGhost :
    public Weapon
{
    enum class AnimationState
    {
        Idle,
        Fire1,
        Fire2,
        Fire1Return,
        Fire2Return,
        Skill,
        Reload,
        Fire3,
        Fire3Return,
        Count
    };
private:
    PoisonousGhost();
    PoisonousGhost(const PoisonousGhost& prototype);
    virtual ~PoisonousGhost() = default;

public:
    static PoisonousGhost* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void Idle()override {};
    void Reload()override;
    void Fire()override;
    void Skill()override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    class PoisonousGhostIdle : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };
    class PoisonousGhostReload : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };
    class PoisonousGhostFire1 : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };
    class PoisonousGhostFire1Return : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };
    class PoisonousGhostFire2 : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };
    class PoisonousGhostFire2Return : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };
    class PoisonousGhostFire3 : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };
    class PoisonousGhostFire3Return : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };
    class PoisonousGhostSkill : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };

    PoisonousGhostIdle m_PoisonousGhostIdle;
    PoisonousGhostReload m_PoisonousGhostReload;
    PoisonousGhostFire1 m_PoisonousGhostFire1;
    PoisonousGhostFire1Return m_PoisonousGhostFire1Return;
    PoisonousGhostFire2 m_PoisonousGhostFire2;
    PoisonousGhostFire2Return m_PoisonousGhostFire2Return;
    PoisonousGhostFire3 m_PoisonousGhostFire3;
    PoisonousGhostFire3Return m_PoisonousGhostFire3Return;
    PoisonousGhostSkill m_PoisonousGhostSkill;
};

NS_END