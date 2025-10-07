#pragma once
#include "Object.h"

NS_BEGIN(Engine)

class ContainerObject;
class ENGINE_DLL PartObject :
    public Object
{
public:
    typedef struct PartObjectDesc :public OBJECT_DESC
    {
        ContainerObject* parent = nullptr;
        TransformComponent* parentSocketTransform = nullptr;

    }PART_OBJECT_DESC;

protected:
    PartObject();
    PartObject(const PartObject& prototype);
    virtual ~PartObject() = default;

public:
    virtual HRESULT Initialize_Prototype()override;
    virtual HRESULT Initialize(InitDESC* arg)override;
    virtual void PriorityUpdate(_float dt)override;
    virtual void Update(_float dt)override;
    virtual void LateUpdate(_float dt)override;

    virtual Object* Clone(InitDESC* arg) = 0;
    virtual void Free()override;

protected:
    ContainerObject* m_pParent = nullptr;

};

NS_END