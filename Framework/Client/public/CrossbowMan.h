#pragma once
#include "Enemy.h"

NS_BEGIN(Client)

class CrossbowMan :
    public Enemy
{
    enum class AnimationState
    {
        StandInCombat1,
        StandInCombat2,
        Patrol,
        Run,
        Stand,
        SquatInCombat,
        HitBody,
        JumpStart,
        Land,
        Walk_F,
        Walk_R,
        Walk_B,
        Walk_L,
        HideRight,
        HideLeft,
        Throw,
        Fire1,
        Reload,
        LightHit,
        HitHead,
        ShieldInCombat,
        Attack,
        Unbalance,
        Rise,
        Down,
        Die,
        Count
    };
    enum class Parts { Head, Head_Socket, Count };
private:
    CrossbowMan();
    CrossbowMan(const CrossbowMan& prototype);
    virtual ~CrossbowMan() = default;

public:
    static CrossbowMan* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void OnCollisionEnter(ColliderComponent* otherCollider)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    HRESULT CreatePartObjects();

    class CrossbowManShow : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class CrossbowManIdle : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fElapsedTime = 0.f;
        _float m_fDuration = 2.f;
    };
    class CrossbowManWalk_F : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fElapsedTime = 0.f;
        _float m_fDuration = 0.2f;
    };
    class CrossbowManWalk_R : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class CrossbowManWalk_L : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class CrossbowManHide_R : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fHideSpeed = 30.f;
    };
    class CrossbowManHide_L : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fHideSpeed = 30.f;
    };
    class CrossbowManFire : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class CrossbowManReload : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class CrossbowManDead : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };

    CrossbowManShow m_CrossbowManShow;
    CrossbowManIdle m_CrossbowManIdle;
    CrossbowManWalk_F m_CrossbowManWalk_F;
    CrossbowManWalk_R m_CrossbowManWalk_R;
    CrossbowManWalk_L m_CrossbowManWalk_L;
    CrossbowManHide_R m_CrossbowManHide_R;
    CrossbowManHide_L m_CrossbowManHide_L;
    CrossbowManFire m_CrossbowManFire;
    CrossbowManReload m_CrossbowManReload;
    CrossbowManDead m_CrossbowManDead;
};

NS_END