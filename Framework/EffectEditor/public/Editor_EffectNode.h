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

private:
    Editor_EffectNode();
    Editor_EffectNode(const Editor_EffectNode& prototype);
    virtual ~Editor_EffectNode() = default;

public:
    static Editor_EffectNode* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies);

#ifdef USE_IMGUI
    void RenderInspector()override;
#endif

    void Start();
    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    Editor_EffectContainer::EFFECT_EDIT_CONTEXT* m_pContext = nullptr;
    EffectType m_eEffectType = EffectType::Count;
    void SetType();

    /*----sprite effect----*/
    void SpriteEffectSetUp();
    void AddCurrTexture();
    void ReplaceMaterial();
    static _uint m_iMaterialIndex;
    _float m_fSpeed{};
    _int m_iMaxFrameIndex{};
    _bool m_IsRepeat{};
    _bool m_IsAnimated{};
    Material* m_pSpriteMaterial = nullptr;

};

NS_END