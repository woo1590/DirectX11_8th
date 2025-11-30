#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class SpriteComponent;
NS_END

NS_BEGIN(Client)

class InteractionUI :
    public UIObject
{
private:
    InteractionUI();
    InteractionUI(const InteractionUI& prototype);
    virtual ~InteractionUI() = default;

public:
    static InteractionUI* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)override;

    void InteractionWeapon(std::any param);
    void InteractionDoor(std::any param);
    void InteractionChest(std::any param);
    void InteractionDeactive(std::any param);

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _bool m_IsActive = false;
    _wstring m_strInteractionText{};
    SpriteComponent* m_pInteractionKey = nullptr;
    TransformComponent* m_pKeyTransform = nullptr;
    std::vector<_float2> m_Offsets;

    _bool m_OnActive = false;
    _bool m_OnDeactive = false;
    _float m_fElpasedTime{};
    _float m_fDuration = 0.2f;
    _float m_fFontAlpha = 0.f;
};

NS_END