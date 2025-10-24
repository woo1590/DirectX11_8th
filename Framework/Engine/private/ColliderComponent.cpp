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
	m_IsCollision = m_pBounding->Instersect(other->m_pBounding);
	other->m_IsCollision = m_IsCollision;

	return m_IsCollision;
}

_bool ColliderComponent::Intersect(RAY worldRay)
{
	return false;
	//m_IsCollision = m_pBounding->Instersect(worldRay);
}

void ColliderComponent::OnCollisionEnter(ColliderComponent* collider, ColliderComponent* otherCollider)
{
	m_pOwner->OnCollisionEnter(collider, otherCollider);
}

void ColliderComponent::OnCollisionStay(ColliderComponent* collider, ColliderComponent* otherCollider)
{
	m_pOwner->OnCollisionStay(collider, otherCollider);
}

void ColliderComponent::OnCollisionExit(ColliderComponent* collider, ColliderComponent* otherCollider)
{
	m_pOwner->OnCollisionExit(collider, otherCollider);
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