#pragma once
#include "Object.h"

NS_BEGIN(Client)

class BossPillar :
    public Object
{
private:
    BossPillar();
    BossPillar(const BossPillar& prototype);
    virtual ~BossPillar() = default;

public:
    static BossPillar* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)override;

    void OnCollisionEnter(ColliderComponent* otherCollider)override;

    void Explode();

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _uint m_iResistantCount = 3;
    Object* m_pBossPillarSmoke = nullptr;

    class BossPillarSpawn : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float m_fDelayDuration = 0.05f;
        _float m_fElapsedTime = 0.f;
        _float m_fDuration = 0.05f;
        _float3 m_StartPosition{};
        _float3 m_EndPosition{};
        _bool m_IsSpawned = false;
    };
    class BossPillarIdle : public State
    {
        void Enter(Object* object)override {};
        void Update(Object* object, _float dt)override {};
        void TestForExit(Object* object)override {};
    };
    class BossPillarExlplode : public State
    {
        void Enter(Object* object)override;
        void Update(Object* object, _float dt)override;
        void TestForExit(Object* object)override;

        _float3 m_StartPosition{};
        _float m_fElapsedTime = 0.f;
        _float m_fDuration = 0.f;
        _float m_fShakePower = 1.f;
    };

    BossPillarSpawn m_BossPillarSpawn;
    BossPillarIdle m_BossPillarIdle;
    BossPillarExlplode m_BossPillarExplode;
};

NS_END