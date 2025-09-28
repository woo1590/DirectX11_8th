#include "MapEditorPCH.h"
#include "PreviewObject.h"
#include "ModelComponent.h"

PreviewObject::PreviewObject()
	:Object()
{
}

PreviewObject::PreviewObject(const PreviewObject& prototype)
	:Object(prototype),
	m_Prefab(prototype.m_Prefab)
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

	AddComponent<ModelComponent>();

	return S_OK;
}

HRESULT PreviewObject::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::Editor), m_Prefab.modelTag);

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
