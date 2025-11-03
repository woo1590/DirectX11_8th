#include "pch.h"
#include "LogoText.h"

//component
#include "SpriteComponent.h"

LogoText::LogoText()
	:UIObject()
{
}

LogoText::LogoText(const LogoText& prototype)
	:UIObject(prototype)
{
}

LogoText* LogoText::Create()
{
	LogoText* Instance = new LogoText();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);
	
	return Instance;
}

HRESULT LogoText::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT LogoText::Initialize(InitDESC* arg)
{
	UIObjectDesc uiDesc{};
	uiDesc.x = WinSizeX * 0.85f;
	uiDesc.y = WinSizeY * 0.8f;
	uiDesc.sizeX = 235.f;
	uiDesc.sizeY = 95.f;

	if (FAILED(__super::Initialize(&uiDesc)))
		return E_FAIL;

	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.fSpeed = 0.f;
	spriteDesc.iMaxFrameIndex = 1;
	spriteDesc.isAnimated = false;
	spriteDesc.isRepeat = false;

	auto sprite = GetComponent<SpriteComponent>();
	if (FAILED(sprite->Initialize(&spriteDesc)))
		return E_FAIL;

	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_Logo_Text");

	return S_OK;
}

void LogoText::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void LogoText::Update(_float dt)
{
	__super::Update(dt);
}

void LogoText::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* LogoText::Clone(InitDESC* arg)
{
	LogoText* Instance = new LogoText(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void LogoText::Free()
{
	__super::Free();
}
