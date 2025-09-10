#pragma once
#include "Component.h"
#include "LightProxy.h"

NS_BEGIN(Engine)

class ENGINE_DLL LightComponent :
    public Component
{
public:
    typedef struct tagLightDesc : public InitDESC
    {
        LightType type{};

        _float4 color{};

        //directional light
        _float4 direction{};

        //point, spot light
        _float range{};

    }LIGHT_DESC;
private:
    LightComponent(Object* owner);
    LightComponent(const LightComponent& prototype);
    virtual ~LightComponent() = default;

public:
    static LightComponent* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void Update(_float dt)override {};  //아마 타겟같은거 넣어주고 거리에따라 온오프? 모르겠네 이건 오브젝트에서 처리하는게 나을까?

    HRESULT ExtractLightProxy(std::vector<LightProxy>& lights);

    _bool IsActive()const { return m_isActive; }
    void SetActive() { m_isActive = true; }
    void SetDeactive() { m_isActive = false; }

    Component* Clone() { return new LightComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override;
#endif

private:
    LightType m_eType{};

    _float4 m_Color{};
    _float4 m_Direction{};
    _float m_fRange{};

    _bool m_isActive = true;
};

NS_END