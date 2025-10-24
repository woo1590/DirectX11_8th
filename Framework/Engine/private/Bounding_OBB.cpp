#include "EnginePCH.h"
#include "Bounding_OBB.h"
#include "Bounding_AABB.h"
#include "Bounding_Sphere.h"

Bounding_OBB::Bounding_OBB()
{
}

Bounding_OBB::~Bounding_OBB()
{
}

Bounding_OBB* Bounding_OBB::Create(BOUNDING_DESC* arg)
{
	Bounding_OBB* Instance = new Bounding_OBB();

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Bounding_OBB::Initialize(BOUNDING_DESC* arg)
{
	OBB_DESC* desc = static_cast<OBB_DESC*>(arg);

	m_LocalDesc.Center = desc->center;
	m_LocalDesc.Extents = desc->halfSize;
	XMStoreFloat4(&m_LocalDesc.Orientation, XMQuaternionRotationRollPitchYaw(desc->rotation.x, desc->rotation.y, desc->rotation.z));

	return S_OK;
}

void Bounding_OBB::Update(_float4x4 worldMatrix)
{
	m_LocalDesc.Transform(m_WorldDesc, XMLoadFloat4x4(&worldMatrix));
}

_bool Bounding_OBB::Intersect(Bounding* other)
{
	return other->IntersectToOBB(this);
}

_bool Bounding_OBB::IntersectToAABB(Bounding_AABB* other)
{
	return m_WorldDesc.Intersects(other->GetWorldDesc());
}

_bool Bounding_OBB::IntersectToOBB(Bounding_OBB* other)
{
	return m_WorldDesc.Intersects(other->GetWorldDesc());
}

_bool Bounding_OBB::IntersectToSphere(Bounding_Sphere* other)
{
	return m_WorldDesc.Intersects(other->GetWorldDesc());
}


void Bounding_OBB::Draw(PrimitiveBatch<VertexPositionColor>* batch, _float4 color)
{
	DX::Draw(batch, m_WorldDesc, XMLoadFloat4(&color));
}

void Bounding_OBB::Free()
{
	__super::Free();
}

#ifdef USE_IMGUI
void Bounding_OBB::RenderInspector()
{
	ImGui::SeparatorText("Bounding OBB");

	ImGui::DragFloat3("Local Center : ", &m_LocalDesc.Center.x, 0.1f, -FLT_MAX, FLT_MAX);
	ImGui::DragFloat3("Local Half : ", &m_LocalDesc.Extents.x, 0.1f, -FLT_MAX, FLT_MAX);
}
#endif
