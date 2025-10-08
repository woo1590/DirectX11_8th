#pragma once
#include "Weapon.h"

NS_BEGIN(Client)

class Cameleon :
    public Weapon
{
private:
    Cameleon();
    Cameleon(const Cameleon& prototype);
    virtual ~Cameleon() = default;

public:
    static Cameleon* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void Idle()override;
    void Reload() override;
    void Fire() override;
    void Skill()override {};

    Object* Clone(InitDESC* arg);
    void Free()override;

private:
    class CameleonIdle : public State
    {
        void Enter(Engine::Object* object) override;
        void Update(Engine::Object* object, Engine::_float dt) override;
        void TestForExit(Engine::Object* object) override;
    };

    class CameleonReload : public State
    {
        void Enter(Engine::Object* object) override;
        void Update(Engine::Object* object, Engine::_float dt) override;
        void TestForExit(Engine::Object* object) override;
    };

    class CameleonFire : public State
    {
        void Enter(Engine::Object* object) override;
        void Update(Engine::Object* object, Engine::_float dt) override;
        void TestForExit(Engine::Object* object) override;
    };

    CameleonIdle m_CameleonIdle;
    CameleonReload m_CameleonReload;
    CameleonFire m_CameleonFire;

};

NS_END