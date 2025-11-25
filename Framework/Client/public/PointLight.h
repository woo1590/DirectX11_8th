#pragma once
#include "Object.h"

NS_BEGIN(Client)

class PointLight :
    public Object
{
public:
    typedef struct tagPointLightDesc : public Object::OBJECT_DESC
    {
        _float range{};
        _float4 color{};
    }POINT_LIGHT_DESC;
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

    Object* Clone(InitDESC* arg)override;
    void Free()override;
};


NS_END