#include "pch.h"
#include "StaticMapObject.h"
#include "MaterialInstance.h"

//component
#include "ModelComponent.h"
#include "MeshColliderComponent.h"

StaticMapObject::StaticMapObject(_bool useEmissive)
	:Object(),
	m_UseEmissive(useEmissive)
{
}

StaticMapObject::StaticMapObject(const StaticMapObject& prototype)
	:Object(prototype),
	m_UseEmissive(prototype.m_UseEmissive)
{
}

StaticMapObject* StaticMapObject::Create(_bool useEmissive)
{
	StaticMapObject* Instance = new StaticMapObject(useEmissive);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT StaticMapObject::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	AddComponent<MeshColliderComponent>();

	m_eRenderGroup = RenderGroup::NonBlend;

	return S_OK;
}

HRESULT StaticMapObject::Initialize(InitDESC* arg)
{
	Object::OBJECT_DESC* desc = static_cast<OBJECT_DESC*>(arg);

	auto model = GetComponent<ModelComponent>();
	model->Initialize(nullptr);
	model->SetModel(ENUM_CLASS(LevelID::Static), desc->modelTag);
	if (m_UseEmissive)
		model->GetMaterialInstance()->SetPass("EmissiveMap_Pass");

	MeshColliderComponent::MESH_COLLIDER_DESC colliderDesc{};
	colliderDesc.colliderFilter = ENUM_CLASS(ColliderFilter::StaticMapObject);
	auto collider = GetComponent<MeshColliderComponent>();
	collider->Initialize(&colliderDesc);
	collider->SetModel(ENUM_CLASS(LevelID::Static), desc->modelTag);
	EngineCore::GetInstance()->RegisterCollider(collider);

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
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<MeshColliderComponent>());

	__super::Free();
}
