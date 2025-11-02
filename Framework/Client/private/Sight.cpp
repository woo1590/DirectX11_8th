#include "pch.h"
#include "Sight.h"

//component
#include "SpriteComponent.h"

Sight::Sight()
	:UIObject()
{
}

Sight::Sight(const Sight& prototype)
	:UIObject(prototype)
{
}

Sight* Sight::Create()
{
	Sight* Instance = new Sight();

	if(FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Sight::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT Sight::Initialize(InitDESC* arg)
{
	UIObject::UIOBJECT_DESC desc{};
	desc.sizeX = 20.f;
	desc.sizeY = 20.f;
	desc.x = WinSizeX * 0.5f;
	desc.y = WinSizeY * 0.5f;

	if (FAILED(__super::Initialize(&desc)))
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
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_Sight");

	return S_OK;
}

void Sight::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Sight::Update(_float dt)
{
	__super::Update(dt);
}

void Sight::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* Sight::Clone(InitDESC* arg)
{
	Sight* Instance = new Sight(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Sight::Free()
{
	__super::Free();
}
