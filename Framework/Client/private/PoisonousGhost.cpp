#include "pch.h"
#include "PoisonousGhost.h"
#include "Bounding_OBB.h"

//component
#include "ModelComponent.h"
#include "AnimatorComponent.h"
#include "ColliderComponent.h"

PoisonousGhost::PoisonousGhost()
	:Weapon()
{
}

PoisonousGhost::PoisonousGhost(const PoisonousGhost& prototype)
	:Weapon(prototype)
{
}

PoisonousGhost* PoisonousGhost::Create()
{
	PoisonousGhost* Instance = new PoisonousGhost();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT PoisonousGhost::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	AddComponent<AnimatorComponent>();
	AddComponent<ColliderComponent>();

	m_strInstanceTag = "PoisonousGhost";
	m_eRenderGroup = RenderGroup::NonBlend;

	return S_OK;
}

HRESULT PoisonousGhost::Initialize(InitDESC* arg)
{
	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::Static), "Model_Weapon_PoisonousGhost");

	auto animator = GetComponent<AnimatorComponent>();
	animator->SetAnimation(ENUM_CLASS(LevelID::Static), "AnimationSet_Weapon_PoisonousGhost");

	model->ConnectAnimator();

	Bounding_OBB::OBB_DESC obbDesc{};
	obbDesc.type = ColliderType::OBB;
	obbDesc.center = _float3{ 1.6f,0.f,0.f };
	obbDesc.halfSize = _float3{ 2.f,0.1f,0.1f };
	auto collider = GetComponent<ColliderComponent>();

	collider->Initialize(&obbDesc);
	collider->SetBoneIndex(model->GetBoneIndex("Bone012"));

	ChangeState(&m_PoisonousGhostIdle);

	/*모델 세팅 이후에 무기 초기화 해야함*/
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	return S_OK;
}

void PoisonousGhost::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void PoisonousGhost::Update(_float dt)
{
	__super::Update(dt);
}

void PoisonousGhost::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);


}

void PoisonousGhost::Reload()
{
	if(m_CurrState == &m_PoisonousGhostIdle)
		ChangeState(&m_PoisonousGhostReload);
}

void PoisonousGhost::Fire()
{
	auto animator = GetComponent<AnimatorComponent>();

	if (m_CurrState == &m_PoisonousGhostIdle)
		ChangeState(&m_PoisonousGhostFire1);
	else if (m_CurrState == &m_PoisonousGhostFire1 && animator->GetProgress() >= 0.8f)
		ChangeState(&m_PoisonousGhostFire2);
	else if (m_CurrState == &m_PoisonousGhostFire2 && animator->GetProgress() >= 0.8f)
		ChangeState(&m_PoisonousGhostFire3);
}

void PoisonousGhost::Skill()
{
	if (m_CurrState == &m_PoisonousGhostIdle)
		ChangeState(&m_PoisonousGhostSkill);
}

Object* PoisonousGhost::Clone(InitDESC* arg)
{
	PoisonousGhost* Instance = new PoisonousGhost(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void PoisonousGhost::Free()
{
	__super::Free();
}

void PoisonousGhost::PoisonousGhostIdle::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Idle), true, false);
}

void PoisonousGhost::PoisonousGhostIdle::Update(Object* object, _float dt)
{
}

void PoisonousGhost::PoisonousGhostIdle::TestForExit(Object* object)
{
}

void PoisonousGhost::PoisonousGhostReload::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Reload), false, true);
}

void PoisonousGhost::PoisonousGhostReload::Update(Object* object, _float dt)
{
}

void PoisonousGhost::PoisonousGhostReload::TestForExit(Object* object)
{
	auto ghost = static_cast<PoisonousGhost*>(object);
	auto animator = object->GetComponent<AnimatorComponent>();

	if (animator->IsFinished())
	{
		ghost->ChangeState(&ghost->m_PoisonousGhostIdle);
	}
}

void PoisonousGhost::PoisonousGhostFire1::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Fire1), false, false);
}

void PoisonousGhost::PoisonousGhostFire1::Update(Object* object, _float dt)
{
}

