#include "pch.h"
#include "SlotNum.h"

//component
#include "SpriteComponent.h"

SlotNum::SlotNum()
	:UIObject()
{
}

SlotNum::SlotNum(const SlotNum& prototype)
	:UIObject(prototype)
{
}

SlotNum* SlotNum::Create()
{
	SlotNum* Instance = new SlotNum();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT SlotNum::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT SlotNum::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	SLOT_NUM_DESC* desc = static_cast<SLOT_NUM_DESC*>(arg);

	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.isAnimated = false;
	spriteDesc.iMaxFrameIndex = 1;

	auto sprite = GetComponent<SpriteComponent>();
	sprite->Initialize(&spriteDesc);

	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");

	switch (desc->num)
	{
	case 1:
		sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_WeaponSlot_Num1");
		break;
	case 2:
		sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_WeaponSlot_Num2");
		break;
	case 3:
		sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_WeaponSlot_Num3");
		break;
	default:
		break;
	}
	
	m_fSizeX = 16.f * 1.2 * 2.f;
	m_fSizeY = m_fSizeX;
	m_pTransform->SetScale(_float3{ m_fSizeX,m_fSizeY,1.f });

	return S_OK;
}

void SlotNum::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void SlotNum::Update(_float dt)
{
	__super::Update(dt);
}

void SlotNum::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* SlotNum::Clone(InitDESC* arg)
{
	SlotNum* Instance = new SlotNum(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void SlotNum::Free()
{
	__super::Free();
}

