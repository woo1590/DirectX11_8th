#pragma once
#include "ContainerObject.h"

NS_BEGIN(Client)

class EffectContainer :
    public ContainerObject
{
public:
    typedef struct tagEffectContainerDesc : public ContainerObject::CONTAINER_OBJECT_DESC
    {
        _uint numNodes{};
        _float duration{};
    }EFFECT_CONTAINER_DESC;
private:
    EffectContainer();
    EffectContainer(const EffectContainer& prototype);
    virtual ~EffectContainer() = default;

public:
    static EffectContainer* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _float m_fElapsedTime = 0.f;
    _float m_fDuration = 0.f;
};

NS_END