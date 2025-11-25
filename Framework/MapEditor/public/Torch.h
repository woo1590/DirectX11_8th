#pragma once
#include "Object.h"

NS_BEGIN(MapEditor)

class Torch :
    public Object
{
private:
    Torch();
    Torch(const Torch& prototype);
    virtual ~Torch() = default;

public:
    static Torch* Create();
    HRESULT Initialize_Prototype();
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void Import(nlohmann::ordered_json& j);
    void Export(nlohmann::ordered_json& j);

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:

};

NS_END

