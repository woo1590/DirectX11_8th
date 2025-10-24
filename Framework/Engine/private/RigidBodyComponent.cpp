#include "EnginePCH.h"
#include "RigidBodyComponent.h"
#include "Object.h"
#include "TransformComponent.h"

RigidBodyComponent::RigidBodyComponent(Object* owner)
	:Component(owner)
{
}

RigidBodyComponent::RigidBodyComponent(const RigidBodyComponent& prototype)
	:Component(prototype)
{
}

RigidBodyComponent* RigidBodyComponent::Create(Object* owner)
{
	RigidBodyComponent* Instance = new RigidBodyComponent(owner);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT RigidBodyComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT RigidBodyComponent::Initialize(InitDESC* arg)
{
	return S_OK;
}

void RigidBodyComponent::Update(_float dt)
{
	__super::Update(dt);
	
	if (!m_IsGround)
		m_Velocity.y -= 200.f * dt;
}

void RigidBodyComponent::AddForce(_float3 force)
{
	XMStoreFloat3(&m_Velocity, XMLoadFloat3(&force) + XMLoadFloat3(&m_Velocity));
}

void RigidBodyComponent::SetGround(_bool isGrounded)
{
	m_IsGround = isGrounded;

	if (m_IsGround)
		m_Velocity.y = 0.f;
}

void RigidBodyComponent::Free()
{
	__super::Free();
}
