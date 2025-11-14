#pragma once
#include "ContainerObject.h"

NS_BEGIN(Client)

class EffectContainer :
    public ContainerObject
{
public:
    typedef struct tagEffectContainerDesc : public ContainerObject::CONTAINER_OBJECT_DESC
    {
        Object* socketObject = nullptr;
        _float3 forward{ 0.f,0.f,1.f };
    }EFFECT_CONTAINER_DESC;
private:
    EffectContainer();
    EffectContainer(const EffectContainer& prototype);
    virtual ~EffectContainer() = default;

public:
    static EffectContainer* Create(const _string& filePath);
    HRESULT Initialize_Prototype(const _string& filePath);
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    Object* m_pParentSocket = nullptr;
    _float m_fElapsedTime = 0.f;
    _float m_fDuration = 0.f;
};

NS_END