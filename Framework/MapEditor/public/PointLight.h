#pragma once
#include "Object.h"

NS_BEGIN(MapEditor)

class PointLight :
    public Object
{
private:
    PointLight();
    PointLight(const PointLight& prototype);
    virtual ~PointLight() = default;

public:
    static PointLight* Create();
    HRESULT Initialize_Prototype();
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void Import(nlohmann::ordered_json& j);
    void Export(nlohmann::ordered_json& j);

    Object* Clone(InitDESC* arg)override;
    void Free()override;
};

NS_END