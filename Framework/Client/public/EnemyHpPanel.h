#pragma once
#include "UIObject.h"

NS_BEGIN(Client)

class EnemyHpPanel :
    public UIObject
{
public:
    typedef struct tagEnemyHpPanel :public UIObject::UIOBJECT_DESC
    {
        _uint ownerID{};
    }ENEMY_HP_PANEL_DESC;

    typedef struct tagEnemyHpPanelParam
    {
        _uint ownerID{};
        _float ratio{};
        _float3 position{};
    }ENEMY_HP_PANEL_PARAM;
private:
    enum class Parts { Bar_Background, Bar_Background2, Bar_Decrease, Bar_Hp, Count };
    EnemyHpPanel();
    EnemyHpPanel(const EnemyHpPanel& prototype);
    virtual ~EnemyHpPanel() = default;

public:
    static EnemyHpPanel* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    /*Event*/
    void HealthDecrease(std::any param);
    void UpdatePosition(std::any param);
    void OwnerEnemyDead(std::any param);
    void ActivePanel(std::any param);

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies);

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    HRESULT CreateChildren();
    _uint m_iOwnerID{};

    _bool m_IsActive = false;
    _float m_fElapsedTime = 0.f;
    _float m_fDuration = 2.f;
};

NS_END