#pragma once
#include "Weapon.h"

NS_BEGIN(Client)

class IcySpear :
    public Weapon
{
private:
    IcySpear();
    IcySpear(const IcySpear& prototype);
    virtual ~IcySpear() = default;

public:
    static IcySpear* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void Idle()override {};
    void Reload()override {};
    void Fire()override;
    void Skill()override;

    Object* Clone(InitDESC* arg);
    void Free()override;

private:
    class IcySpearIdle 
        : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };
    class IcySpearFire
        :public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };
    class IcySpearSkill
        :public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };
    

    IcySpearIdle m_IcySpearIdle;
    IcySpearFire m_IcySpearFire;
    IcySpearSkill m_IcySpearSkill;
};

NS_END