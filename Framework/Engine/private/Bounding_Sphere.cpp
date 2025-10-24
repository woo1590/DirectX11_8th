#include "EnginePCH.h"
#include "Bounding_Sphere.h"
#include "Bounding_AABB.h"
#include "Bounding_OBB.h"

Bounding_Sphere::Bounding_Sphere()
	:Bounding()
{
}

Bounding_Sphere* Bounding_Sphere::Create(BOUNDING_DESC* arg)
{
	Bounding_Sphere* Instance = new Bounding_Sphere();

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Bounding_Sphere::Initialize(BOUNDING_DESC* arg)
{
	SPHERE_DESC* desc = static_cast<SPHERE_DESC*>(arg);

	m_LocalDesc.Center = desc->center;
	m_LocalDesc.Radius = desc->radius;

	return S_OK;
}

void Bounding_Sphere::Update(_float4x4 worldMatrix)
{
	m_LocalDesc.Transform(m_WorldDesc, XMLoadFloat4x4(&worldMatrix));
}

_bool Bounding_Sphere::Instersect(Bounding* other)
{
	return other->IntersectToSphere(this);
}

_bool Bounding_Sphere::IntersectToAABB(Bounding_AABB* other)
{
	return m_WorldDesc.Intersects(other->GetWorldDesc());
}

_bool Bounding_Sphere::IntersectToOBB(Bounding_OBB* other)
{
	return m_WorldDesc.Intersects(other->GetWorldDesc());
}

_bool Bounding_Sphere::IntersectToSphere(Bounding_Sphere* other)
{
	return m_WorldDesc.Intersects(other->m_WorldDesc);
}


void Bounding_Sphere::Draw(PrimitiveBatch<VertexPositionColor>* batch, _float4 color)
{
	DX::Draw(batch, m_WorldDesc, XMLoadFloat4(&color));
}

void Bounding_Sphere::Free()
{
	__super::Free();
}

#ifdef USE_IMGUI
void Bounding_Sphere::RenderInspector()
{
	ImGui::SeparatorText("Bounding Sphere");

	ImGui::DragFloat3("Local Center : ", &m_LocalDesc.Center.x, 0.1f, -FLT_MAX, FLT_MAX);
	ImGui::DragFloat("Local Radius : ", &m_LocalDesc.Radius, 0.1f, -FLT_MAX, FLT_MAX);
}
#endif
