#pragma once
#include "PartObject.h"

NS_BEGIN(Client)

class Sight :
    public PartObject
{
public:
    typedef struct tagSightDesc : public PartObject::PART_OBJECT_DESC
    {

    }SIGHT_DESC;
private:
    Sight();
    Sight(const Sight& prototype);
    virtual ~Sight() = default;
    
public:
    static Sight* Create();
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