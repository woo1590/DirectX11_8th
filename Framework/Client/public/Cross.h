#pragma once
#include "UIObject.h"

NS_BEGIN(Client)

class Cross :
    public UIObject
{
public:
    typedef struct tagCrossDesc : public UIObject::UIOBJECT_DESC
    {
        _float4 color{};
    }CROSS_DESC;
private:
    Cross();
    Cross(const Cross& prototype);
    virtual ~Cross() = default;

public:
    static Cross* Create();
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