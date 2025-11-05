#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class TransformComponent;
class RigidBodyComponent;
class NavigationSystem;
class ENGINE_DLL NavigationComponent :
    public Component
{
private:
    NavigationComponent(Object* owner);
    NavigationComponent(const NavigationComponent& prototype);
    virtual ~NavigationComponent() = default;

public:
    static NavigationComponent* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void AttachTransform();
    void AttachRigidBody();
    void AttachSystem(NavigationSystem* system);

    void SetMoveSpeed(_float speed) { m_fMoveSpeed = speed; }
    void SetArriveRange(_float range) { m_fArriveRange = range; }
    void SetCurrCellIndex(_uint cellIndex);
    _uint GetCurrCellIndex()const { return m_iCurrCellIndex; }

    /*API*/
    void FindPath(_float3 startPosition, _uint startCellIndex, _float3 targetPosition, _uint targetCellIndex);
    void SpawnInCell(_uint cellIndex);
    void MoveTo(_float3 deltaPosition);
    void MoveByVelocity(_float dt);
    void MoveByPath(_float dt);

    Component* Clone()override { return new NavigationComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override;
#endif

private:
    _float3 GetCurrCellNormal();
    _bool IsLinkedCell(_float3 position);
    _bool IsMove(_float3 position);
    _float GetHeight(_float3 position);
    _float3 MakeSlideVector(_float3 position, _float3 nextPosition);

    _bool m_IsInLink = false;
    _int m_iCurrLinkedCellIndex = -1;
    _uint m_iCurrCellIndex{};

    /*astar*/
    ASTAR_RESULT m_CurrPath{};
    _uint m_iCurrPathCursor;
    _float m_fMoveSpeed{};
    _float m_fArriveRange{};

    NavigationSystem* m_pNavigationSystem = nullptr;
    TransformComponent* m_pTransform = nullptr;
    RigidBodyComponent* m_pRigidBody = nullptr;
};

NS_END