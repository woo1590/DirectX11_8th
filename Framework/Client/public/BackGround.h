#pragma once
#include "UIObject.h"

NS_BEGIN(Client)

class BackGround final:
    public UIObject
{
public:
    typedef struct tagBackGroundDesc : public UIObject::UIOBJECT_DESC
    {
        _string mtrlTag{};
    }BACKGROUND_DESC;
private:
    BackGround();
    virtual ~BackGround() = default;

public:
    static BackGround* Create();
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