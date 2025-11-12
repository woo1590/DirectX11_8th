#pragma once
#include "PartObject.h"

NS_BEGIN(Client)

class EffectNode :
    public PartObject

{
public:
    typedef struct tagEffectNodeDesc : public PartObject::PART_OBJECT_DESC
    {
        EffectNodeType type = EffectNodeType::Count;
        _float duration{};
    }EFFECT_NODE_DESC;
private:
    EffectNode();
    EffectNode(const EffectNode& prototype);
    virtual ~EffectNode() = default;

public:
    static EffectNode* Create();
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