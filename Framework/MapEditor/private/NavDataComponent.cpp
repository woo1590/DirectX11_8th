#include "MapEditorPCH.h"
#include "NavDataComponent.h"
#include "VIBufferCell.h"
#include "MaterialInstance.h"

//object
#include "Object.h"

//component
#include "TransformComponent.h"

NavDataComponent::NavDataComponent(Object* owner)
	:Component(owner)
{
}

NavDataComponent::NavDataComponent(const NavDataComponent& prototype)
	:Component(prototype)
{
}

NavDataComponent* NavDataComponent::Create(Object* owner)
{
	NavDataComponent* Instance = new NavDataComponent(owner);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT NavDataComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT NavDataComponent::Initialize(InitDESC* arg)
{
	return S_OK;
}

HRESULT NavDataComponent::ExtractRenderProxy(std::vector<RenderProxy>& proxies)
{
	auto transform = m_pOwner->GetComponent<TransformComponent>();

	for (_uint i = 0; i < m_VIBuffers.size(); ++i)
	{
		RenderProxy proxy{};
		proxy.buffer = m_VIBuffers[i];
		proxy.worldMatrix = transform->GetWorldMatrix();

		proxies.push_back(proxy);
	}

	return S_OK;
}

void NavDataComponent::AddNavCell(const _float3* points)
{
	_uint index = m_NavCellDatas.size();

	/*add cell data*/
	NAVCELL_DATA navCellData{};
	navCellData.index = index;
	memcpy_s(navCellData.points, sizeof(_float3) * 3, points, sizeof(_float3) * 3);

	_uint p0, p1, p2;
	p0 = m_iPointIndex++;
	p1 = m_iPointIndex++;
	p2 = m_iPointIndex++;

	navCellData.edges[0] = std::make_pair(p0, p1);
	navCellData.edges[1] = std::make_pair(p1, p2);
	navCellData.edges[2] = std::make_pair(p2, p0);

	VIBufferCell* bufferCell = VIBufferCell::Create(navCellData.points);
	
	m_NavCellDatas.push_back(navCellData);
	m_VIBuffers.push_back(bufferCell);
}

void NavDataComponent::Free()
{
	__super::Free();

	for (auto& buffer : m_VIBuffers)
		Safe_Release(buffer);
	m_VIBuffers.clear();

	for (auto& mtrl : m_MaterialInstances)
		Safe_Release(mtrl);
	m_MaterialInstances.clear();
}
