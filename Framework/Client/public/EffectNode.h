#pragma once
#include "PartObject.h"

NS_BEGIN(Client)

class EffectNode :
    public PartObject

{
public:
    typedef struct tagEffectNodeDesc : public PartObject::PART_OBJECT_DESC
    {
        EffectType type = EffectType::Count;
        _float duration{};
    }EFFECT_NODE_DESC;
protected:
    EffectNode();
    EffectNode(const EffectNode& prototype);
    virtual ~EffectNode() = default;

public:
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

protected:
    EffectType m_eEffectType = EffectType::Count;
    _float m_fDuration{};
    _float m_fElapsedTime{};
};

NS_END