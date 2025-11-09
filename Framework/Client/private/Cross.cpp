#include "pch.h"
#include "Cross.h"
#include "MaterialInstance.h"

//component
#include "SpriteComponent.h"

Cross::Cross()
	:UIObject()
{
}

Cross::Cross(const Cross& prototype)
	:UIObject(prototype)
{
}

Cross* Cross::Create()
{
	Cross* Instance = new Cross();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Cross::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT Cross::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	CROSS_DESC* desc = static_cast<CROSS_DESC*>(arg);

	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.isAnimated = false;
	spriteDesc.iMaxFrameIndex = 1;
	spriteDesc.fSpeed = 0.f;
	spriteDesc.isRepeat = false;
	auto sprite = GetComponent<SpriteComponent>();
	sprite->Initialize(&spriteDesc);

	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_NumCross");

	auto mtrlInstance = sprite->GetMaterialInstance();
	mtrlInstance->SetPass("CountNumber_Pass");
	mtrlInstance->SetFloat4("g_Color", desc->color);

	return S_OK;
}

void Cross::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Cross::Update(_float dt)
{
	__super::Update(dt);
}

void Cross::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* Cross::Clone(InitDESC* arg)
{
	Cross* Instance = new Cross(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Cross::Free()
{
	__super::Free();

}
