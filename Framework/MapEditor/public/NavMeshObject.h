#pragma once
#include "Object.h"

NS_BEGIN(Engine)
class VIBufferCell;
class MaterialInstance;
NS_END


NS_BEGIN(MapEditor)

class PickingSystem;
class NavMeshObject :
    public Object
{
public:
    typedef struct tagNavCellDesc : public Object::OBJECT_DESC
    {
        const _float3* points = nullptr;
        _uint index{};
    }NAVCELL_DESC;

private:
    NavMeshObject();
    NavMeshObject(const NavMeshObject& prototype);
    virtual ~NavMeshObject() = default;

public:
    static NavMeshObject* Create(PickingSystem* picking);
    HRESULT Initialize_Prototype(PickingSystem* picking);
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)override;

    void ExportNavData(std::ofstream& file);
    void ImportNavData(std::ifstream& file);

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    PickingSystem* m_pPickingSystem = nullptr;
};

NS_END