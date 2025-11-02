#include "pch.h"
#include "BackGround.h"
#include "Material.h"
#include "SpriteComponent.h"

BackGround::BackGround()
{
}

BackGround* BackGround::Create()
{
	BackGround* Instance = new BackGround();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT BackGround::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT BackGround::Initialize(InitDESC* arg)
{
	UIObjectDesc uiDesc{};
	uiDesc.x = 640.f;
	uiDesc.y = 360.f;
	uiDesc.sizeX = 1280.f;
	uiDesc.sizeY = 720.f;

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
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_Background_Logo");

	return S_OK;
}

void BackGround::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void BackGround::Update(_float dt)
{
	__super::Update(dt);

}

void BackGround::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* BackGround::Clone(InitDESC* arg)
{
	BackGround* Instance = new BackGround(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void BackGround::Free()
{
	__super::Free();
}
