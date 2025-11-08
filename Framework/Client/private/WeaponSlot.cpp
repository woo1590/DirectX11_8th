#include "pch.h"
#include "WeaponSlot.h"
#include "MaterialInstance.h"

//component
#include "SlotNum.h"
#include "SpriteComponent.h"

WeaponSlot::WeaponSlot()
	:UIObject()
{
}

WeaponSlot::WeaponSlot(const WeaponSlot& prototype)
	:UIObject(prototype)
{
}

WeaponSlot* WeaponSlot::Create()
{
	WeaponSlot* Instance = new WeaponSlot();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT WeaponSlot::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT WeaponSlot::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	WEAPON_SLOT_DESC* desc = static_cast<WEAPON_SLOT_DESC*>(arg);

	auto engine = EngineCore::GetInstance();

	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.isAnimated = false;
	spriteDesc.iMaxFrameIndex = 1;

	auto sprite = GetComponent<SpriteComponent>();
	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	sprite->Initialize(&spriteDesc);

	auto mtrlInstance = sprite->GetMaterialInstance();
	mtrlInstance->SetFloat("g_CustomAlpha", 0.5f);
	mtrlInstance->SetPass("CustomAlpha_Pass");

	switch (desc->slotNum)
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

	m_fSizeX = 48.f * 1.2f;
	m_fSizeY = 41.f * 1.2f;
	m_StartScale = _float3{ m_fSizeX,m_fSizeY,1.f };
	XMStoreFloat3(&m_TargetScale, XMLoadFloat3(&m_StartScale) * 1.5f);

	m_pTransform->SetScale(m_StartScale);

	return S_OK;
}

void WeaponSlot::SetActive(_bool active)
{
	if (active == m_IsActive)
		return;

	auto mtrlInstance = GetComponent<SpriteComponent>()->GetMaterialInstance();

	m_fElapsedTime = 0.f;
	m_IsActive = active;

	if (active)
	{
		mtrlInstance->SetPass("Default_Pass");
	}
	else
	{
		mtrlInstance->SetFloat("g_CustomAlpha", 0.5f);
		mtrlInstance->SetPass("CustomAlpha_Pass");
	}
}

void WeaponSlot::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void WeaponSlot::Update(_float dt)
{
	__super::Update(dt);

	if (m_fElapsedTime < m_fDuration)
	{
		m_fElapsedTime += dt;

		_float t = m_fElapsedTime / m_fDuration;
		t = std::clamp(t, 0.f, 1.f);

		/*start -> target*/
		if (m_IsActive)
		{
			_float3 currScale{};
			XMStoreFloat3(&currScale, XMVectorLerp(XMLoadFloat3(&m_StartScale), XMLoadFloat3(&m_TargetScale), t));
			m_pTransform->SetScale(currScale);
		}
		/*target -> start*/
		else
		{
			_float3 currScale{};
			XMStoreFloat3(&currScale, XMVectorLerp(XMLoadFloat3(&m_TargetScale), XMLoadFloat3(&m_StartScale), t));
			m_pTransform->SetScale(currScale);
		}

	}
}

void WeaponSlot::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* WeaponSlot::Clone(InitDESC* arg)
{
	WeaponSlot* Instance = new WeaponSlot(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void WeaponSlot::Free()
{
	__super::Free();
}
