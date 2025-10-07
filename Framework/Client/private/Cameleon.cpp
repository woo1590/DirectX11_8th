#include "pch.h"
#include "Cameleon.h"
#include "EngineCore.h"
#include "Skeleton.h"

//component
#include "ModelComponent.h"
#include "AnimatorComponent.h"

Cameleon::Cameleon()
	:Weapon()
{
}

Cameleon::Cameleon(const Cameleon& prototype)
	:Weapon(prototype)
{
}

Cameleon* Cameleon::Create()
{
	Cameleon* Instance = new Cameleon();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Cameleon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_strInstanceTag = "Cameleon";

	AddComponent<ModelComponent>();
	AddComponent<AnimatorComponent>();

	return S_OK;
}

HRESULT Cameleon::Initialize(InitDESC* arg)
{
	auto engine = EngineCore::GetInstance();

	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::GamePlay), "Model_Weapon_Cameleon");

	auto animator = GetComponent<AnimatorComponent>();
	animator->SetAnimation(ENUM_CLASS(LevelID::GamePlay), "AnimationSet_Weapon_Cameleon");

	model->ConnectAnimator();
	animator->ChangeAnimation(0, true);

	/*모델 세팅 이후에 무기 초기화 해야함*/
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	ChangeState(&m_CameleonIdle);

	return S_OK;
}

void Cameleon::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Cameleon::Update(_float dt)
{
	__super::Update(dt);

}

void Cameleon::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void Cameleon::Reload()
{
	ChangeState(&m_CameleonReload);
}

void Cameleon::Fire()
{
	if (&m_CameleonFire == m_CurrState)
		return;
	
	ChangeState(&m_CameleonFire);
}

Object* Cameleon::Clone(InitDESC* arg)
{
	Cameleon* Instance = new Cameleon(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Cameleon::Free()
{
	__super::Free();
}

void Cameleon::CameleonIdle::Enter(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(0, true);
}

void Cameleon::CameleonIdle::Update(Engine::Object* object, Engine::_float dt)
{
}

void Cameleon::CameleonIdle::TestForExit(Engine::Object* object)
{
}

void Cameleon::CameleonReload::Enter(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(1, false);
}

void Cameleon::CameleonReload::Update(Engine::Object* object, Engine::_float dt)
{
}

void Cameleon::CameleonReload::TestForExit(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	auto cameleon = static_cast<Cameleon*>(object);

	if (animator->IsFinished())
		object->ChangeState(&cameleon->m_CameleonIdle);
}

void Cameleon::CameleonFire::Enter(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(3, false);
}

void Cameleon::CameleonFire::Update(Engine::Object* object, Engine::_float dt)
{
}

void Cameleon::CameleonFire::TestForExit(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	auto cameleon = static_cast<Cameleon*>(object);

	if (animator->IsFinished())
		object->ChangeState(&cameleon->m_CameleonIdle);
}
