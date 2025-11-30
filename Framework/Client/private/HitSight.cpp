#include "pch.h"
#include "HitSight.h"
#include "MaterialInstance.h"

//componenet
#include "SpriteComponent.h"

HitSight::HitSight()
	:UIObject()
{
}

HitSight::HitSight(const HitSight& prototype)
	:UIObject(prototype)
{
}

HitSight* HitSight::Create()
{
	HitSight* Instance = new HitSight();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT HitSight::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT HitSight::Initialize(InitDESC* arg)
{
	UIObject::UIOBJECT_DESC uiDesc{};
	uiDesc.x = WinSizeX * 0.5f;
	uiDesc.y = WinSizeY * 0.5f;
	uiDesc.sizeX = 150.f * 0.2f;
	uiDesc.sizeY = 150.f * 1.2f;

	if (FAILED(__super::Initialize(&uiDesc)))
		return E_FAIL;

	HIT_SIGHT_DESC* desc = static_cast<HIT_SIGHT_DESC*>(arg);
	m_HitWorldPosition = desc->hitWorldPosition;

	auto engine = EngineCore::GetInstance();

	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	auto sprite = GetComponent<SpriteComponent>();
	sprite->Initialize(&spriteDesc);
	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_HitSight");

	auto mtrlInstance = sprite->GetMaterialInstance();
	mtrlInstance->SetPass("HitSight_Pass");
	mtrlInstance->SetFloat4("g_Color", _float4{ 1.f,0.f,0.f,1.f });
	mtrlInstance->SetFloat("g_CustomAlpha", 0.f);

	return S_OK;
}

void HitSight::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void HitSight::Update(_float dt)
{
	__super::Update(dt);

	m_fElpasedTime += dt;
	if (m_fElpasedTime >= m_fAlphaDuration)
	{
		SetDead();
		return;
	}

	auto mtrlInstance = GetComponent<SpriteComponent>()->GetMaterialInstance();

	_float t = m_fElpasedTime / m_fAlphaDuration;
	t = math::EaseOutQuint(t);
	t = math::PalabolaCurve(t);
	mtrlInstance->SetFloat("g_CustomAlpha", t);
}

void HitSight::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);

	CAMERA_CONTEXT camContext = EngineCore::GetInstance()->GetCameraContext();

	_float4x4 viewMatrixInverse = camContext.viewMatrixInverse;
	_float3 camPosition = camContext.camPosition;
	_float3 camForward{ viewMatrixInverse._31,viewMatrixInverse._32,viewMatrixInverse._33 };
	_float3 camRight{ viewMatrixInverse._11,viewMatrixInverse._12,viewMatrixInverse._13 };
	_float3 hitDir{};
	XMStoreFloat3(&hitDir, XMVector3Normalize(XMLoadFloat3(&m_HitWorldPosition) - XMLoadFloat3(&camPosition)));
	XMStoreFloat3(&camForward, XMVector3Normalize(XMLoadFloat3(&camForward)));
	XMStoreFloat3(&camRight, XMVector3Normalize(XMLoadFloat3(&camRight)));

	_float2 screenDir{};
	screenDir.x = XMVectorGetX(XMVector3Dot(XMLoadFloat3(&hitDir), XMLoadFloat3(&camRight)));
	screenDir.y = XMVectorGetX(XMVector3Dot(XMLoadFloat3(&hitDir), XMLoadFloat3(&camForward)));
	XMStoreFloat2(&screenDir, XMVector2Normalize(XMLoadFloat2(&screenDir)));

	_float t = m_fElpasedTime / m_fPosDuration;
	t = math::EaseOutQuint(t);
	_float3 startPos{ screenDir.x * m_fStartRadius,screenDir.y * m_fStartRadius,0.f };
	_float3 endPos{ screenDir.x * m_fEndRadius,screenDir.y * m_fEndRadius,0.f };
	_float3 position{};
	XMStoreFloat3(&position, XMVectorLerp(XMLoadFloat3(&startPos), XMLoadFloat3(&endPos), t));
	_float rotZ = std::atan2(screenDir.y, screenDir.x);

	m_pTransform->SetPosition(position);
	m_pTransform->Rotate(_float3{ 0.f,0.f,rotZ });
}

Object* HitSight::Clone(InitDESC* arg)
{
	HitSight* Instance = new HitSight(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void HitSight::Free()
{
	__super::Free();
}
