#pragma once
#include "UIObject.h"

NS_BEGIN(Client)

class MouseCursor :
    public UIObject
{
private:
    MouseCursor();
    MouseCursor(const MouseCursor& prototype);
    virtual ~MouseCursor() = default;

public:
    static MouseCursor* Create();
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