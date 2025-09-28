#include "pch.h"
#include "Cameleon.h"

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
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::GamePlay), "Model_Cameleon");

	auto animator = GetComponent<AnimatorComponent>();
	animator->SetAnimation(ENUM_CLASS(LevelID::GamePlay), "AnimationSet_Cameleon");

	model->ConnectAnimator();
	animator->ChangeAnimation(0, true);

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
