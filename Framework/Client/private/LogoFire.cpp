#include "pch.h"
#include "LogoFire.h"

//component
#include "SpriteComponent.h"

LogoFire::LogoFire()
	:UIObject()
{
}

LogoFire::LogoFire(const LogoFire& prototype)
	:UIObject(prototype)
{
}

LogoFire* LogoFire::Create()
{
	LogoFire* Instance = new LogoFire();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT LogoFire::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT LogoFire::Initialize(InitDESC* arg)
{
	UIObjectDesc uiDesc{};
	uiDesc.x = WinSizeX * 0.92f;
	uiDesc.y = WinSizeY * 0.7f;
	uiDesc.sizeX = 163.f * 1.4f;
	uiDesc.sizeY = 172.f * 1.4f;

	if (FAILED(__super::Initialize(&uiDesc)))
		return E_FAIL;

	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.fSpeed = 20.f;
	spriteDesc.iMaxFrameIndex = 30;
	spriteDesc.isAnimated = true;
	spriteDesc.isRepeat = true;

	auto sprite = GetComponent<SpriteComponent>();
	if (FAILED(sprite->Initialize(&spriteDesc)))
		return E_FAIL;

	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_Logo_Fire");

	return S_OK;
}

void LogoFire::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void LogoFire::Update(_float dt)
{
	__super::Update(dt);
}

void LogoFire::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* LogoFire::Clone(InitDESC* arg)
{
	LogoFire* Instance = new LogoFire(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void LogoFire::Free()
{
	__super::Free();
}
