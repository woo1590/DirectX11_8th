#include "EnginePCH.h"
#include "ColliderComponent.h"
#include "EngineCore.h"
#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"
#include "Object.h"

//component
#include "TransformComponent.h"
#include "ModelComponent.h"

ColliderComponent::ColliderComponent(Object* owner)
	:Component(owner)
{
}

ColliderComponent::ColliderComponent(const ColliderComponent& prototype)
	:Component(prototype)
{
}

ColliderComponent* ColliderComponent::Create(Object* owner)
{
	ColliderComponent* Instance = new ColliderComponent(owner);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT ColliderComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT ColliderComponent::Initialize(InitDESC* arg)
{
	Bounding::BOUNDING_DESC* desc = static_cast<Bounding::BOUNDING_DESC*>(arg);

	switch (desc->type)
	{
	case ColliderType::AABB:
		m_pBounding = Bounding_AABB::Create(desc);
		break;
	case ColliderType::OBB:
		m_pBounding = Bounding_OBB::Create(desc);
		break;
	case ColliderType::Sphere:
		m_pBounding = Bounding_Sphere::Create(desc);
		break;
	default:
		break;
	}
	m_ColliderType = desc->type;
	m_iColliderFilter = desc->colliderFilter;

	if (!m_pBounding)
	{
		MSG_BOX("Failed to create : Bounding");
		return E_FAIL;
	}

	auto engine = EngineCore::GetInstance();

	m_pDevice = engine->GetDevice();
	m_pDevice->AddRef();

	m_pDeviceContext = engine->GetDeviceContext();
	m_pDeviceContext->AddRef();

	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pDeviceContext);
	m_pEffect = new BasicEffect(m_pDevice);

	m_pEffect->SetVertexColorEnabled(true);

	const void* shaderByteCode = nullptr;
	size_t shaderByteCodeLength = 0;

	m_pEffect->GetVertexShaderBytecode(&shaderByteCode, &shaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount,
		shaderByteCode, shaderByteCodeLength,
		&m_pInputLayout)))
		return E_FAIL;

	return S_OK;
}

void ColliderComponent::Update(_float dt)
{
	_float4x4 worldMatrix = m_pOwner->GetComponent<TransformComponent>()->GetWorldMatrix();
	if (-1 != m_iBoneIndex)
	{
		auto model = m_pOwner->GetComponent<ModelComponent>();
		_float4x4 boneMatrix = model->GetCombinedMatrixByIndex(m_iBoneIndex);

		XMStoreFloat4x4(&worldMatrix, XMLoadFloat4x4(&boneMatrix) * XMLoadFloat4x4(&worldMatrix));
	}

	m_pBounding->Update(worldMatrix);
}

_bool ColliderComponent::Intersect(ColliderComponent* other)
{
	m_IsCollision = m_pBounding->Intersect(other->m_pBounding);
	other->m_IsCollision = m_IsCollision;

	return m_IsCollision;
}

RAYCAST_DATA ColliderComponent::RayCast(RAY worldRay)
{
	RAYCAST_DATA data = m_pBounding->RayCast(worldRay);

	if (data.isHit)
		data.hitObject = m_pOwner;

	return data;
}

