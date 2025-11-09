#pragma once
#include "Component.h"

NS_BEGIN(Client)

class StatusComponent :
    public Component
{
public:
    typedef struct tagStatusDesc : public InitDESC
    {
        _uint hp{};
        _uint shield{};
        _uint attackPower{};
        _float speed{};
    }STATUS_DESC;
private:
    StatusComponent(Object* owner);
    StatusComponent(const StatusComponent& prototype);
    virtual ~StatusComponent() = default;

public:
    static StatusComponent* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    _float GetShieldRatio();
    _float GetHpRatio();
    void RegenShield(_float dt);
    void HealthUp(_uint health);
    void BeAttacked(_uint power);
    STATUS_DESC GetDesc()const;

    Component* Clone() { return new StatusComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override {};
#endif

private:
    _int m_iMaxHP{};
    _int m_iHP{};
    _int m_iMaxShield{};
    _int m_iShield{};
    _uint m_iAttackPower{};
    _float m_fMoveSpeed{};

    _float m_fRegenDuration = 0.05f;
    _float m_fRegenElapsedTime{};
};

NS_END