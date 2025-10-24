#include "MapEditorPCH.h"
#include "Door.h"

#include "Door_L.h"
#include "Door_R.h"

//component
#include "MakePrefabComponent.h"

Door::Door()
	:ContainerObject()
{
}

Door::Door(const Door& prototype)
	:ContainerObject(prototype)
{
}

Door* Door::Create()
{
	Door* Instance = new Door();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Door::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<MakePrefabComponent>();

	m_strInstanceTag = "Door";

	return S_OK;
}

HRESULT Door::Initialize(InitDESC* arg)
{
	if (FAILED(Object::Initialize(arg)))
		return E_FAIL;

	m_iNumPartObjects = ENUM_CLASS(Parts::Count);
	m_PartObjects.resize(m_iNumPartObjects);

	MakePrefabComponent::MAKE_PREFAB_DESC desc{};
	desc.prototypeTag = "Prototype_Object_Door";
	desc.modelTag = "";
	desc.layerTag = "Layer_StaticMapObject";

	auto makePrefab = GetComponent<MakePrefabComponent>();
	if (FAILED(makePrefab->Initialize(&desc)))
		return E_FAIL;

	PartObject::PART_OBJECT_DESC doorL{};
	doorL.parent = this;
	doorL.position = _float3{ -14.7f,0.f,0.f };
	AddPartObject(ENUM_CLASS(LevelID::Editor), "Prototype_Object_Door_L", 0,&doorL);

	PartObject::PART_OBJECT_DESC doorR{};
	doorR.parent = this;
	doorR.position = _float3{ 14.7f,0.f,0.f };
	AddPartObject(ENUM_CLASS(LevelID::Editor), "Prototype_Object_Door_R", 1, &doorR);

	return S_OK;
}

Object* Door::Clone(InitDESC* arg)
{
	Door* Instance = new Door(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Door::Free()
{
	__super::Free();
}
