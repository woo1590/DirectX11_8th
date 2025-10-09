#pragma once
#include "Object.h"

NS_BEGIN(Engine)
class VIBufferCell;
class MaterialInstance;
NS_END


NS_BEGIN(MapEditor)

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
    static NavMeshObject* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:

};

NS_END