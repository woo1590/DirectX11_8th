#pragma once
#include "ContainerObject.h"
#include "EffectContainer.h"

NS_BEGIN(Engine)
class ModelComponent;
NS_END

NS_BEGIN(Client)

class EnemyHpPanel;
class Enemy abstract:
    public ContainerObject
{
protected:
    Enemy();
    Enemy(const Enemy& prototype);
    virtual ~Enemy() = default;

public:
    virtual HRESULT Initialize(InitDESC* arg)override;
    virtual HRESULT LateInitialize()override;
    void PriorityUpdate(_float dt)override;
    virtual void Update(_float dt)override;
    virtual void LateUpdate(_float dt)override;
    virtual HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)override;

    virtual void OnCollisionEnter(ColliderComponent* otherCollider)override;
    virtual void LockOn() { m_IsLockOn = true; }

    virtual Object* Clone(InitDESC* arg) = 0;
    virtual void Free()override;
    virtual void SetDead()override;

protected:
    static _uint m_iNextEnemyID;
    _uint m_iEnemyID{};
    _float m_fElapsedTime = 0.f;
    _float m_fHitDelay = 2.f;
    _uint m_iHpPanelBoneIndex{};

    ModelComponent* m_pOutLineModel = nullptr;
    _bool m_IsLockOn = false;
};

NS_END