#include "pch.h"	
#include "Torch.h"
#include "EffectContainer.h"

//compnent
#include "LightComponent.h"

Torch::Torch()
	:Object()
{
}

Torch::Torch(const Torch& prototype)
	:Object(prototype)
{
}

Torch* Torch::Create()
{
	Torch* Instance = new Torch();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Torch::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<LightComponent>();

	return S_OK;
}

HRESULT Torch::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();
	TORCH_DESC* desc = static_cast<TORCH_DESC*>(arg);

	/*light*/
	LightComponent::LIGHT_DESC lightDesc{};
	lightDesc.type = LightType::Point;
	lightDesc.color = desc->color;
	lightDesc.range = desc->range;

	auto light = GetComponent<LightComponent>();
	light->Initialize(&lightDesc);

	engine->RegisterLight(light);

	EffectContainer::EFFECT_CONTAINER_DESC effectDesc{};
	m_pTorchFireEffect = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_TorchFire", &effectDesc);
	m_pTorchFireEffect->GetComponent<TransformComponent>()->SetParent(m_pTransform);
	m_pTorchFireEffect->GetComponent<TransformComponent>()->SetPosition(_float3{ 0.f,3.f,0.f });


	return S_OK;
}

void Torch::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);

	if (m_pTorchFireEffect)
		m_pTorchFireEffect->PriorityUpdate(dt);
}

void Torch::Update(_float dt)
{
	__super::Update(dt);

	if (m_pTorchFireEffect)
		m_pTorchFireEffect->Update(dt);
}

void Torch::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT Torch::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	__super::ExtractRenderProxies(proxies);

	if (m_pTorchFireEffect)
		m_pTorchFireEffect->ExtractRenderProxies(proxies);

	return S_OK;
}

Object* Torch::Clone(InitDESC* arg)
{
	Torch* Instance = new Torch(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Torch::Free()
{
	EngineCore::GetInstance()->UnRegisterLight(GetComponent<LightComponent>());

	__super::Free();
	Safe_Release(m_pTorchFireEffect);
}
