#pragma once
#include "PartObject.h"

NS_BEGIN(Client)

class Beetle_Head :
    public PartObject
{
public:
    typedef struct tagBeetleHeadDesc : public PartObject::PART_OBJECT_DESC
    {

    }BEETLE_HEAD_DESC;
private:
    Beetle_Head();
    Beetle_Head(const Beetle_Head& prototype);
    virtual ~Beetle_Head() = default;

public:
    static Beetle_Head* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    void OnCollisionEnter(ColliderComponent* otherCollider)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:

};

NS_END