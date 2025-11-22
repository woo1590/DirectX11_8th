#pragma once
#include "PartObject.h"

NS_BEGIN(Client)

class BossEye :
    public PartObject
{
public:
    typedef struct tagBossEyeDesc : public PartObject::PART_OBJECT_DESC
    {
        _bool isRightEye = false;
    }BOSS_EYE_DESC;
private:
    BossEye();
    BossEye(const BossEye& prototype);
    virtual ~BossEye() = default;

public:
    static BossEye* Create();
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