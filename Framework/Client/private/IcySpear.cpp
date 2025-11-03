#include "pch.h"
#include "IcySpear.h"

//component
#include "ModelComponent.h"
#include "AnimatorComponent.h"

IcySpear::IcySpear()
	:Weapon()
{
}

IcySpear::IcySpear(const IcySpear& prototype)
	:Weapon(prototype)
{
}

IcySpear* IcySpear::Create()
{
	IcySpear* Instance = new IcySpear();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT IcySpear::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	AddComponent<AnimatorComponent>();
	m_strInstanceTag = "IcySpear";
	m_eRenderGroup = RenderGroup::NonBlend;

	return S_OK;
}

HRESULT IcySpear::Initialize(InitDESC* arg)
{
	auto engine = EngineCore::GetInstance();

	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::Static), "Model_Weapon_IcySpear");

	auto animator = GetComponent<AnimatorComponent>();
	animator->SetAnimation(ENUM_CLASS(LevelID::Static), "AnimationSet_Weapon_IcySpear");

	model->ConnectAnimator();

	/*모델 세팅 이후에 무기 초기화 해야함*/
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	ChangeState(&m_IcySpearIdle);

	m_eWeaponID = WeaponID::IcySpear;

	return S_OK;
}

void IcySpear::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void IcySpear::Update(_float dt)
{
	__super::Update(dt);
}

void IcySpear::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void IcySpear::Fire()
{
	if (&m_IcySpearIdle == m_CurrState)
		ChangeState(&m_IcySpearFire);
}

void IcySpear::Skill()
{
	if (&m_IcySpearIdle == m_CurrState)
		ChangeState(&m_IcySpearSkill);
}

Object* IcySpear::Clone(InitDESC* arg)
{
	IcySpear* Instance = new IcySpear(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void IcySpear::Free()
{
	__super::Free();
}


void IcySpear::IcySpearIdle::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(1, true);
}

void IcySpear::IcySpearIdle::Update(Object* object, _float dt)
{
}

void IcySpear::IcySpearIdle::TestForExit(Object* object)
{
}

void IcySpear::IcySpearFire::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(0, false, true);
}

void IcySpear::IcySpearFire::Update(Object* object, _float dt)
{
}

void IcySpear::IcySpearFire::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	auto icySpear = static_cast<IcySpear*>(object);

	if (animator->IsFinished())
		icySpear->ChangeState(&icySpear->m_IcySpearIdle);
}

void IcySpear::IcySpearSkill::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(4, false, true);
}

void IcySpear::IcySpearSkill::Update(Object* object, _float dt)
{
}

void IcySpear::IcySpearSkill::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	auto icySpear = static_cast<IcySpear*>(object);
	_float progress = animator->GetProgress();

	if (progress>=0.99f)
		icySpear->ChangeState(&icySpear->m_IcySpearIdle);
}
