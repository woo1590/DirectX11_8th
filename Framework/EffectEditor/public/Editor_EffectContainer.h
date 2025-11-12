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
        _string textureTag{};
        Texture* texture = nullptr;
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
    std::unordered_map<_string, Texture*> m_Textures;
    EFFECT_EDIT_CONTEXT m_Context{};

#ifdef USE_IMGUI
    void Play();
    void Import(const _string& filePath);
    void Export(const _string& outFilePath);
    void LoadTextureFromDirectory(const _string& dirPath);
    void DisplayTextures();
    void AddNode();
    void RemoveNode();
#endif
};

NS_END