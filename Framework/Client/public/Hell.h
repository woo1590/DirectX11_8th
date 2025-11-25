#pragma once
#include "Weapon.h"

NS_BEGIN(Client)

class Hell :
    public Weapon
{
    enum class AnimationState
    {
        Idle,
        Fire1,
        Fire2,
        Fire3,
        ReloadStart,
        ReloadLoop,
        ReloadEnd,
        LeftHandDown,
        Count
    };
private:
    Hell();
    Hell(const Hell& prototype);
    virtual ~Hell() = default;

public:
    static Hell* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void Idle()override {};
    void Reload()override;
    void Fire()override;
    void Skill()override;

#ifdef USE_IMGUI
    void RenderInspector()override;
#endif

    Object* Clone(InitDESC* arg);
    void Free()override;

private:
    _uint m_iFireLightBoneIndex{};
    Object* m_pMuzzleSocket = nullptr;

    class HellIdle : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class HellReloadStart : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class HellReloadLoop : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _uint m_iReloadCount{};
    };
    class HellReloadEnd : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class HellFire1 : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class HellFire2 : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class HellFire3 : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };

    HellIdle m_HellIdle;
    HellReloadStart m_HellReloadStart;
    HellReloadLoop m_HellReloadLoop;
    HellReloadEnd m_HellReloadEnd;
    HellFire1 m_HellFire1;
    HellFire2 m_HellFire2;
    HellFire3 m_HellFire3;
};

NS_END