#include "pch.h"
#include "StatusComponent.h"

StatusComponent::StatusComponent(Object* owner)
    :Component(owner)
{
}

StatusComponent::StatusComponent(const StatusComponent& prototype)
    :Component(prototype)
{
}

StatusComponent* StatusComponent::Create(Object* owner)
{
    StatusComponent* Instance = new StatusComponent(owner);

    if (FAILED(Instance->Initialize_Prototype()))
        Safe_Release(Instance);

    return Instance;
}

HRESULT StatusComponent::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT StatusComponent::Initialize(InitDESC* arg)
{
    STATUS_DESC* desc = static_cast<STATUS_DESC*>(arg);
    m_iHP = desc->hp;
    m_iShield = desc->shield;
    m_iAttackPower = desc->attackPower;
    m_fMoveSpeed = desc->speed;

    if (FAILED(__super::Initialize(arg)))
        return E_FAIL;

    return S_OK;
}

void StatusComponent::BeAttacked(_uint power)
{
    m_iHP -= power;

    if (m_iHP < 0)
        m_iHP = 0;
}

StatusComponent::STATUS_DESC StatusComponent::GetDesc() const
{
    StatusComponent::STATUS_DESC desc{};
    desc.hp = m_iHP;
    desc.shield = m_iShield;
    desc.attackPower = m_iAttackPower;
    desc.speed = m_fMoveSpeed;

    return desc;
}

void StatusComponent::Free()
{
    __super::Free();
}
