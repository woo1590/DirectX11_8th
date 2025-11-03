#pragma once
#include "Weapon.h"
class Prism :
    public Weapon
{
    enum class AnimationState
    {
        Fire1,
        Idle,
        Fire2,
        Fire3,
        Count
    };
private:
    Prism();
    Prism(const Prism& prototype);
    virtual ~Prism() = default;

public:
    static Prism* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void Idle()override {};
    void Reload()override {};
    void Fire()override;
    void Skill()override {};

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _uint m_iFireLightBoneIndex{};

    class PrismIdle : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class PrismFire : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };

    PrismIdle m_PrismIdle;
    PrismFire m_PrismFire;
};

