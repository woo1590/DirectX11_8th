#pragma once
#include "PartObject.h"

NS_BEGIN(Engine)

class ModelComponent;
class ENGINE_DLL Socket :
    public PartObject
{
public:
    typedef struct SocketDesc : public PartObject::PART_OBJECT_DESC
    {
        ModelComponent* parentModel = nullptr;
        _uint boneIndex{};
        _bool useScale{};
    }SOCKET_DESC;

private:
    Socket();
    Socket(const Socket& prototype);
    virtual ~Socket() = default;

public:
    static Socket* Create();
    virtual HRESULT Initialize_Prototype()override;
    virtual HRESULT Initialize(InitDESC* arg)override;
    virtual void PriorityUpdate(_float dt)override;
    virtual void Update(_float dt)override;
    virtual void LateUpdate(_float dt)override;

    virtual Object* Clone(InitDESC* arg);
    virtual void Free()override;

private:
    ModelComponent* m_pParentModel = nullptr;
    _uint m_iBoneIndex{};
    _bool m_useScale = false;
};

NS_END