#include "EnginePCH.h"
#include "NavigationSystem.h"
#include "EngineCore.h"
#include "NavigationComponent.h"
#include "NavMesh.h"

NavigationSystem::NavigationSystem()
{
}

NavigationSystem* NavigationSystem::Create()
{
	NavigationSystem* Instance = new NavigationSystem();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT NavigationSystem::Initialize()
{
	return S_OK;
}

HRESULT NavigationSystem::SetNavMesh(_uint levelID, const _string& navMeshTag)
{
	Safe_Release(m_pNavMesh);
	
	auto engine = EngineCore::GetInstance();

	auto navMesh = engine->GetNavMesh(levelID, navMeshTag);
	if (!navMesh)
	{
		MSG_BOX("Navmesh not exist");
		return E_FAIL;
	}

	m_pNavMesh = navMesh;
	m_pNavMesh->AddRef();

	return S_OK;
}

void NavigationSystem::RegisterNavigation(NavigationComponent* component)
{
	component->AttachSystem(this);
}

_bool NavigationSystem::IsCellExist(_uint cellIndex)
{
	return m_pNavMesh->IsCellExist(cellIndex);
}

_float3 NavigationSystem::GetPositionInCell(_uint cellIndex)
{
	return m_pNavMesh->GetPositionInCell(cellIndex);
}

_bool NavigationSystem::IsLinkedCell(_float3 position, _uint& currCellIndex)
{
	return m_pNavMesh->IsLinkedCell(position, currCellIndex);
}

_bool NavigationSystem::IsMove(_float3 position, _uint& currCellIndex)
{
	return m_pNavMesh->IsMove(position, currCellIndex);
}

_float NavigationSystem::GetHeight(_float3 position, _uint currCellIndex)
{
	return m_pNavMesh->GetHeight(position, currCellIndex);
}

_float3 NavigationSystem::MakeSlideVector(_float3 position, _float3 nextPosition, _uint& currCellIndex)
{
	return m_pNavMesh->MakeSlideVector(position, nextPosition, currCellIndex);
}

HRESULT NavigationSystem::ExtractDebugProxies(std::vector<RenderProxy>& proxies)
{
	if (!m_pNavMesh)
		return S_OK;

	return m_pNavMesh->ExtractDebugProxies(proxies);
}

void NavigationSystem::Free()
{
	__super::Free();

	Safe_Release(m_pNavMesh);
}
