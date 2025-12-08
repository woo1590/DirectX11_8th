#include "pch.h"
#include "MouseCursor.h"

//component
#include "SpriteComponent.h"

MouseCursor::MouseCursor()
	:UIObject()
{
}

MouseCursor::MouseCursor(const MouseCursor& prototype)
	:UIObject(prototype)
{
}

MouseCursor* MouseCursor::Create()
{
	MouseCursor* Instance = new MouseCursor();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT MouseCursor::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT MouseCursor::Initialize(InitDESC* arg)
{
	UIObject::UIOBJECT_DESC desc{};
	desc.sizeX = 32.f;
	desc.sizeY = 32.f;

	if (FAILED(__super::Initialize(&desc)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.iMaxFrameIndex = 1;
	spriteDesc.isAnimated = false;
	auto sprite = GetComponent<SpriteComponent>();
	sprite->Initialize(&spriteDesc);
	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_MouseCursor");
	return S_OK;
}

void MouseCursor::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void MouseCursor::Update(_float dt)
{
	__super::Update(dt);
	
	_float3 scale = m_pTransform->GetScale();
	_float2 mousePos = EngineCore::GetInstance()->GetMousePosition();
	_float3 position{};

	position.x = mousePos.x - WinSizeX * 0.5f;
	position.y = WinSizeY * 0.5f - mousePos.y + scale.y;

	m_pTransform->SetPosition(position);
}

void MouseCursor::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* MouseCursor::Clone(InitDESC* arg)
{
	MouseCursor* Instance = new MouseCursor(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void MouseCursor::Free()
{
	__super::Free();
}
