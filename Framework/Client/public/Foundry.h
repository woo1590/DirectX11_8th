#pragma once
#include "Weapon.h"

NS_BEGIN(Client)

class Foundry :
    public Weapon
{
    enum class AnimationState
    {
        Fire,
        Idle,
        Reload
    };
private:
    Foundry(); 
    Foundry(const Foundry& prototype);
    virtual ~Foundry() = default;

public: 
    static Foundry* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void Idle()override {};
    void Reload()override;
    void Fire()override;
    void Skill()override {};

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _uint m_iFireLightBoneIndex{};

    class FoundryIdle : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };
    class FoundryReload : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };
    class FoundryFire : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;
    };

    FoundryIdle m_FoundryIdle;
    FoundryReload m_FoundryReload;
    FoundryFire m_FoundryFire;
};

NS_END