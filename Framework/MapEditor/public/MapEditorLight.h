#pragma once
#include "Object.h"

NS_BEGIN(MapEditor)

class MapEditorLight :
    public Object
{
private:
    MapEditorLight();
    MapEditorLight(const MapEditorLight& prototype);
    virtual ~MapEditorLight() = default;

public:
    static MapEditorLight* Create();
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