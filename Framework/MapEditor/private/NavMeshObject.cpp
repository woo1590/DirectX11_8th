#include "MapEditorPCH.h"
#include "NavMeshObject.h"
#include "PickingSystem.h"

//component
#include "NavDataComponent.h"
#include "NavPickable.h"

NavMeshObject::NavMeshObject()
	:Object()
{
}

NavMeshObject::NavMeshObject(const NavMeshObject& prototype)
	:Object(prototype),
	m_pPickingSystem(prototype.m_pPickingSystem)
{
	m_pPickingSystem->AddRef();
}

NavMeshObject* NavMeshObject::Create(PickingSystem* picking)
{
	NavMeshObject* Instance = new NavMeshObject();

	if (FAILED(Instance->Initialize_Prototype(picking)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT NavMeshObject::Initialize_Prototype(PickingSystem* picking)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_strInstanceTag = "NavMeshObject";
	m_eRenderGroup = RenderGroup::NavMeshDebug;

	AddComponent<NavDataComponent>();
	AddComponent<NavPickable>();

	m_pPickingSystem = picking;
	m_pPickingSystem->AddRef();

	return S_OK;
}

HRESULT NavMeshObject::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	m_pPickingSystem->RegisterComponent(GetComponent<NavPickable>());

	return S_OK;
}

void NavMeshObject::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void NavMeshObject::Update(_float dt)
{
	__super::Update(dt);
}

void NavMeshObject::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT NavMeshObject::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	auto navData = GetComponent<NavDataComponent>();

	return navData->ExtractRenderProxy(proxies[ENUM_CLASS(m_eRenderGroup)]);
}

void NavMeshObject::ExportNavData(std::ofstream& file)
{
	auto navData = GetComponent<NavDataComponent>();

	navData->ExportNavData(file);
}

void NavMeshObject::ImportNavData(std::ifstream& file)
{
	auto navData = GetComponent<NavDataComponent>();

	navData->ImportNavData(file);
}

Object* NavMeshObject::Clone(InitDESC* arg)
{
	NavMeshObject* Instance = new NavMeshObject(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void NavMeshObject::Free()
{
	m_pPickingSystem->UnRegisterComponent(GetComponent<NavPickable>());
	Safe_Release(m_pPickingSystem);

	__super::Free();
}
