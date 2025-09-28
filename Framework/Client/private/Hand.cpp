#include "pch.h"
#include "Hand.h"
#include "ModelComponent.h"
#include "AnimatorComponent.h"

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
	animator->ChangeAnimation(1, true);

	return S_OK;
}

void Hand::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Hand::Update(_float dt)
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
			animator->ChangeAnimation(2, true);
	}
}

void Hand::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT Hand::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	auto model = GetComponent<ModelComponent>();
	if (!model)
		return S_OK;

	return model->ExtractRenderProxy(m_pTransform, proxies[ENUM_CLASS(RenderGroup::NonBlend)]);
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
