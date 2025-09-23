#pragma once
#include "Object.h"

NS_BEGIN(Engine)

class PartObject;
class ENGINE_DLL ContainerObject :
    public Object
{
public:
    typedef struct tagContainerObjectDesc : public OBJECT_DESC
    {
        _uint numPartObjects{};

    }CONTAINER_OBJECT_DESC;

protected:
    ContainerObject();
    ContainerObject(const ContainerObject& prototype);
    virtual ~ContainerObject() = default;

public:
    virtual HRESULT Initialize_Prototype()override;
    virtual HRESULT Initialize(InitDESC* arg)override;
    virtual void PriorityUpdate(_float dt)override;
    virtual void Update(_float dt)override;
    virtual void LateUpdate(_float dt)override;

    virtual Object* Clone(InitDESC* arg) = 0;
    virtual void Free()override;

protected:
    HRESULT AddPartObject(_uint levelID, const _string& prototypeTag, _uint partObjectIndex, InitDESC* arg);

    _uint m_iNumPartObjects{};
    std::vector<PartObject*> m_PartObjects;

};

NS_END