#include "pch.h"
#include "Hand.h"

//component
#include "TransformComponent.h"
#include "AnimatorComponent.h"
#include "ModelComponent.h"

Hand::Hand()
	:PartObject()
{
}

Hand::Hand(const Hand& prototype)
	:PartObject(prototype)
{
}

Hand* Hand::Create()
{
	Hand* Instance = new Hand();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Hand::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<AnimatorComponent>();
	AddComponent<ModelComponent>();
	m_strInstanceTag = "Hand";
	m_eRenderGroup = RenderGroup::NonBlend;

	return S_OK;
}

HRESULT Hand::Initialize(InitDESC* arg)
{
	HAND_DESC* desc = static_cast<HAND_DESC*>(arg);

	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::GamePlay), "Model_PlayerHand");

	auto animator = GetComponent<AnimatorComponent>();
	animator->SetAnimation(ENUM_CLASS(LevelID::GamePlay), "AnimationSet_PlayerHand");

	model->ConnectAnimator();

	ChangeState(&m_HandIdle);

	return S_OK;
}

void Hand::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Hand::Update(_float dt)
{
	__super::Update(dt);

	
}

void Hand::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* Hand::Clone(InitDESC* arg)
{
	Hand* Instance = new Hand(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Hand::Free()
{
	__super::Free();
}
