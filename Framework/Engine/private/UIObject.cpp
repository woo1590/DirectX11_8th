#include "EnginePCH.h"
#include "UIObject.h"
#include "EngineCore.h"
#include "SpriteComponent.h"

UIObject::UIObject()
	:Object()
{
}

UIObject::UIObject(const UIObject& prototype)
	:Object(prototype),
	m_fX(prototype.m_fX),
	m_fY(prototype.m_fY),
	m_fSizeX(prototype.m_fSizeX),
	m_fSizeY(prototype.m_fSizeY),
	m_fViewportWidth(prototype.m_fViewportWidth),
	m_fViewportHeight(prototype.m_fViewportHeight)
{
}

HRESULT UIObject::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	D3D11_VIEWPORT viewportDesc = EngineCore::GetInstance()->GetViewport();
	m_fViewportWidth = viewportDesc.Width;
	m_fViewportHeight = viewportDesc.Height;

	return S_OK;
}

HRESULT UIObject::Initialize(InitDESC* arg)
{
	if (arg)
	{
		UIObjectDesc* desc = static_cast<UIObjectDesc*>(arg);
		m_fX = desc->x;
		m_fY = desc->y;
		m_fSizeX = desc->sizeX;
		m_fSizeY = desc->sizeY;
		m_pParent = desc->parent;
		m_iPriority = desc->priority;

		if (FAILED(__super::Initialize(arg)))
			return E_FAIL;

		if (!m_pParent)
			UpdateTransform();
		else
		{
			m_pTransform->SetParent(m_pParent->GetComponent<TransformComponent>());
			m_pTransform->SetUseParentScale(false);
		}
	}

	return S_OK;
}

void UIObject::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);

	for (const auto& child : m_Childrens)
	{
		if (child)
			child->PriorityUpdate(dt);
	}
}

void UIObject::Update(_float dt)
{
	__super::Update(dt);

	for (const auto& child : m_Childrens)
	{
		if (child)
			child->Update(dt);
	}
}

void UIObject::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);

	for (const auto& child : m_Childrens)
	{
		if (child)
			child->LateUpdate(dt);
	}

	if (m_isDirty && !m_pParent)
		UpdateTransform();
}

HRESULT UIObject::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	auto sprite = GetComponent<SpriteComponent>();

	if (!sprite)
		return S_OK;

	sprite->ExtractRenderProxy(m_pTransform, proxies[ENUM_CLASS(RenderGroup::UI)]);

	for (const auto& child : m_Childrens)
		if(child)
			child->ExtractRenderProxies(proxies);

	return S_OK;
}

void UIObject::Free()
{
	__super::Free();

	for (auto& child : m_Childrens)
		Safe_Release(child);
	m_Childrens.clear();
}

void UIObject::UpdateTransform()
{
	_float3 position{ m_fX - m_fViewportWidth * 0.5f,-m_fY + m_fViewportHeight * 0.5f,0.f };
	_float3 scale{ m_fSizeX,m_fSizeY,1.f };

	m_pTransform->SetPosition(position);
	m_pTransform->SetScale(scale);

	m_isDirty = false;
}
