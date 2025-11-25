#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class SpriteComponent;
NS_END

NS_BEGIN(Client)

class HitCrossHair :
    public UIObject
{
    enum class Parts { LT, RT, RB, LB, Count };
private:
    HitCrossHair();
    HitCrossHair(const HitCrossHair& prototype);
    virtual ~HitCrossHair() = default;

public:
    static HitCrossHair* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)override;

    void Hit(std::any param);
    void HitWeakness(std::any param);

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    std::vector<SpriteComponent*> m_CrossHairs;
    std::vector<TransformComponent*> m_Transforms;
    _float3 m_StartPosition{};
    _float3 m_EndPosition{};
    _bool m_IsActive = false;
    _bool m_IsWeakness = false;
    _float m_fElapsedTime{};
    _float m_fDuration = 0.3f;

};

NS_END