#pragma once
#include "UIObject.h"

NS_BEGIN(Client)

class PlayerIcon :
    public UIObject
{
public:
    typedef struct tagPlayerIconDesc : public UIObject::UIOBJECT_DESC
    {

    }PLAYER_ICON_DESC;
private:
    PlayerIcon();
    PlayerIcon(const PlayerIcon& prototype);
    virtual ~PlayerIcon() = default;

public:
    static PlayerIcon* Create();
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