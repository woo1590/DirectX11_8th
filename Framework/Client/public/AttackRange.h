#pragma once
#include "Object.h"

NS_BEGIN(Engine)
class TransformComponent;
class SpriteComponent;
NS_END

NS_BEGIN(Client)

class AttackRange :
    public Object
{
public:
    typedef struct tagAttackRangeDesc : public Object::OBJECT_DESC
    {
        _float lifeDuration{};
        _float attackRange{};
    }ATTACK_RANGE_DESC;
private:
    AttackRange();
    AttackRange(const AttackRange& prototype);
    virtual ~AttackRange() = default;

public:
    static AttackRange* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies);

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    TransformComponent* m_pInnerCircleTransform = nullptr;
    SpriteComponent* m_pInnerCircleSprite = nullptr;

    _float m_fLifeElapsedTime{};
    _float m_fLifeDuration{};

    _float m_fElapsedTime{};
    _float m_fDuration = 0.4f;
    _float m_fStartRange = 1.f;
    _float m_fTargetRange{};
};

NS_END