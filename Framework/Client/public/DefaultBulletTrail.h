#pragma once
#include "Object.h"

NS_BEGIN(Client)

class DefaultBulletTrail :
    public Object
{
public:
    typedef struct tagDefaultBulletTrailDesc : public Object::OBJECT_DESC
    {
        _string mtrlTag = "Mtrl_TrailBulletRed";
        _float3 startPosition{};
    }DEFAULT_BULLET_TRAIL_DESC;
private:
    DefaultBulletTrail();
    DefaultBulletTrail(const DefaultBulletTrail& prototype);
    virtual ~DefaultBulletTrail() = default;

public:
    static DefaultBulletTrail* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void AddNextPosition(_float3 position);

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _float3 m_PrevPoint{};
    _float3 m_CurrPoint{};

    _bool m_IsActive = false;
    _float m_fElapsedTime{};
    _float m_fDuration = 0.02f;
};

NS_END