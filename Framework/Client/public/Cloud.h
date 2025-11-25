#pragma once
#include "Object.h"

NS_BEGIN(Client)

class Cloud :
    public Object
{
private:
    Cloud();
    Cloud(const Cloud& prototype);
    virtual ~Cloud() = default;

public:
    static Cloud* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _float2 m_UVOffset{};
};

NS_END