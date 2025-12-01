#pragma once
#include "Enemy.h"

NS_BEGIN(Client)

class Beetle :
    public Enemy
{
    enum class Parts { Head_Socket, Head, Count };
    enum class AnimationState
    {
        Stand,
        StandInCombat,
        Patrol,
        HitBody,
        Attack,
        Run,
        JumpStart,
        Land,
        Count
    };
private:
    Beetle();
    Beetle(const Beetle& prototype);
    virtual ~Beetle() = default;

public:
    static Beetle* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    HRESULT LateInitialize()override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    void OnCollisionEnter(ColliderComponent* otherCollider)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;
    
private:
    HRESULT CreatePartObjects();

    class BeetleShow : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fElapsedTime{};
        _float m_fDuration = 0.8f;
    };
    class BeetleIdle : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class BeetleRun : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fSoundDuration = 1.5f;
        _float m_fSoundElpasedTime{};
    };
    class BeetleAttack : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class BeetleDead : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };

    BeetleShow m_BeetleShow;
    BeetleIdle m_BeetleIdle;
    BeetleRun m_BeetleRun;
    BeetleAttack m_BeetleAttack;
    BeetleDead m_BeetleDead;
};

NS_END