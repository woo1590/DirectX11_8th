#pragma once
#include "Object.h"

NS_BEGIN(MapEditor)

class MapEditorCamera :
    public Object
{
private:
    MapEditorCamera();
    MapEditorCamera(const MapEditorCamera& prototype);
    virtual ~MapEditorCamera() = default;

public:
    static MapEditorCamera* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:

};

NS_END