#include "pch.h"	
#include "MiniMapPanel.h"
#include "MinimapTime.h"
#include "MinimapLevel.h"

//component
#include "SpriteComponent.h"

MiniMapPanel::MiniMapPanel()
	:UIObject()
{
}

MiniMapPanel::MiniMapPanel(const MiniMapPanel& prototype)
	:UIObject(prototype)
{
}

MiniMapPanel* MiniMapPanel::Create()
{
	MiniMapPanel* Instance = new MiniMapPanel();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT MiniMapPanel::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT MiniMapPanel::Initialize(InitDESC* arg)
{
	UIObject::UIOBJECT_DESC desc{};
	desc.sizeX = 261.f * 0.8f;
	desc.sizeY = 273.f * 0.8f;
	desc.x = WinSizeX * 0.5f - WinSizeX * 0.42f;
	desc.y = WinSizeY * 0.5f - WinSizeY * 0.37f;

	if (FAILED(__super::Initialize(&desc)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.iMaxFrameIndex = 1;
	spriteDesc.isAnimated = false;
	auto sprite = GetComponent<SpriteComponent>();
	sprite->Initialize(&spriteDesc);
	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_MinimapPanel");

	m_Childrens.resize(ENUM_CLASS(Parts::Count));
	if (FAILED(CreateChildren()))
		return E_FAIL;

	return S_OK;
}

void MiniMapPanel::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void MiniMapPanel::Update(_float dt)
{
	__super::Update(dt);
}

void MiniMapPanel::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* MiniMapPanel::Clone(InitDESC* arg)
{
	MiniMapPanel* Instance = new MiniMapPanel(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void MiniMapPanel::Free()
{
	__super::Free();
}

HRESULT MiniMapPanel::CreateChildren()
{
	auto engine = EngineCore::GetInstance();

	_float3 scale = m_pTransform->GetScale();

	/*add minimap time*/
	{
		UIObject::UIOBJECT_DESC desc{};
		desc.parent = this;
		desc.position = _float3{ 0.f,-scale.y * 0.42f,0.f };
		desc.scale = _float3{ 160.f * 0.8f,40.f * 0.8f,1.f };
		Object* time = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_MinimapTime", &desc);

		m_Childrens[ENUM_CLASS(Parts::Time)] = static_cast<UIObject*>(time);
	}
	/*add minimap level*/
	{
		UIObject::UIOBJECT_DESC desc{};
		desc.parent = this;
		desc.position = _float3{ scale.x * 0.5f + scale.x * 0.13f,scale.y * 0.3f,0.f };
		desc.scale = _float3{ 324 * 0.6f,54.f * 0.8f,1.f };
		Object* level = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_MinimapLevel", &desc);

		m_Childrens[ENUM_CLASS(Parts::Level)] = static_cast<UIObject*>(level);
	}
	/*add minimap area*/
	{
		UIObject::UIOBJECT_DESC desc{};
		desc.parent = this;
		desc.position = _float3{ 0.f,4.f,0.f };
		desc.scale = _float3{ 256.f * 0.58f,256.f * 0.58f,1.f };
		Object* area = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_MinimapArea", &desc);

		m_Childrens[ENUM_CLASS(Parts::Area)] = static_cast<UIObject*>(area);
	}

	return S_OK;
}
