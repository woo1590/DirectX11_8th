#pragma once
#include "ContainerObject.h"

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
    virtual void Update(_float dt)override;

    virtual Object* Clone(InitDESC* arg) = 0;
    virtual void Free()override;
    virtual void SetDead()override;

protected:
    static _uint m_iNextEnemyID;
    _uint m_iEnemyID{};
    _float m_fElapsedTime = 0.f;
    _float m_fHitDelay = 2.f;
    _uint m_iHpPanelBoneIndex{};
};

NS_END