void PoisonousGhost::PoisonousGhostFire1::TestForExit(Object* object)
{
	auto ghost = static_cast<PoisonousGhost*>(object);
	auto animator = object->GetComponent<AnimatorComponent>();

	_float progress = animator->GetProgress();
	if (progress>=0.9f)
	{
		ghost->ChangeState(&ghost->m_PoisonousGhostFire1Return);
	}
}

void PoisonousGhost::PoisonousGhostFire1Return::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Fire1Return), false, true);
}

void PoisonousGhost::PoisonousGhostFire1Return::Update(Object* object, _float dt)
{
}

void PoisonousGhost::PoisonousGhostFire1Return::TestForExit(Object* object)
{
	auto ghost = static_cast<PoisonousGhost*>(object);
	auto animator = object->GetComponent<AnimatorComponent>();

	_float progress = animator->GetProgress();
	if (progress>=0.9f)
	{
		ghost->ChangeState(&ghost->m_PoisonousGhostIdle);
	}
}

void PoisonousGhost::PoisonousGhostFire2::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Fire2), false, false);
}

void PoisonousGhost::PoisonousGhostFire2::Update(Object* object, _float dt)
{
}

void PoisonousGhost::PoisonousGhostFire2::TestForExit(Object* object)
{
	auto ghost = static_cast<PoisonousGhost*>(object);
	auto animator = object->GetComponent<AnimatorComponent>();

	_float progress = animator->GetProgress();
	if (progress >= 0.9f)
	{
		ghost->ChangeState(&ghost->m_PoisonousGhostFire2Return);
	}
}

void PoisonousGhost::PoisonousGhostFire2Return::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Fire2Return), false, true);
}


void PoisonousGhost::PoisonousGhostFire2Return::Update(Object* object, _float dt)
{
}

void PoisonousGhost::PoisonousGhostFire2Return::TestForExit(Object* object)
{
	auto ghost = static_cast<PoisonousGhost*>(object);
	auto animator = object->GetComponent<AnimatorComponent>();

	_float progress = animator->GetProgress();
	if (progress >= 0.9f)
	{
		ghost->ChangeState(&ghost->m_PoisonousGhostIdle);
	}
}

void PoisonousGhost::PoisonousGhostFire3::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Fire3), false, false);
}

void PoisonousGhost::PoisonousGhostFire3::Update(Object* object, _float dt)
{
}

void PoisonousGhost::PoisonousGhostFire3::TestForExit(Object* object)
{
	auto ghost = static_cast<PoisonousGhost*>(object);
	auto animator = object->GetComponent<AnimatorComponent>();

	_float progress = animator->GetProgress();
	if (progress >= 0.9f)
	{
		ghost->ChangeState(&ghost->m_PoisonousGhostFire3Return);
	}
}

void PoisonousGhost::PoisonousGhostFire3Return::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Fire3Return), false, true);
}

void PoisonousGhost::PoisonousGhostFire3Return::Update(Object* object, _float dt)
{
}

void PoisonousGhost::PoisonousGhostFire3Return::TestForExit(Object* object)
{
	auto ghost = static_cast<PoisonousGhost*>(object);
	auto animator = object->GetComponent<AnimatorComponent>();

	_float progress = animator->GetProgress();
	if (progress >= 0.9f)
	{
		ghost->ChangeState(&ghost->m_PoisonousGhostIdle);
	}
}

void PoisonousGhost::PoisonousGhostSkill::Enter(Object* object)
{
	auto animator = object->GetComponent<AnimatorComponent>();
	animator->ChangeAnimation(ENUM_CLASS(AnimationState::Skill), false, true);
}

void PoisonousGhost::PoisonousGhostSkill::Update(Object* object, _float dt)
{
}

void PoisonousGhost::PoisonousGhostSkill::TestForExit(Object* object)
{
	auto ghost = static_cast<PoisonousGhost*>(object);
	auto animator = object->GetComponent<AnimatorComponent>();

	_float progress = animator->GetProgress();
	if (progress >= 0.9f)
	{
		ghost->ChangeState(&ghost->m_PoisonousGhostIdle);
	}
}
