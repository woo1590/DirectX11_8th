#include "pch.h"
#include "Door.h"

#include "Door_L.h"
#include "Door_R.h"

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

	return S_OK;
}
HRESULT Door::Initialize(InitDESC* arg)
{
	if (FAILED(Object::Initialize(arg)))
		return E_FAIL;

	m_iNumPartObjects = ENUM_CLASS(Parts::Count);
	m_PartObjects.resize(m_iNumPartObjects);
	if (FAILED(CreatePartObjects()))
		return E_FAIL;

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

HRESULT Door::CreatePartObjects()
{
	/*add door l*/
	{
		Door_L::DOOR_L_DESC doorLDesc{};
		doorLDesc.parent = this;
		if (FAILED(AddPartObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Door_L", ENUM_CLASS(Parts::Door_L), &doorLDesc)))
			return E_FAIL;
	}
	/*add door r*/
	{
		Door_R::DOOR_R_DESC doorRDesc{};
		doorRDesc.parent = this;
		if (FAILED(AddPartObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Door_R", ENUM_CLASS(Parts::Door_R), &doorRDesc)))
			return E_FAIL;
	}

	return S_OK;
}

void Door::DoorOpen::Enter(Object* object)
{
}

void Door::DoorOpen::Update(Object* object, _float t)
{
}

void Door::DoorOpen::TestForExit(Object* object)
{
}

void Door::DoorOpening::Enter(Object* object)
{
}

void Door::DoorOpening::Update(Object* object, _float t)
{
}

void Door::DoorOpening::TestForExit(Object* object)
{
}

void Door::DoorClosed::Enter(Object* object)
{
}

void Door::DoorClosed::Update(Object* object, _float t)
{
}

void Door::DoorClosed::TestForExit(Object* object)
{
}

void Door::DoorClosing::Enter(Object* object)
{
}

void Door::DoorClosing::Update(Object* object, _float t)
{
}

void Door::DoorClosing::TestForExit(Object* object)
{
}
