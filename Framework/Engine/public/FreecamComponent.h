#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL FreecamComponent :
    public Component
{
private:
    FreecamComponent(Object* owner);
    virtual ~FreecamComponent() = default;

public:
    static FreecamComponent* Create(Object* owner, InitDESC* arg);
    HRESULT Initialize(InitDESC* arg)override;
    void Update(_float dt)override;

    Component* Clone(InitDESC* arg)override { return nullptr; }
    void Free()override;

private:

};

NS_END