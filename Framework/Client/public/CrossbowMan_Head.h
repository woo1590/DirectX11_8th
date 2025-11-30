#pragma once
#include "PartObject.h"

NS_BEGIN(Client)

class CrossbowMan_Head :
    public PartObject
{
public:
    typedef struct tagCrossbowManHeadDesc : public PartObject::PART_OBJECT_DESC
    {

    }CROSSBOW_MAN_HEAD_DESC;
private:
    CrossbowMan_Head();
    CrossbowMan_Head(const CrossbowMan_Head& prototype);
    virtual ~CrossbowMan_Head() = default;

public:
    static CrossbowMan_Head* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void OnCollisionEnter(ColliderComponent* otherCollider);

    Object* Clone(InitDESC* arg)override;
    void Free()override;
};

NS_END