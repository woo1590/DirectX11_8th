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
#include "NavigationComponent.h"

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
	m_UseResolve = desc->useResolve;

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

	D3D11_DEPTH_STENCIL_DESC dsDesc{};
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	if (FAILED(m_pDevice->CreateDepthStencilState(&dsDesc, &m_pDSState)))
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
	if (!m_UseResolve && !other->m_UseResolve)
		return;

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
		_float3 overlap{};
		_float3 deltaMove{};

		overlap.x = (std::min)(max.x, otherMax.x) - (std::max)(min.x, otherMin.x);
		overlap.y = (std::min)(max.y, otherMax.y) - (std::max)(min.y, otherMin.y);
		overlap.z = (std::min)(max.z, otherMax.z) - (std::max)(min.z, otherMin.z);

		if (overlap.x <= overlap.y && overlap.x <= overlap.z)
		{
			if (min.x < otherMin.x)
				normal = XMVectorSet(-1.f, 0.f, 0.f, 0.f);
			else
				normal = XMVectorSet(1.f, 0.f, 0.f, 0.f);
		}
		else if (overlap.y <= overlap.x && overlap.y <= overlap.z)
		{
			if (min.y < otherMin.y)
				normal = XMVectorSet(0.f, -1.f, 0.f, 0.f);
			else
				normal = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		}
		else
		{
			if (min.z < otherMin.z)
				normal = XMVectorSet(0.f, 0.f, -1.f, 0.f);
			else
				normal = XMVectorSet(0.f, 0.f, 1.f, 0.f);
		}

		XMStoreFloat3(&deltaMove, XMLoadFloat3(&overlap) * normal);

		auto nav = m_pOwner->GetComponent<NavigationComponent>();
		auto otherNav = other->m_pOwner->GetComponent<NavigationComponent>();
		auto transform = m_pOwner->GetComponent<TransformComponent>();
		auto otherTransform = other->m_pOwner->GetComponent<TransformComponent>();


		if (m_UseResolve && other->m_UseResolve)
		{
			XMStoreFloat3(&deltaMove, XMLoadFloat3(&deltaMove) * 0.5f);

			if (nav)
				nav->MoveTo(deltaMove);
			else
				transform->Translate(XMLoadFloat3(&deltaMove));

			XMStoreFloat3(&deltaMove, XMLoadFloat3(&deltaMove) * -1.f);
			if (otherNav)
				otherNav->MoveTo(deltaMove);
			else
				otherTransform->Translate(XMLoadFloat3(&deltaMove));
		}
		else if (m_UseResolve)
		{
			if (nav)
				nav->MoveTo(deltaMove);
			else
				transform->Translate(XMLoadFloat3(&deltaMove));
		}
		else
		{
			XMStoreFloat3(&deltaMove, XMLoadFloat3(&deltaMove) * -1.f);
			if (otherNav)
				otherNav->MoveTo(deltaMove);
			else
				otherTransform->Translate(XMLoadFloat3(&deltaMove));
		}
	}
	else if (m_ColliderType == ColliderType::Sphere && other->m_ColliderType == ColliderType::Sphere)
	{
		BoundingSphere boundingSphere = static_cast<Bounding_Sphere*>(m_pBounding)->GetWorldDesc();
		BoundingSphere otherBoundingSphere = static_cast<Bounding_Sphere*>(other->m_pBounding)->GetWorldDesc();

		_float3 centerA = boundingSphere.Center;
		_float3 centerB = otherBoundingSphere.Center;
		
		_float distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&centerB) - XMLoadFloat3(&centerA)));
		_float maxDistance = boundingSphere.Radius + otherBoundingSphere.Radius;

		_float penetration = maxDistance - distance;
		_vector dir = XMVector3Normalize(XMLoadFloat3(&centerA) - XMLoadFloat3(&centerB));
		_float3 deltaMove{};
		XMStoreFloat3(&deltaMove, dir * penetration * 0.5f);

		auto nav = m_pOwner->GetComponent<NavigationComponent>();
		auto otherNav = other->m_pOwner->GetComponent<NavigationComponent>();
		auto transform = m_pOwner->GetComponent<TransformComponent>();
		auto otherTransform = other->m_pOwner->GetComponent<TransformComponent>();

		if (nav)
			nav->MoveTo(deltaMove);
		else
			transform->Translate(XMLoadFloat3(&deltaMove));

		XMStoreFloat3(&deltaMove, XMLoadFloat3(&deltaMove) * -1.f);
		if (otherNav)
			otherNav->MoveTo(deltaMove);
		else
			otherTransform->Translate(XMLoadFloat3(&deltaMove));
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
	
	m_pDeviceContext->GSSetShader(nullptr, nullptr, NULL);
	m_pDeviceContext->OMSetDepthStencilState(m_pDSState,0);
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