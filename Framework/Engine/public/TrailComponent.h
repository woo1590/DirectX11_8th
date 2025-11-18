#pragma once
#include "Component.h"
#include "RenderProxy.h"

NS_BEGIN(Engine)

typedef struct tagTrailPoint
{
    _float3 position{};
    _float lifeTime{};
}TRAIL_POINT;

class VIBuffer_Trail;
class Material;
class MaterialInstance;
class ENGINE_DLL TrailComponent :
    public Component
{
public:
    typedef struct tagTrailComponentDesc : public InitDESC
    {
        TrailMode mode = TrailMode::Default;
        _float width{};

        /*for ribbon trail*/
        _uint numMaxPoints{};
        _float minDistance{};
        _float maxLifeTime{};
    }TRAIL_DESC;
private:
    TrailComponent(Object* owner);
    TrailComponent(const TrailComponent& prototype);
    virtual ~TrailComponent() = default;

public:
    static TrailComponent* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void Update(_float dt)override;

    void SetMaterial(_uint levelID, const _string& key);
    MaterialInstance* GetMaterialInstance()const { return m_pMaterialInstance; }
    void AddPoints(_float3 point0, _float3 point1);/*only default*/

    HRESULT ExtractRenderProxy(std::vector<RenderProxy>& proxies);
    Component* Clone()override { return new TrailComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override {};
#endif

private:
    void AddPoint(_float3 point);/*only ribbon*/
    void BuildVertexData();

    TrailMode m_eMode = TrailMode::Default;

    VIBuffer_Trail* m_pBuffer = nullptr;
    Material* m_pMaterial = nullptr;
    MaterialInstance* m_pMaterialInstance = nullptr;
    _uint m_iMaxNumPoints{};

    std::vector<VTX_TRAIL> m_DefaultPoints;
    std::deque<TRAIL_POINT> m_RibbonPoints;
    _float m_fWidth{};
    _float m_fMinDistance{};
    _float m_fMaxLifeTime{};
};

NS_END