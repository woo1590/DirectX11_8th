#pragma once
#include "Weapon.h"

NS_BEGIN(Client)

class LightningBlast :
    public Weapon
{
private:
    LightningBlast();
    LightningBlast(const LightningBlast& prototype);
    virtual ~LightningBlast() = default;

public:
    static LightningBlast* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void Reload()override;
    void Fire()override;

    Object* Clone(InitDESC* arg);
    void Free()override;

private:
    class LightningBlastIdle : public State
    {
        void Enter(Engine::Object* object) override;
        void Update(Engine::Object* object, Engine::_float dt) override;
        void TestForExit(Engine::Object* object) override;
    };

    class LightningBlastReload : public State
    {
        void Enter(Engine::Object* object) override;
        void Update(Engine::Object* object, Engine::_float dt) override;
        void TestForExit(Engine::Object* object) override;
    };

    class LightningBlastFire : public State
    {
        void Enter(Engine::Object* object) override;
        void Update(Engine::Object* object, Engine::_float dt) override;
        void TestForExit(Engine::Object* object) override;
    };

    LightningBlastIdle m_LightningBlastIdle;
    LightningBlastReload m_LightningBlastReload;
    LightningBlastFire m_LightningBlastFire;


};

NS_END