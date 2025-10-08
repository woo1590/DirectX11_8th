#include "pch.h"
#include "ConcealedAmmo.h"

//component
#include "ModelComponent.h"
#include "AnimatorComponent.h"

ConcealedAmmo::ConcealedAmmo()
	:Weapon()
{
}

ConcealedAmmo::ConcealedAmmo(const ConcealedAmmo& prototype)
	:Weapon(prototype)
{
}

ConcealedAmmo* ConcealedAmmo::Create()
{
	ConcealedAmmo* Instance = new ConcealedAmmo();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT ConcealedAmmo::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	AddComponent<AnimatorComponent>();

	return S_OK;
}

HRESULT ConcealedAmmo::Initialize(InitDESC* arg)
{
	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::GamePlay), "Model_Weapon_ConcealedAmmo");

	auto animator = GetComponent<AnimatorComponent>();
	animator->SetAnimation(ENUM_CLASS(LevelID::GamePlay), "AnimationSet_Weapon_ConcealedAmmo");

	model->ConnectAnimator();

	ChangeState(&m_ConcealedAmmoIdle);

	/*모델 세팅 이후에 무기 초기화 해야함*/
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	return S_OK;
}

void ConcealedAmmo::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void ConcealedAmmo::Update(_float dt)
{
	__super::Update(dt);

}

void ConcealedAmmo::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void ConcealedAmmo::Idle()
{
	ChangeState(&m_ConcealedAmmoIdle);
}

void ConcealedAmmo::Reload()
{
	if (&m_ConcealedAmmoIdle == m_CurrState)
		ChangeState(&m_ConcealedAmmoReload);
}

void ConcealedAmmo::Fire()
{
	if (&m_ConcealedAmmoIdle == m_CurrState)
		ChangeState(&m_ConcealedAmmoFire);
}

void ConcealedAmmo::Skill()
{
	if (&m_ConcealedAmmoIdle == m_CurrState)
		ChangeState(&m_ConcealedAmmoSkill);
}

Object* ConcealedAmmo::Clone(InitDESC* arg)
{
	ConcealedAmmo* Instance = new ConcealedAmmo(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void ConcealedAmmo::Free()
{
	__super::Free();
}

void ConcealedAmmo::ConcealedAmmoIdle::Enter(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(1, true, false);
}

void ConcealedAmmo::ConcealedAmmoIdle::Update(Engine::Object* object, Engine::_float dt)
{
}

void ConcealedAmmo::ConcealedAmmoIdle::TestForExit(Engine::Object* object)
{
}

void ConcealedAmmo::ConcealedAmmoReload::Enter(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(2, false, true);
}

void ConcealedAmmo::ConcealedAmmoReload::Update(Engine::Object* object, Engine::_float dt)
{
}

void ConcealedAmmo::ConcealedAmmoReload::TestForExit(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	auto blast = static_cast<ConcealedAmmo*>(object);

	if (animator->IsFinished())
		blast->ChangeState(&blast->m_ConcealedAmmoIdle);
}

void ConcealedAmmo::ConcealedAmmoFire::Enter(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(0, true, true);
	animator->SetPlaySpeedScale(4.f);
}

void ConcealedAmmo::ConcealedAmmoFire::Update(Engine::Object* object, Engine::_float dt)
{
}

void ConcealedAmmo::ConcealedAmmoFire::TestForExit(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	auto blast = static_cast<ConcealedAmmo*>(object);

	if (animator->IsFinished())
		blast->ChangeState(&blast->m_ConcealedAmmoIdle);
}

void ConcealedAmmo::ConcealedAmmoSkill::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(4, false, true);
}

void ConcealedAmmo::ConcealedAmmoSkill::Update(Object* object, _float dt)
{
}

void ConcealedAmmo::ConcealedAmmoSkill::TestForExit(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	auto blast = static_cast<ConcealedAmmo*>(object);

	if (animator->IsFinished())
		blast->ChangeState(&blast->m_ConcealedAmmoIdle);
}
