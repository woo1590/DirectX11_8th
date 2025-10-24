#pragma once
#include "Object.h"

NS_BEGIN(Client)

class ThirdCam :
    public Object
{
private:
    ThirdCam();
    ThirdCam(const ThirdCam& prototype);
    virtual ~ThirdCam() = default;

public:
    static ThirdCam* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    Object* m_pTarget = nullptr;
    _float3 m_Offset{ 0.f,30.f,0.f };
};

NS_END