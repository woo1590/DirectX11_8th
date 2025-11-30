#pragma once
#include "Object.h"

NS_BEGIN(Engine)
class SpriteComponent;
NS_END

NS_BEGIN(Client)

class OpenDoorMark :
    public Object
{
private:
    OpenDoorMark();
    OpenDoorMark(const OpenDoorMark& prototype);
    virtual ~OpenDoorMark() = default;

public:
    static OpenDoorMark* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies);
    
    void Dead(std::any param);

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    SpriteComponent* m_pDoorRing = nullptr;
    SpriteComponent* m_pDoorArrow = nullptr;
    _float m_fArrowOffsetY = -0.08f;
    _float m_fElapsedTime{};
};

NS_END