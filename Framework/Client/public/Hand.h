#pragma once
#include "PartObject.h"

NS_BEGIN(Client)

class BobbingController;
class Hand :
    public PartObject
{
public:
    typedef struct HandDesc : public PartObject::PART_OBJECT_DESC
    {
     
    }HAND_DESC;
private:
    Hand();
    Hand(const Hand& prototype);
    virtual ~Hand() = default;

public:
    static Hand* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void Idle();
    void Walk();
    void StartJump();
    void EndJump();
    void AddRecoil(_float power);

    void SetSwayTargetRoation(_float yaw);
    void SwayEnable();
    void SwayDisable();

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    void UpdateSway(_float dt);
    _float4 m_TargetRotation{};
    _float4 m_CurrRotation{};
    _float3 m_TargetPosition{};
    _float3 m_CurrPosition{};
    _float3 m_InitPosition{ 0.f,0.f,-0.15f };
    _float m_fSwayAmount = 0.02f;
    _float m_fMaxSwayAmount = 0.06f;
    _float m_fSmoothAmount = 5.f;
    _bool m_EnableSway = true;

    BobbingController* m_pBobbingController = nullptr;

    class HandIdle : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override {};
        void TestForExit(Object* object) override {};
    };
    class HandWalk : public State
    {
        void Enter(Object* object) override;
        void Update(Object* object, _float dt) override {};
        void TestForExit(Object* object) override {};

    };

    HandIdle m_HandIdle;
    HandWalk m_HandWalk;

    _float timer = 0.f;
};

NS_END