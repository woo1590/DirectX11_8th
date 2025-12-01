#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class VIBuffer;
class Material;
NS_END

NS_BEGIN(Client)

class MinimapArea :
    public UIObject
{
private:
    MinimapArea();
    MinimapArea(const MinimapArea& prototype);
    virtual ~MinimapArea() = default;

public:
    static MinimapArea* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)override;
    HRESULT DrawMinimap();

    void DoorMarkActive(std::any param);
    void DoorMarkDeactive(std::any param);

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _float2 ComputeMinimapPosition(_float3 worldPosition);

    VIBuffer* m_pBuffer = nullptr;
    Material* m_pPlayerIcon = nullptr;
    Material* m_pEnemyIcon = nullptr;
    Material* m_pDoorIcon = nullptr;
    _bool m_IsDoorActive = false;
    _float3 m_DoorPosition{};
    MaterialInstance* m_pMaterialInstance = nullptr;

    _float m_fMinimapDistance = 250.f;
};

NS_END