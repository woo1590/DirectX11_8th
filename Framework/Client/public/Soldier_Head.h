#pragma once
#include "PartObject.h"

NS_BEGIN(Client)

class Soldier_Head :
    public PartObject
{
public:
    typedef struct tagSolderHeadDesc : public PartObject::PART_OBJECT_DESC
    {

    }SOLDIER_HEAD_DESC;
private:
    Soldier_Head();
    Soldier_Head(const Soldier_Head& prototype);
    virtual ~Soldier_Head() = default;

public:
    static Soldier_Head* Create();
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