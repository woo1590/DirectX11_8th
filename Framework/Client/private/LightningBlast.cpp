#include "pch.h"
#include "LightningBlast.h"

//component
#include "ModelComponent.h"
#include "AnimatorComponent.h"

LightningBlast::LightningBlast()
	:Weapon()
{
}

LightningBlast::LightningBlast(const LightningBlast& prototype)
	:Weapon(prototype)
{
}

LightningBlast* LightningBlast::Create()
{
	LightningBlast* Instance = new LightningBlast();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT LightningBlast::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	AddComponent<AnimatorComponent>();

	return S_OK;
}

HRESULT LightningBlast::Initialize(InitDESC* arg)
{
	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::GamePlay), "Model_Weapon_LightningBlast");

	auto animator = GetComponent<AnimatorComponent>();
	animator->SetAnimation(ENUM_CLASS(LevelID::GamePlay), "AnimationSet_Weapon_LightningBlast");

	model->ConnectAnimator();
	animator->ChangeAnimation(0, true);

	/*모델 세팅 이후에 무기 초기화 해야함*/
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	return S_OK;
}

void LightningBlast::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void LightningBlast::Update(_float dt)
{
	__super::Update(dt);

	/*For Animation Test*/
	{
		auto engine = EngineCore::GetInstance();
		auto animator = GetComponent<AnimatorComponent>();

		if (engine->IsKeyPressed(VK_F1))
			animator->ChangeAnimation(0, true);

		if (engine->IsKeyPressed(VK_F2))
			animator->ChangeAnimation(1, true);

		if (engine->IsKeyPressed(VK_F3))
			animator->ChangeAnimation(4, true);
	}

}

void LightningBlast::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void LightningBlast::Reload()
{
	ChangeState(&m_LightningBlastReload);
}

void LightningBlast::Fire()
{
	if (&m_LightningBlastFire == m_CurrState)
		return;

	ChangeState(&m_LightningBlastFire);
}

Object* LightningBlast::Clone(InitDESC* arg)
{
	LightningBlast* Instance = new LightningBlast(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void LightningBlast::Free()
{
	__super::Free();
}

void LightningBlast::LightningBlastIdle::Enter(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(0, true);
}

void LightningBlast::LightningBlastIdle::Update(Engine::Object* object, Engine::_float dt)
{
}

void LightningBlast::LightningBlastIdle::TestForExit(Engine::Object* object)
{
}

void LightningBlast::LightningBlastReload::Enter(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(1);
}

void LightningBlast::LightningBlastReload::Update(Engine::Object* object, Engine::_float dt)
{
}

void LightningBlast::LightningBlastReload::TestForExit(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	auto blast = static_cast<LightningBlast*>(object);

	if (animator->IsFinished())
		blast->ChangeState(&blast->m_LightningBlastIdle);
}

void LightningBlast::LightningBlastFire::Enter(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(4);
}

void LightningBlast::LightningBlastFire::Update(Engine::Object* object, Engine::_float dt)
{
}

void LightningBlast::LightningBlastFire::TestForExit(Engine::Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	auto blast = static_cast<LightningBlast*>(object);

	if (animator->IsFinished())
		blast->ChangeState(&blast->m_LightningBlastIdle);
}