void ColliderComponent::ResolveCollision(ColliderComponent* other)
{
	if (m_ColliderType == ColliderType::AABB && other->m_ColliderType == ColliderType::AABB)
	{
		BoundingBox boudingBox = static_cast<Bounding_AABB*>(m_pBounding)->GetWorldDesc();
		BoundingBox otherBoundingBox = static_cast<Bounding_AABB*>(other->m_pBounding)->GetWorldDesc();

		_float3 min{}, max{}, otherMin{}, otherMax{};
		XMStoreFloat3(&min, XMLoadFloat3(&boudingBox.Center) - XMLoadFloat3(&boudingBox.Extents));
		XMStoreFloat3(&max, XMLoadFloat3(&boudingBox.Center) + XMLoadFloat3(&boudingBox.Extents));
		XMStoreFloat3(&otherMin, XMLoadFloat3(&otherBoundingBox.Center) - XMLoadFloat3(&otherBoundingBox.Extents));
		XMStoreFloat3(&otherMax, XMLoadFloat3(&otherBoundingBox.Center) + XMLoadFloat3(&otherBoundingBox.Extents));

		_vector normal = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		_float overlapX = (std::min)(max.x, otherMax.x) - (std::max)(min.x, otherMin.x);
		_float overlapY = (std::min)(max.y, otherMax.y) - (std::max)(min.y, otherMin.y);
		_float overlapZ = (std::min)(max.z, otherMax.z) - (std::max)(min.z, otherMin.z);

		if (overlapX <= overlapY && overlapX <= overlapZ)
		{
			if (min.x < otherMin.x)
				normal = XMVectorSet(-1.f, 0.f, 0.f, 0.f);
			else
				normal = XMVectorSet(1.f, 0.f, 0.f, 0.f);

			auto transform = m_pOwner->GetComponent<TransformComponent>();
			auto otherTransform = other->m_pOwner->GetComponent<TransformComponent>();

			transform->Translate(normal * 0.5f * overlapX);
			otherTransform->Translate(-1.f * normal * 0.f * overlapX);
		}
		else if (overlapY <= overlapX && overlapY <= overlapZ)
		{
			if (min.y < otherMin.y)
				normal = XMVectorSet(0.f, -1.f, 0.f, 0.f);
			else
				normal = XMVectorSet(0.f, 1.f, 0.f, 0.f);

			auto transform = m_pOwner->GetComponent<TransformComponent>();
			auto otherTransform = other->m_pOwner->GetComponent<TransformComponent>();

			transform->Translate(normal * 0.5f * overlapY);
			otherTransform->Translate(-1.f * normal * 0.f * overlapY);
		}
		else
		{
			if (min.z < otherMin.z)
				normal = XMVectorSet(0.f, 0.f, -1.f, 0.f);
			else
				normal = XMVectorSet(0.f, 0.f, 1.f, 0.f);

			auto transform = m_pOwner->GetComponent<TransformComponent>();
			auto otherTransform = other->m_pOwner->GetComponent<TransformComponent>();

			transform->Translate(normal * 0.5f * overlapZ);
			otherTransform->Translate(-1.f * normal * 0.f * overlapZ);
		}
	}
	else if (m_ColliderType == ColliderType::Sphere && other->m_ColliderType == ColliderType::Sphere)
	{
		BoundingSphere boundingSphere = static_cast<Bounding_Sphere*>(m_pBounding)->GetWorldDesc();
		BoundingSphere otherBoundingSphere = static_cast<Bounding_Sphere*>(other->m_pBounding)->GetWorldDesc();
	}
	else
		return;
}

void ColliderComponent::OnCollisionEnter(ColliderComponent* otherCollider)
{
	m_pOwner->OnCollisionEnter(otherCollider);
}

void ColliderComponent::OnCollisionStay(ColliderComponent* otherCollider)
{
	m_pOwner->OnCollisionStay(otherCollider);
}

void ColliderComponent::OnCollisionExit(ColliderComponent* otherCollider)
{
	m_pOwner->OnCollisionExit(otherCollider);
}

void ColliderComponent::Draw()
{
	CAMERA_CONTEXT camContext = EngineCore::GetInstance()->GetCameraContext();
	_float4 color;
	m_IsCollision ? color = _float4(1.f, 0.f, 0.f, 1.f) : color = _float4(0.f, 1.f, 0.f, 1.f);

	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(XMLoadFloat4x4(&camContext.viewMatrix));
	m_pEffect->SetProjection(XMLoadFloat4x4(&camContext.projMatrix));
	m_pEffect->Apply(m_pDeviceContext);

	m_pDeviceContext->IASetInputLayout(m_pInputLayout);

	m_pBatch->Begin();
	m_pBounding->Draw(m_pBatch,color);
	m_pBatch->End();
}

void ColliderComponent::Free()
{
	__super::Free();

	Safe_Release(m_pBounding);

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pInputLayout);

	Safe_Delete(m_pBatch);
	Safe_Delete(m_pEffect);
}
#ifdef USE_IMGUI
void ColliderComponent::RenderInspector()
{
	ImGui::PushID(this);

	if (!ImGui::CollapsingHeader("Collider",
		ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
		ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding))
	{
		ImGui::Separator();
		
		m_pBounding->RenderInspector();
	}
	ImGui::PopID();
}
#endif