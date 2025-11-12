#pragma once
#include "UIObject.h"

NS_BEGIN(Client)

class BossHpPanel :
    public UIObject
{
    enum class Parts { Bar_Background, Bar_Decrease, Bar_Hp, Boss_Icon, Count };
private:
    BossHpPanel();
    BossHpPanel(const BossHpPanel& prototype);
    virtual ~BossHpPanel() = default;

public:
    static BossHpPanel* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    /*Event*/
    void HealthDecrease(std::any param);

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    HRESULT CreateChildren();
};

NS_END