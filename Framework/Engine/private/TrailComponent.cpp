#include "EnginePCH.h"
#include "TrailComponent.h"
#include "EngineCore.h"
#include "VIBuffer_Trail.h"
#include "Material.h"
#include "MaterialInstance.h"

#include "Object.h"
#include "TransformComponent.h"

TrailComponent::TrailComponent(Object* owner)
	:Component(owner)
{
}

TrailComponent::TrailComponent(const TrailComponent& prototype)
	:Component(prototype)
{
}

TrailComponent* TrailComponent::Create(Object* owner)
{
	TrailComponent* Instance = new TrailComponent(owner);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT TrailComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT TrailComponent::Initialize(InitDESC* arg)
{
	TRAIL_DESC* desc = static_cast<TRAIL_DESC*>(arg);
	m_eMode = desc->mode;
	m_fWidth = desc->width;
	
	switch (m_eMode)
	{
	case Engine::TrailMode::Default:
	{
		m_iMaxNumPoints = 2;
		m_DefaultPoints.resize(m_iMaxNumPoints);
	}break;
	case Engine::TrailMode::Ribbon:
	{
		m_fMinDistance = desc->minDistance;
		m_iMaxNumPoints = desc->numMaxPoints;
		m_fMaxLifeTime = desc->maxLifeTime;
	}break;
	default:
		break;
	}

	m_pBuffer = VIBuffer_Trail::Create(m_iMaxNumPoints);

	return S_OK;
}

void TrailComponent::Update(_float dt)
{
	if (m_eMode == TrailMode::Ribbon)
	{
		_float3 currPosition = m_pOwner->GetComponent<TransformComponent>()->GetPosition();
		AddPoint(currPosition);

		for (auto& point : m_RibbonPoints)
			point.lifeTime += dt;

		while (!m_RibbonPoints.empty() && m_RibbonPoints.front().lifeTime >= m_fMaxLifeTime)
			m_RibbonPoints.pop_front();

		BuildVertexData();
	}
}

void TrailComponent::SetMaterial(_uint levelID, const _string& key)
{
	auto engine = EngineCore::GetInstance();

	m_pMaterial = engine->GetMaterial(levelID, key);
	m_pMaterial->AddRef();

	m_pMaterialInstance = MaterialInstance::Create();
}

void TrailComponent::AddPoints(_float3 point0, _float3 point1)
{
	m_DefaultPoints[0] = VTX_TRAIL{ point0,m_fWidth };
	m_DefaultPoints[1] = VTX_TRAIL{ point1,m_fWidth };

	m_pBuffer->UpdateBuffer(m_DefaultPoints.data(), m_iMaxNumPoints);
}

void TrailComponent::AddPoint(_float3 point)
{
	if (!m_RibbonPoints.empty())
	{
		_float3 lastPoint = m_RibbonPoints.back().position;
		_float distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&lastPoint) - XMLoadFloat3(&point)));

		if (distance < m_fMinDistance)
			return;
	}

	TRAIL_POINT trailPoint{};
	trailPoint.position = point;
	trailPoint.lifeTime = 0.f;

	m_RibbonPoints.push_back(trailPoint);
	if (m_RibbonPoints.size() >= m_iMaxNumPoints)
		m_RibbonPoints.pop_front();
}

void TrailComponent::BuildVertexData()
{
	if (m_RibbonPoints.size() < 2)
		return;

	std::vector<VTX_TRAIL> vertices;

	for (_uint i = 0; i < m_RibbonPoints.size() - 1; ++i)
	{
		VTX_TRAIL vertex0{}, vertex1{};
		TRAIL_POINT point0{}, point1{};

		point0 = m_RibbonPoints[i];
		point1 = m_RibbonPoints[i + 1];

		vertex0.position = point0.position;
		vertex0.width = m_fWidth;
		vertex0.lifeTime.x = point0.lifeTime;
		vertex0.lifeTime.y = m_fMaxLifeTime;

		vertex1.position = point1.position;
		vertex1.width = m_fWidth;
		vertex1.lifeTime.x = point1.lifeTime;
		vertex1.lifeTime.y = m_fMaxLifeTime;

		vertices.push_back(vertex0);
		vertices.push_back(vertex1);
	}

	if (vertices.size() >= 2)
		m_pBuffer->UpdateBuffer(vertices.data(),m_RibbonPoints.size());
}

HRESULT TrailComponent::ExtractRenderProxy(std::vector<RenderProxy>& proxies)
{
	if (m_DefaultPoints.size() < 2 && m_RibbonPoints.size() < 2)
		return S_OK;

	RenderProxy proxy{};
	XMStoreFloat4x4(&proxy.worldMatrix, XMMatrixIdentity());
	proxy.buffer = m_pBuffer;
	proxy.material = m_pMaterial;
	proxy.materialInstance = m_pMaterialInstance;
	
	proxies.push_back(proxy);

	return S_OK;
}

void TrailComponent::Free()
{
	__super::Free();

	Safe_Release(m_pBuffer);
	Safe_Release(m_pMaterial);
	Safe_Release(m_pMaterialInstance);
}
