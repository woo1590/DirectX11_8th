#include "pch.h"
#include "BossIcon.h"
#include "MaterialInstance.h"

//component
#include "SpriteComponent.h"

BossIcon::BossIcon()
	:UIObject()
{
}

BossIcon::BossIcon(const BossIcon& prototype)
	:UIObject(prototype)
{
}

BossIcon* BossIcon::Create()
{
	BossIcon* Instance = new BossIcon();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT BossIcon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT BossIcon::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	BOSS_ICON_DESC* desc = static_cast<BOSS_ICON_DESC*>(arg);

	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.isAnimated = false;
	spriteDesc.iMaxFrameIndex = 1;

	auto sprite = GetComponent<SpriteComponent>();
	sprite->Initialize(&spriteDesc);

	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_BossIcon");

	return S_OK;
}

void BossIcon::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void BossIcon::Update(_float dt)
{
	__super::Update(dt);
}

void BossIcon::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* BossIcon::Clone(InitDESC* arg)
{
	BossIcon* Instance = new BossIcon(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void BossIcon::Free()
{
	__super::Free();
}

