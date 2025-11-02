#include "pch.h"
#include "BossStage.h"

//component
#include "ModelComponent.h"
#include "MeshColliderComponent.h"

BossStage::BossStage()
	:Object()
{
}

BossStage::BossStage(const BossStage& prototype)
	:Object(prototype)
{
}

BossStage* BossStage::Create()
{
	BossStage* Instance = new BossStage();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);
	
	return Instance;
}

HRESULT BossStage::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_strInstanceTag = "BossStage";
	m_eRenderGroup = RenderGroup::NonBlend;

	AddComponent<ModelComponent>();
	AddComponent<MeshColliderComponent>();

	return S_OK;
}

HRESULT BossStage::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::StageBoss), "Model_Boss_Stage");

	MeshColliderComponent::MESH_COLLIDER_DESC colliderDesc{};
	colliderDesc.colliderFilter = ENUM_CLASS(ColliderFilter::StaticMapObject);
	auto collider = GetComponent<MeshColliderComponent>();
	collider->Initialize(&colliderDesc);
	collider->SetModel(ENUM_CLASS(LevelID::StageBoss), "Model_Boss_Stage");
	EngineCore::GetInstance()->RegisterCollider(collider);
	return S_OK;
}

Object* BossStage::Clone(InitDESC* arg)
{
	BossStage* Instance = new BossStage(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void BossStage::Free()
{
	__super::Free();
}
