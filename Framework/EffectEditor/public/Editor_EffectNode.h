#pragma once
#include "PartObject.h"
#include "Editor_EffectContainer.h"

NS_BEGIN(Engine)
class Material;
NS_END

NS_BEGIN(EffectEditor)

class Editor_EffectNode :
    public PartObject
{
public:
    typedef struct tagEffectNodeDesc : public PartObject::PART_OBJECT_DESC
    {
        EffectType effectType = EffectType::Count;
        Editor_EffectContainer::EFFECT_EDIT_CONTEXT* context = nullptr;
    }EFFECT_NODE_DESC;

protected:
    Editor_EffectNode();
    Editor_EffectNode(const Editor_EffectNode& prototype);
    virtual ~Editor_EffectNode() = default;

public:
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    virtual void Start() = 0;
    virtual void Export(nlohmann::ordered_json& j) = 0;

    virtual Object* Clone(InitDESC* arg) = 0;
    void Free()override;

protected:
    Editor_EffectContainer::EFFECT_EDIT_CONTEXT* m_pContext = nullptr;
    EffectType m_eEffectType = EffectType::Count;

    /*----public effect params----*/
    static _uint m_iMaterialIndex;

    _float m_fDuration{};
    _float m_fElapsedTime{};
};

NS_END