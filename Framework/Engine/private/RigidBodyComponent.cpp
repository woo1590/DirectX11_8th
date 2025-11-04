#include "EnginePCH.h"
#include "RigidBodyComponent.h"
#include "Object.h"

//component
#include "TransformComponent.h"
#include "ColliderComponent.h"

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
	RIGID_BODY_DESC* desc = static_cast<RIGID_BODY_DESC*>(arg);

	m_fInverseMass = desc->inverseMass;
	m_fLinearDamping = desc->linearDamping;
	m_fAngularDamping = desc->angularDamping;
	m_fMass = desc->mass;
	m_fInverseMass = 1.f / m_fMass;

	return S_OK;
}

void RigidBodyComponent::Update(_float dt)
{
	__super::Update(dt);

	if (m_UseIntegrate)
	{
		if (!m_IsGround)
			m_Acceleration = _float3{ 0.f,-200.f,0.f };

		Integrate(dt);
	}
	else
	{
		if (!m_IsGround)
			m_Velocity.y -= 200.f * dt;
	}
}

void RigidBodyComponent::AddForce(_float3 force)
{
	XMStoreFloat3(&m_Velocity, XMLoadFloat3(&force) + XMLoadFloat3(&m_Velocity));
}

void RigidBodyComponent::AddAngularImpulse(_float3 impulse)
{
	XMStoreFloat3(&m_AngularVelocity, XMLoadFloat3(&m_AngularVelocity) + XMLoadFloat3(&impulse) * m_fInverseMass);
}

void RigidBodyComponent::AddImpulse(_float3 impulse)
{
	if (m_UseIntegrate)
		XMStoreFloat3(&m_Velocity, XMLoadFloat3(&m_Velocity) + XMLoadFloat3(&impulse) * m_fInverseMass);
}

void RigidBodyComponent::SetInertiaTensor(_float halfSize)
{
	XMStoreFloat3x3(&m_InertiaTensor, XMMatrixIdentity());

	m_InertiaTensor._11 = 1.f / 12.f * m_fMass * (std::powf(halfSize * 2.f, 2.f) + std::powf(halfSize * 2.f, 2.f));
	m_InertiaTensor._22 = 1.f / 12.f * m_fMass * (std::powf(halfSize * 2.f, 2.f) + std::powf(halfSize * 2.f, 2.f));
	m_InertiaTensor._33 = 1.f / 12.f * m_fMass * (std::powf(halfSize * 2.f, 2.f) + std::powf(halfSize * 2.f, 2.f));

	XMStoreFloat3x3(&m_InvInertiaTensor, XMMatrixInverse(nullptr, XMLoadFloat3x3(&m_InertiaTensor)));
}

void RigidBodyComponent::SetGround(_bool isGrounded)
{
	m_IsGround = isGrounded;

	if (m_IsGround)
	{
		if(!m_UseIntegrate)
			m_Velocity.y = 0.f;
	}
}

void RigidBodyComponent::Free()
{
	__super::Free();
}

#ifdef USE_IMGUI
void RigidBodyComponent::RenderInspector()
{
	ImGui::PushID(this);

	if (!ImGui::CollapsingHeader("RigidBody",
		ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
		ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding))
	{
		ImGui::Separator();

		m_IsGround ? ImGui::Text("On Ground : True") : ImGui::Text("On Ground : False");
		ImGui::DragFloat3("Velocity : ", &m_Velocity.x);
	}

	ImGui::PopID();
}
#endif

void RigidBodyComponent::Integrate(_float dt)
{
	auto transform = m_pOwner->GetComponent<TransformComponent>();
	
	m_LastFrameAcceleration = m_Acceleration;
	XMStoreFloat3(&m_LastFrameAcceleration, XMLoadFloat3(&m_LastFrameAcceleration) + XMLoadFloat3(&m_ForceAccum) * m_fInverseMass);
	
	_float3 angularAcceleration{};
	XMStoreFloat3(&angularAcceleration, XMVector3TransformNormal(XMLoadFloat3(&m_TorqueAccum), XMLoadFloat3x3(&m_InertiaTensor)));

	XMStoreFloat3(&m_Velocity, XMLoadFloat3(&m_Velocity) + XMLoadFloat3(&m_LastFrameAcceleration) * dt);
	XMStoreFloat3(&m_AngularVelocity, XMLoadFloat3(&m_AngularVelocity) + XMLoadFloat3(&angularAcceleration) * dt);

	XMStoreFloat3(&m_Velocity, XMLoadFloat3(&m_Velocity) * std::powf(m_fLinearDamping, dt));
	XMStoreFloat3(&m_AngularVelocity, XMLoadFloat3(&m_AngularVelocity) * std::powf(m_fAngularDamping, dt));

	_float4 rot = transform->GetQuaternion();
	_vector orientation = XMLoadFloat4(&rot);
	_vector deltaRot = XMVectorSet(m_AngularVelocity.x, m_AngularVelocity.y, m_AngularVelocity.z, 0.f) * dt;
	
	deltaRot = XMQuaternionMultiply(orientation, deltaRot) * 0.5f;
	XMStoreFloat4(&rot, XMQuaternionNormalize(XMVectorAdd(orientation, deltaRot)));
	
	transform->SetQuaternion(rot);
}
