#include "pch.h"
#include "PlayerIcon.h"

//component
#include "SpriteComponent.h"

PlayerIcon::PlayerIcon()
	:UIObject()
{
}

PlayerIcon::PlayerIcon(const PlayerIcon& prototype)
	:UIObject(prototype)
{
}

PlayerIcon* PlayerIcon::Create()
{
	PlayerIcon* Instance = new PlayerIcon();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT PlayerIcon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT PlayerIcon::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.isAnimated = false;
	spriteDesc.iMaxFrameIndex = 1;

	auto sprite = GetComponent<SpriteComponent>();
	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_PlayerIcon");

	_float3 parentScale = m_pParent->GetComponent<TransformComponent>()->GetScale();
	_float3 position{};
	position.x = parentScale.x * -0.5f + parentScale.x * 0.2f;
	position.y = parentScale.y * 0.5f;
	position.z = 1.f;

	m_fSizeX = WinSizeX * 0.15f;
	m_fSizeY = m_fSizeX;

	m_pTransform->SetPosition(position);
	m_pTransform->SetScale(_float3{ m_fSizeX,m_fSizeY,1.f });

	return S_OK;
}

void PlayerIcon::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void PlayerIcon::Update(_float dt)
{
	__super::Update(dt);
}

void PlayerIcon::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* PlayerIcon::Clone(InitDESC* arg)
{
	PlayerIcon* Instance = new PlayerIcon(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void PlayerIcon::Free()
{
	__super::Free();
}
