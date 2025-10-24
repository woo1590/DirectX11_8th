#include "pch.h"
#include "StaticMapObject.h"

//component
#include "ModelComponent.h"

StaticMapObject::StaticMapObject()
	:Object()
{
}

StaticMapObject::StaticMapObject(const StaticMapObject& prototype)
	:Object(prototype)
{
}

StaticMapObject* StaticMapObject::Create()
{
	StaticMapObject* Instance = new StaticMapObject();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT StaticMapObject::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();

	return S_OK;
}

HRESULT StaticMapObject::Initialize(InitDESC* arg)
{
	Object::OBJECT_DESC* desc = static_cast<OBJECT_DESC*>(arg);

	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::GamePlay), desc->modelTag);

	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	return S_OK;
}

Object* StaticMapObject::Clone(InitDESC* arg)
{
	StaticMapObject* Instance = new StaticMapObject(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void StaticMapObject::Free()
{
	__super::Free();
}
