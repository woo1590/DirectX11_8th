#include "pch.h"
#include "SelectHome.h"

//component
#include "ModelComponent.h"
#include "AnimatorComponent.h"

SelectHome::SelectHome()
	:Object()
{
}

SelectHome::SelectHome(const SelectHome& prototype)
	:Object(prototype)
{
}

SelectHome* SelectHome::Create()
{
	SelectHome* Instance = new SelectHome();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT SelectHome::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	AddComponent<AnimatorComponent>();

	return S_OK;
}

HRESULT SelectHome::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	auto model = GetComponent<ModelComponent>();
	model->Initialize(nullptr);
	model->SetModel(ENUM_CLASS(LevelID::Static), "Model_SelectHome");

	auto animator = GetComponent<AnimatorComponent>();
	animator->SetAnimation(ENUM_CLASS(LevelID::Static), "AnimationSet_SelectHome");

	model->ConnectAnimator();

	m_pTransform->SetScale(_float3{ 6.2f,6.2f,6.2f });
	m_UseShadow = true;

	return S_OK;
}

void SelectHome::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void SelectHome::Update(_float dt)
{
	__super::Update(dt);
}

void SelectHome::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* SelectHome::Clone(InitDESC* arg)
{
	SelectHome* Instance = new SelectHome(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void SelectHome::Free()
{
	__super::Free();
}
