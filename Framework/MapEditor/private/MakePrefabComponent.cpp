#include "MapEditorPCH.h"
#include "MakePrefabComponent.h"
#include "Object.h"
#include "TransformComponent.h"

MakePrefabComponent::MakePrefabComponent(Object* owner)
	:Component(owner)
{
}

MakePrefabComponent::MakePrefabComponent(const MakePrefabComponent& prototype)
	:Component(prototype)
{
}

MakePrefabComponent* MakePrefabComponent::Create(Object* owner)
{
	MakePrefabComponent* Instance = new MakePrefabComponent(owner);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT MakePrefabComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT MakePrefabComponent::Initialize(InitDESC* arg)
{
	MAKE_PREFAB_DESC* desc = static_cast<MAKE_PREFAB_DESC*>(arg);

	m_strPrototypeTag = desc->prototypeTag;
	m_strModelTag = desc->modelTag;
	m_strLayerTag = desc->layerTag;

	return S_OK;
}

PREFAB MakePrefabComponent::ExportPrefab()
{
	auto transform = m_pOwner->GetComponent<TransformComponent>();
	
	PREFAB prefab{};
	prefab.prototypeTag = m_strPrototypeTag;
	prefab.modelTag = m_strModelTag;
	prefab.layerTag = m_strLayerTag;
	prefab.position = transform->GetPosition();
	prefab.scale = transform->GetScale();
	prefab.quaternion = transform->GetQuaternion();

	return prefab;
}

void MakePrefabComponent::Free()
{
	__super::Free();
}
