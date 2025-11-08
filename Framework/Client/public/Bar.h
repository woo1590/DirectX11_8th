#pragma once
#include "UIObject.h"

NS_BEGIN(Client)

class Bar :
    public UIObject
{
public:
    typedef struct tagBarDesc : public UIObject::UIOBJECT_DESC
    {
        _string mtrlTag{};
    }BAR_DESC;

    typedef struct tagBarChangeParam
    {
        _bool immediate = false;
        _bool paused = false;
        _bool useAlpha = false;
        _float targetRatio{};
        _float pauseDuration{};
        _float duration{};
    }BAR_CHANGE_PARAM;
private:
    Bar();
    Bar(const Bar& prototype);
    virtual ~Bar() = default;
    
public:
    static Bar* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void MakeChange(BAR_CHANGE_PARAM param);
    void InverseAlpha();

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _float3 m_OriginScale{};

    _bool m_IsChanged = false;
    _bool m_IsPaused = false;
    _bool m_IsImmediateChange = false;

    _float m_fCurrRatio{};
    _float m_fTargetRatio{};

    _float m_fPauseDuration{};
    _float m_fPauseElapsedTime{};
    _float m_fDuration{};
    _float m_fElapsedTime{};

    _bool m_IsUsedAlpha = false;
    _float m_fAlphaDuration = 0.5f;
    _float m_fAlphaElapsedTime = 0.f;
};

NS_END