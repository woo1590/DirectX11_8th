#pragma once
#include "UIObject.h"

NS_BEGIN(Client)

class Crosshair :
    public UIObject
{
    public:
    typedef struct tagSightDesc : public UIObject::UIOBJECT_DESC
    {

    }SIGHT_DESC;
private:
    Crosshair();
    Crosshair(const Crosshair& prototype);
    virtual ~Crosshair() = default;
    
public:
    static Crosshair* Create();
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