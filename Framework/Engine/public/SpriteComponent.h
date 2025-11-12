#pragma once
#include "Component.h"
#include "RenderProxy.h"

NS_BEGIN(Engine)

class Material;
class VIBuffer;
class MaterialInstance;
class TransformComponent;
class ENGINE_DLL SpriteComponent :
    public Component
{
public:
    typedef struct tagSpriteDesc : public InitDESC
    {
        _bool useBillboard = false;
        _bool isAnimated{};
        _bool isRepeat{};
        _uint iMaxFrameIndex{};
        _float fSpeed{};

    }SPRITE_DESC;
private:
    SpriteComponent(Object* owner);
    SpriteComponent(const SpriteComponent& prototype);
    virtual ~SpriteComponent() = default;

public:
    static SpriteComponent* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void Update(_float dt)override;

    void SetBuffer(_uint levelID, const _string& key);
    void SetMaterial(_uint levelID, const _string& key);

    HRESULT ExtractRenderProxy(TransformComponent* transform, std::vector<RenderProxy>& proxies);
    _bool IsFinished()const { return m_isEnd; }

    /*Getter*/
    MaterialInstance* GetMaterialInstance()const { return m_pMaterialInstance; }

    /*setter*/
    void SetFrameIndex(_uint index) { m_iCurrFrameIndex = index; m_isEnd = false; }

    Component* Clone() { return new SpriteComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override;
#endif

private:
    RenderGroup m_eGroup{};
    _bool m_UseBillBoard = false;

    VIBuffer* m_pBuffer = nullptr;
    Material* m_pMaterial = nullptr;
    MaterialInstance* m_pMaterialInstance = nullptr;

    _bool m_isAnimated = false;
    _bool m_isRepeat = false;
    _bool m_isEnd = false;

    _uint m_iMaxFrameIndex{};
    _uint m_iCurrFrameIndex{};
    _float m_fElapsedTime{};
    _float m_fSpeed{};
};

NS_END