#pragma once
#include "Object.h"

NS_BEGIN(Engine)
class SpriteComponent;
NS_END

NS_BEGIN(Client)

class DamageFont :
    public Object
{
public:
    typedef struct tagDamageFontDesc : public Object::OBJECT_DESC
    {
        _float4 color{};
        _float fontSize{};
        _uint number{};
    }DAMAGE_FONT_DESC;
private:
    enum class Digit { Thousands, Hundreads, Tens, Units, Count };
    DamageFont();
    DamageFont(const DamageFont& prototype);
    virtual ~DamageFont() = default;

public:
    static DamageFont* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)override;

    void SetNumber(_uint number);

    Object* Clone(InitDESC* arg)override;
    void Free()override;
    
private:
    void SetNumberScale(_float scale);

    std::vector<SpriteComponent*> m_Numbers;
    _uint m_iNumber{};
    _float m_fFontSize{};

    class DamageFontSpawn : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fElapsedTime = 0.f;
        _float m_fDuration = 0.3f;
        _float3 m_StartPosition{};
        _float3 m_TargetPosition{};
        _float m_fStartScale{};
        _float m_fTargetScale{};
    };
    class DamageFontDead : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fElapsedTime = 0.f;
        _float m_fDuration = 0.5f;
    };

    DamageFontSpawn m_DamageFontSpawn;
    DamageFontDead m_DamageFontDead;
};

NS_END