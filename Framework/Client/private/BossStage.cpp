#include "pch.h"
#include "BossStage.h"

//component
#include "ModelComponent.h"

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

	return S_OK;
}

HRESULT BossStage::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::GamePlay), "Model_Boss_Stage");

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
