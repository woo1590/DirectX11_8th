#include "pch.h"
#include "MuzzleFire.h"

//component
#include "SpriteComponent.h"

MuzzleFire::MuzzleFire()
	:Object()
{
}

MuzzleFire::MuzzleFire(const MuzzleFire& prototype)
	:Object(prototype)
{
}

MuzzleFire* MuzzleFire::Create()
{
	MuzzleFire* Instance = new MuzzleFire();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT MuzzleFire::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT MuzzleFire::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.fSpeed = 100.f;
	spriteDesc.iMaxFrameIndex = 2;
	spriteDesc.isAnimated = true;
	spriteDesc.isRepeat = false;

	auto sprite = GetComponent<SpriteComponent>();
	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Point");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_MuzzleFire");

	return S_OK;
}

void MuzzleFire::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void MuzzleFire::Update(_float dt)
{
	__super::Update(dt);

	auto sprite = GetComponent<SpriteComponent>();
}

void MuzzleFire::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* MuzzleFire::Clone(InitDESC* arg)
{
	MuzzleFire* Instance = new MuzzleFire(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void MuzzleFire::Free()
{
	__super::Free();
}
