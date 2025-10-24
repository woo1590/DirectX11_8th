#pragma once
#include "Base.h"

NS_BEGIN(Client)

class BobbingController :
    public Base
{
    struct JumpParams
    {
        /*jump start*/
        _float upDuration = 0.3f;
        _float upPitch = math::ToRadian(2.f);

        /*on air*/
        _float airDuration = 0.7f;
        _float airPitchMax = math::ToRadian(5.f);
        _float airZMax = 0.02f;

        /*jump end*/
        _float landDuration = 0.2f;
        _float landPitch = math::ToRadian(4.f);
        _float landZ = 0.01f;
    };

    struct JumpState
    {
        _bool onAir = false;
        _bool isJump = false;
        _bool isLand = false;

        _float weight = 0.f;
        _float elapsedTime = 0.f;
    };

    struct RecoilState
    {
        _bool isShot = false;
        _float velocity = 0.f;
        _float positionZ = 0.f;
    };

private:
    BobbingController();
    virtual ~BobbingController() = default;

public:
    static BobbingController* Create();
    HRESULT Initialize();

    void Update(_float dt, _float3& posAdd, _float3& rotAdd);

    void OnIdle();
    void OnWalk();
    void StartJump();
    void EndJump();
    void AddRecoil(_float power);

    void Free()override;

private:
    void UpdateGround(_float dt, _float3& posAdd, _float3& rotAdd);
    void UpdateAir(_float dt, _float3& posAdd, _float3& rotAdd);
    void UpdateRecoil(_float dt, _float3& posAdd, _float3& rotAdd);
    _float FollowExp(_float t, _float target, _float dt);

    /*--state--*/
    _float m_fPhase{};
    _float m_fCurrWeight{};
    _float m_fCurrFreq{};

    _float m_fTargetWeight{};
    _float m_fTargetFreq{};

    _float3 m_CurrOffset{};
    _float3 m_CurrRot{};

    _float3 m_TargetOffset{};
    _float3 m_TargetRot{};

    /*---params---*/
    _float3 m_IdleOffset{0.003f,0.003f,0.f};
    _float3 m_IdleRot{math::ToRadian(0.6f),math::ToRadian(0.2f),0.f};

    _float3 m_WalkOffset{ 0.005f,0.005f,0.f };
    _float3 m_WalkRot{ math::ToRadian(0.6f),math::ToRadian(0.2f),0.f };

    _float m_fIdleFreq = 0.3f;
    _float m_fWalkFreq = 1.5f;

    _float m_fFollow = 12.f;

    JumpParams m_JumpParams;
    JumpState m_JumpState;
    RecoilState m_RecoilState;
    
};

NS_END