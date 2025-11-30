#pragma once
#include "UIObject.h"

NS_BEGIN(Client)

class MiniMapPanel :
    public UIObject
{
    enum class Parts { Time, Level, Area, Count };
private:
    MiniMapPanel();
    MiniMapPanel(const MiniMapPanel& prototype);
    virtual ~MiniMapPanel() = default;

public:
    static MiniMapPanel* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    HRESULT CreateChildren();
};

NS_END