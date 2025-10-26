#include "EnginePCH.h"
#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"

Bounding_AABB::Bounding_AABB()
	:Bounding()
{
}

void Bounding_AABB::Update(_float4x4 worldMatrix)
{
	_matrix world = XMLoadFloat4x4(&worldMatrix);
	_matrix transformMatrix;
	transformMatrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVector3Length(world.r[0]);
	transformMatrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVector3Length(world.r[1]);
	transformMatrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVector3Length(world.r[2]);
	transformMatrix.r[3] = world.r[3];

	m_LocalDesc.Transform(m_WorldDesc, transformMatrix);
}

_bool Bounding_AABB::Intersect(Bounding* other)
{
	return other->IntersectToAABB(this);
}

RAYCAST_DATA Bounding_AABB::RayCast(RAY worldRay)
{
	RAYCAST_DATA data{};
	
	if (m_WorldDesc.Intersects(XMLoadFloat3(&worldRay.origin), XMLoadFloat3(&worldRay.direction), data.worldDistance))
		data.isHit = true;

	return data;
}

_bool Bounding_AABB::IntersectToAABB(Bounding_AABB* other)
{
	return m_WorldDesc.Intersects(other->m_WorldDesc);
}

_bool Bounding_AABB::IntersectToOBB(Bounding_OBB* other)
{
	return m_WorldDesc.Intersects(other->GetWorldDesc());
}

_bool Bounding_AABB::IntersectToSphere(Bounding_Sphere* other)
{
	return m_WorldDesc.Intersects(other->GetWorldDesc());
}

void Bounding_AABB::Draw(PrimitiveBatch<VertexPositionColor>* batch, _float4 color)
{
	DX::Draw(batch, m_WorldDesc, XMLoadFloat4(&color));
}

Bounding_AABB* Bounding_AABB::Create(BOUNDING_DESC* arg)
{
	Bounding_AABB* Instance = new Bounding_AABB();

	if (FAILED(Instance->Initialize(arg)))
	{
		MSG_BOX("Failed to create : Bounding_AABB");
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT Bounding_AABB::Initialize(BOUNDING_DESC* arg)
{
	AABB_DESC* desc = static_cast<AABB_DESC*>(arg);

	m_LocalDesc.Center = desc->center;
	m_LocalDesc.Extents = desc->halfSize;

	return S_OK;
}

void Bounding_AABB::Free()
{
	__super::Free();
}

#ifdef USE_IMGUI
void Bounding_AABB::RenderInspector()
{
	ImGui::SeparatorText("Bounding AABB");

	ImGui::DragFloat3("Local Center : ", &m_LocalDesc.Center.x, 0.1f, -FLT_MAX, FLT_MAX);
	ImGui::DragFloat3("Local Half : ", &m_LocalDesc.Extents.x, 0.1f, -FLT_MAX, FLT_MAX);
}
#endif	