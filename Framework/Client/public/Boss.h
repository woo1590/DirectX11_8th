#pragma once
#include "Enemy.h"

NS_BEGIN(Client)

class Boss :
    public Enemy
{
    enum class AnimationState
    {
        StandInCombat,
        Attack1Start,
        Attack1InProgress,
        Attack1End,
        Die,
        Fire1Start,
        Fire1InProgress,
        Fire1End,
        Fire2Start,
        Fire2Ready,
        Fire2LeftArm,
        Fire2RightArm,
        Fire2End,
        Fire3Start,
        Fire3InProgress,
        Fire3End,
        Fire4,
        Count
    };
    enum class Parts { Core, Core_Socket, Head, Head_Socket, RightArm, RightArm_Socket, LeftArm, LeftArm_Socket, RightArmStart_Socket, RightArmEnd_Socket, LeftArmStart_Socket, LeftArmEnd_Socket, Count };
private:
    Boss();
    Boss(const Boss& prototype);
    virtual ~Boss()override;
    
public:
    static Boss* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void HitBody(_uint attackPower);
    void HitWeakness(_uint attackPower);
    void Dead();

    void OnCollisionEnter(ColliderComponent* otherCollider)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    HRESULT CreatePartObjects();

    _float3 m_PillarAreaMin{};
    _float3 m_PillarAreaMax{};
    _float3 m_StoneAreaMin{};
    _float3 m_StoneAreaMax{};

    class BossIdle : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fElapsedTime = 0.f;
        _float m_fDuration = 1.f;
    };
    class BossAttack1Start : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class BossAttack1InProgress : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class BossAttack1End : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        std::vector<_uint> m_IndexX;
        std::vector<_uint> m_IndexZ;
        _uint m_iNumPillars = 10;
        _bool m_IsPillarSpawned = false;
    };
    class BossFire1Start : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        std::vector<_uint> m_IndexX;
        std::vector<_uint> m_IndexY;
        std::vector<_uint> m_IndexZ;
        _uint m_iNumStones = 7;
        _bool m_IsProjectileSpawned = false;
    };
    class BossFire1InProgress : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class BossFire1End : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class BossFire2Start : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class BossFire2Ready : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class BossFire2LeftArm : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class BossFire2RightArm : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class BossFire2End : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float4 m_TargetRotation{};
    };
    class BossFire3Start : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class BossFire3InProgress : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class BossFire3End : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class BossFire4 : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };
    class BossDie : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;
    };

    BossIdle m_BossIdle;

    BossAttack1Start m_BossAttack1Start;
    BossAttack1InProgress m_BossAttack1InProgress;
    BossAttack1End m_BossAttack1End;

    BossFire1Start m_BossFire1Start;
    BossFire1InProgress m_BossFire1InProgress;
    BossFire1End m_BossFire1End;

    BossFire2Start m_BossFire2Start;
    BossFire2Ready m_BossFire2Ready;
    BossFire2LeftArm m_BossFire2LeftArm;
    BossFire2RightArm m_BossFire2RightArm;
    BossFire2End m_BossFire2End;

    BossFire3Start m_BossFire3Start;
    BossFire3InProgress m_BossFire3InProgress;
    BossFire3End m_BossFire3End;

    BossFire4 m_BossFire4;

    BossDie m_BossDie;

};

NS_END