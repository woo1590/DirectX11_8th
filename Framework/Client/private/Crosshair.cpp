#include "pch.h"
#include "Crosshair.h"

//component
#include "SpriteComponent.h"

Crosshair::Crosshair()
	:UIObject()
{
}

Crosshair::Crosshair(const Crosshair& prototype)
	:UIObject(prototype)
{
}

Crosshair* Crosshair::Create()
{
	Crosshair* Instance = new Crosshair();

	if(FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Crosshair::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT Crosshair::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.fSpeed = 0.f;
	spriteDesc.iMaxFrameIndex = 1;
	spriteDesc.isAnimated = false;
	spriteDesc.isRepeat = false;

	auto sprite = GetComponent<SpriteComponent>();
	if (FAILED(sprite->Initialize(&spriteDesc)))
		return E_FAIL;

	m_pTransform->SetScale(_float3{ 22.f ,22.f,1.f });
	m_isDirty = false;
	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_Sight");

	return S_OK;
}

void Crosshair::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Crosshair::Update(_float dt)
{
	__super::Update(dt);
}

void Crosshair::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* Crosshair::Clone(InitDESC* arg)
{
	Crosshair* Instance = new Crosshair(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Crosshair::Free()
{
	__super::Free();
}
