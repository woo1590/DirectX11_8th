#pragma once
#include "UIObject.h"

NS_BEGIN(Client)

class BossIcon :
    public UIObject
{
public: 
    typedef struct tagBossIconDesc : public UIObject::UIOBJECT_DESC
    {

    }BOSS_ICON_DESC;
private:
    BossIcon();
    BossIcon(const BossIcon& prototype);
    virtual ~BossIcon() = default;

public:
    static BossIcon* Create();
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