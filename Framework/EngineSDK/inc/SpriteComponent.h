#pragma once
#include "Component.h"
#include "RenderProxy.h"

NS_BEGIN(Engine)

class Material;
class VIBuffer;
class TransformComponent;
class ENGINE_DLL SpriteComponent :
    public Component
{
public:
    typedef struct tagSpriteDesc : public InitDESC
    {
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
    Component* Clone() { return new SpriteComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override;
#endif

private:
    RenderGroup m_eGroup{};

    VIBuffer* m_pBuffer = nullptr;
    Material* m_pMaterial = nullptr;

    _bool m_isAnimated = false;
    _bool m_isRepeat = false;
    _bool m_isEnd = false;

    _uint m_iMaxFrameIndex{};
    _uint m_iCurrFrameIndex{};
    _float m_fElapsedTime{};
    _float m_fSpeed{};
};

NS_END