#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class SpriteComponent;
NS_END

NS_BEGIN(Client)

class CountNumber :
    public UIObject
{
    enum class Digit { Hundreads, Tens, Units, Count };
public:
    typedef struct tagCountNumberDesc : public UIObject::UIOBJECT_DESC
    {
        _float4 color{};
    }COUNT_NUMBER_DESC;
private:
    CountNumber();
    CountNumber(const CountNumber& prototype);
    virtual ~CountNumber() = default;

public:
    static CountNumber* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)override;

    void SetActive(_bool active);
    void SetNumber(_uint number);
    void Bounce();

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _bool m_UseBounce = false;
    _float m_fElapsedTime = 0.f;
    _float m_fDuration = 0.1f;
    _float3 m_OriginScale{};
    _float3 m_TargetScale{};

    std::vector<SpriteComponent*> m_Numbers;

};

NS_END