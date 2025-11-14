#pragma once
#include "ContainerObject.h"

NS_BEGIN(Engine)
class Texture;
NS_END

NS_BEGIN(EffectEditor)

class Editor_EffectContainer :
    public ContainerObject
{
public:
    typedef struct tagEffectContainerDesc : public ContainerObject::CONTAINER_OBJECT_DESC
    {

    }EFFECT_CONTAINER_DESC;

    typedef struct tagEffectEditContext
    {
        std::vector<_string> textureTags{};
        std::vector<Texture*> textures;
        std::map<_string, Texture*>* pAllTextures;
    }EFFECT_EDIT_CONTEXT;
private:
    Editor_EffectContainer();
    Editor_EffectContainer(const Editor_EffectContainer& prototype);
    virtual ~Editor_EffectContainer() = default;

public:
    static Editor_EffectContainer* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

#ifdef USE_IMGUI
    void RenderInspector()override;
#endif

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    std::vector<_int> m_SelectIndices;
    std::map<_string, Texture*> m_Textures;
    EFFECT_EDIT_CONTEXT m_Context{};

    _string m_strEffectName{};
    _float m_fDuration{};

#ifdef USE_IMGUI
    void Play();
    void Import();
    void Export();
    void LoadTextureFromDirectory(const _string& dirPath);
    void DisplayTextures();
    void AddNode();
    void RemoveNode();
    void ContextClear();
#endif
};

NS_END