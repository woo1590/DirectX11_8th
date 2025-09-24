#include "MapEditorPCH.h"
#include "PreviewObject.h"

PreviewObject::PreviewObject()
	:Object()
{
}

PreviewObject::PreviewObject(const PreviewObject& prototype)
	:Object(prototype)
{
}

PreviewObject* PreviewObject::Create(PREFAB prefab)
{
	PreviewObject* Instance = new PreviewObject();

	if (FAILED(Instance->Initialize_Prototype(prefab)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT PreviewObject::Initialize_Prototype(PREFAB prefab)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_Prefab = prefab;

	return S_OK;
}

HRESULT PreviewObject::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	return S_OK;
}

void PreviewObject::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void PreviewObject::Update(_float dt)
{
	__super::Update(dt);
}

void PreviewObject::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* PreviewObject::Clone(InitDESC* arg)
{
	PreviewObject* Instance = new PreviewObject(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void PreviewObject::Free()
{
	__super::Free();
}
