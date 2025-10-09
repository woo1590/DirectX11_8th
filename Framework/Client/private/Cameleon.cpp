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

	AddComponent<ModelComponent>();
	AddComponent<AnimatorComponent>();
	m_strInstanceTag = "Cameleon";
	m_eRenderGroup = RenderGroup::NonBlend;

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
	animator->ChangeAnimation(0, true, false);

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

void Cameleon::Idle()
{
	ChangeState(&m_CameleonIdle);
}

void Cameleon::Reload()
{
	if (&m_CameleonIdle == m_CurrState)
		ChangeState(&m_CameleonReload);
}

void Cameleon::Fire()
{
	if (&m_CameleonIdle == m_CurrState)
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
	animator->SetFadeDurtaion(0.2f);
	animator->ChangeAnimation(0, true, false);
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
	animator->ChangeAnimation(1, false, false);
}

void Cameleon::CameleonReload::Update(Engine::Object* object, Engine::_float dt)
{
}

void Cameleon::CameleonReload::TestForExit(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	auto cameleon = static_cast<Cameleon*>(object);
	_float progress = animator->GetProgress();

	if (progress>0.99f)
		object->ChangeState(&cameleon->m_CameleonIdle);
}

void Cameleon::CameleonFire::Enter(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(3, true, false);
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
