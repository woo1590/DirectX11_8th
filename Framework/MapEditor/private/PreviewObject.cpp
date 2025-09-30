#include "MapEditorPCH.h"
#include "PreviewObject.h"
#include "ModelComponent.h"
#include "ModelPickable.h"
#include "PickingSystem.h"

PreviewObject::PreviewObject()
	:Object()
{
}

PreviewObject::PreviewObject(const PreviewObject& prototype)
	:Object(prototype),
	m_Prefab(prototype.m_Prefab),
	m_pPickingSystem(prototype.m_pPickingSystem)
{
	m_pPickingSystem->AddRef();
}

PreviewObject* PreviewObject::Create(PREFAB prefab, PickingSystem* picking)
{
	PreviewObject* Instance = new PreviewObject();

	if (FAILED(Instance->Initialize_Prototype(prefab,picking)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT PreviewObject::Initialize_Prototype(PREFAB prefab, PickingSystem* picking)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_Prefab = prefab;
	m_strInstanceTag = m_Prefab.prototypeTag;

	m_pPickingSystem = picking;
	m_pPickingSystem->AddRef();

	AddComponent<ModelComponent>();
	AddComponent<ModelPickable>();

	return S_OK;
}

HRESULT PreviewObject::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::Editor), m_Prefab.modelTag);

	auto pickable = GetComponent<ModelPickable>();
	pickable->SetModel(ENUM_CLASS(LevelID::Editor), m_Prefab.modelTag);

	m_pTransform->SetScale(m_Prefab.scale);

	/*Register PickingSystem*/
	m_pPickingSystem->RegisterComponent(GetComponent<ModelPickable>());

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
	m_pPickingSystem->UnRegisterComponent(GetComponent<ModelPickable>());
	Safe_Release(m_pPickingSystem);
	__super::Free();
}
