#pragma once
#include "Weapon.h"

NS_BEGIN(Client)

class ConcealedAmmo :
    public Weapon
{
    enum class AnimationState { Idle, Reload, Fire, Count };
private:
    ConcealedAmmo();
    ConcealedAmmo(const ConcealedAmmo& prototype);
    virtual ~ConcealedAmmo() = default;

public:
    static ConcealedAmmo* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void Idle()override;
    void Reload()override;
    void Fire()override;
    void Skill()override;

    Object* Clone(InitDESC* arg);
    void Free()override;

private:
    _uint m_iFireLightBoneIndex{};
    Object* m_pMuzzleSocket = nullptr;

    class ConcealedAmmoIdle : public State
    {
        void Enter(Engine::Object* object) override;
        void Update(Engine::Object* object, Engine::_float dt) override;
        void TestForExit(Engine::Object* object) override;
    };

    class ConcealedAmmoReload : public State
    {
        void Enter(Engine::Object* object) override;
        void Update(Engine::Object* object, Engine::_float dt) override;
        void TestForExit(Engine::Object* object) override;
    };

    class ConcealedAmmoFire : public State
    {
        void Enter(Engine::Object* object) override;
        void Update(Engine::Object* object, Engine::_float dt) override;
        void TestForExit(Engine::Object* object) override;

        _bool m_IsShot = false;
    };

    class ConcealedAmmoSkill : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override;
        void TestForExit(Object* object) override;

        _bool m_IsShot = false;
    };

    ConcealedAmmoIdle m_ConcealedAmmoIdle;
    ConcealedAmmoReload m_ConcealedAmmoReload;
    ConcealedAmmoFire m_ConcealedAmmoFire;
    ConcealedAmmoSkill m_ConcealedAmmoSkill;
};

NS_END