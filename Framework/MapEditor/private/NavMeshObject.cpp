#include "MapEditorPCH.h"
#include "NavMeshObject.h"

//component
#include "NavDataComponent.h"
#include "NavPickable.h"

NavMeshObject::NavMeshObject()
	:Object()
{
}

NavMeshObject::NavMeshObject(const NavMeshObject& prototype)
	:Object(prototype)
{
}

NavMeshObject* NavMeshObject::Create()
{
	NavMeshObject* Instance = new NavMeshObject();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT NavMeshObject::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_strInstanceTag = "NavMeshObject";
	m_eRenderGroup = RenderGroup::NavMeshDebug;

	AddComponent<NavDataComponent>();
	AddComponent<NavPickable>();

	return S_OK;
}

HRESULT NavMeshObject::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

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

Object* NavMeshObject::Clone(InitDESC* arg)
{
	NavMeshObject* Instance = new NavMeshObject(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void NavMeshObject::Free()
{
	__super::Free();
}
