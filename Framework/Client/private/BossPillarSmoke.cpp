#include "pch.h"
#include "BossPillarSmoke.h"
#include "MaterialInstance.h"

//component
#include "ModelComponent.h"

BossPillarSmoke::BossPillarSmoke()
	:Object()
{
}

BossPillarSmoke::BossPillarSmoke(const BossPillarSmoke& prototype)
	:Object(prototype)
{
}

BossPillarSmoke* BossPillarSmoke::Create()
{
	BossPillarSmoke* Instance = new BossPillarSmoke();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT BossPillarSmoke::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	
	AddComponent<ModelComponent>();
	m_eRenderGroup = RenderGroup::Blend;

	return S_OK;
}

HRESULT BossPillarSmoke::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	BOSS_PILLAR_SMOKE_DESC* desc = static_cast<BOSS_PILLAR_SMOKE_DESC*>(arg);

	/*model*/
	auto model = GetComponent<ModelComponent>();
	model->Initialize(nullptr);
	model->SetModel(ENUM_CLASS(LevelID::Static), "Model_BossPillarSmoke");

	auto mtrlInstance = model->GetMaterialInstance();
	mtrlInstance->SetPass("BossPillarSmoke_Pass");
	mtrlInstance->SetFloat("g_CustomAlpha", 0.f);

	m_fDurtaion = desc->duration;

	return S_OK;
}

void BossPillarSmoke::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void BossPillarSmoke::Update(_float dt)
{
	__super::Update(dt);

	m_fElapsedTime += dt;
	if (m_fElapsedTime >= m_fDurtaion)
		return;

	_float t = m_fElapsedTime / m_fDurtaion;
	t = math::EaseInQuint(t);

	auto mtrlInstance = GetComponent<ModelComponent>()->GetMaterialInstance();
	mtrlInstance->SetFloat("g_CustomAlpha", t);
}

void BossPillarSmoke::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* BossPillarSmoke::Clone(InitDESC* arg)
{
	BossPillarSmoke* Instance = new BossPillarSmoke(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void BossPillarSmoke::Free()
{
	__super::Free();
}
