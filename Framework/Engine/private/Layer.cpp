#include "EnginePCH.h"
#include "Layer.h"
#include "Object.h"

Layer::Layer()
{
}

Layer* Layer::Create()
{
	Layer* Instance = new Layer();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Layer::Initialize()
{
	m_Objects.clear();

	return S_OK;
}

HRESULT Layer::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	for (const auto& object : m_Objects)
	{
		if (FAILED(object->ExtractRenderProxies(proxies)))
			return E_FAIL;
	}

	return S_OK;
}

Object* Layer::GetFrontObject() const
{
	if (m_Objects.empty())
		return nullptr;

	return m_Objects.front();
}

Object* Layer::GetLastObject() const
{
	if (m_Objects.empty())
		return nullptr;

	return m_Objects.back();
}

Object* Layer::GetObjectByInstanceTag(const _string& instanceTag)
{
	for (const auto& object : m_Objects)
	{
		if (object->GetInstanceTag() == instanceTag)
			return object;
	}

	return nullptr; /*Can't find object by instance tag*/
}

void Layer::Free()
{
	__super::Free();

	for (auto& object : m_Objects)
		Safe_Release(object);
}

void Layer::AddObject(Object* object)
{
	m_Objects.push_back(object);
}

void Layer::PriorityUpdate(_float dt)
{
	for (const auto& object : m_Objects)
		object->PriorityUpdate(dt);
}

void Layer::Update(_float dt)
{
	for (const auto& object : m_Objects)
		object->Update(dt);
}

void Layer::LateUpdate(_float dt)
{
	for (const auto& object : m_Objects)
		object->LateUpdate(dt);
}

void Layer::RemoveDeadObjects()
{
	for (auto iter = m_Objects.begin(); iter != m_Objects.end();)
	{
		if ((*iter)->IsDead())
		{
			Safe_Release(*iter);
			iter = m_Objects.erase(iter);
		}
		else ++iter;
	}
}
