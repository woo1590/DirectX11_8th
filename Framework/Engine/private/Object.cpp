#include "EnginePCH.h"
#include "Object.h"
#include "TransformComponent.h"
#include "ModelComponent.h"
#include "SpriteComponent.h"
#include "ColliderComponent.h"

_uint Object::m_iInstanceID = 0;

Object::Object()
{
}

Object::Object(const Object& prototype)
	:m_strInstanceTag(prototype.m_strInstanceTag),
	m_eRenderGroup(prototype.m_eRenderGroup)
{
	for (const auto& pair : prototype.m_ComponentMap)
	{
		auto comp = pair.second->Clone();
		comp->SetOwner(this);

		m_Components.push_back(comp);
		m_ComponentMap[pair.first] = comp;
		comp->AddRef();
	}

	m_pTransform = GetComponent<TransformComponent>();
	m_pTransform->AddRef();
}

HRESULT Object::Initialize_Prototype()
{
	m_Components.clear();
	m_ComponentMap.clear();

	m_pTransform = AddComponent<TransformComponent>();
	if (!m_pTransform)
		return E_FAIL;
	
	m_pTransform->AddRef();

	return S_OK;
}

HRESULT Object::Initialize(InitDESC* arg)
{
	m_strInstanceTag = m_strInstanceTag + '#' + std::to_string(m_iInstanceID++);

	if (arg)
	{
		if (FAILED(m_pTransform->Initialize(arg)))
			return E_FAIL;
	}

	return S_OK;
}

void Object::PriorityUpdate(_float dt)
{
	for (const auto& comp : m_Components)
		comp->PriorityUpdate(dt);
}

void Object::Update(_float dt)
{
	for (const auto& comp : m_Components)
		comp->Update(dt);

	m_CurrState->Update(this, dt);
	m_CurrState->TestForExit(this);
}

void Object::LateUpdate(_float dt)
{
	for (const auto& comp : m_Components)
		comp->LateUpdate(dt);
}

HRESULT Object::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	auto model = GetComponent<ModelComponent>();
	auto sprite = GetComponent<SpriteComponent>();
	auto collider = GetComponent<ColliderComponent>();

	/*for debug*/
	if (collider)
	{
		RenderProxy proxy{};
		proxy.collider = collider;

		proxies[ENUM_CLASS(RenderGroup::ColliderDebug)].push_back(proxy);
	}

	if (!model && !sprite)
		return S_OK;

	if (model)
		return model->ExtractRenderProxy(m_pTransform, proxies[ENUM_CLASS(m_eRenderGroup)]);
	else if (sprite)
		return sprite->ExtractRenderProxy(m_pTransform, proxies[ENUM_CLASS(m_eRenderGroup)]);

	return S_OK;
}

void Object::Free()
{
	for (auto& comp : m_Components)
		Safe_Release(comp);

	for (auto& [key, comp] : m_ComponentMap)
		Safe_Release(comp);

	Safe_Release(m_pTransform);

	m_Components.clear();
	m_ComponentMap.clear();
}

#ifdef USE_IMGUI

void Object::RenderInspector()
{
	for (const auto& comp : m_Components)
		comp->RenderInspector();
}

#endif

void Object::ChangeState(State* state)
{
	m_CurrState = state;
	m_CurrState->Enter(this);
}
