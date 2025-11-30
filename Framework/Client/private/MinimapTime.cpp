#include "pch.h"
#include "MinimapTime.h"

//component
#include "SpriteComponent.h"

MinimapTime::MinimapTime()
	:UIObject()
{
}

MinimapTime::MinimapTime(const MinimapTime& prototype)
	:UIObject(prototype)
{
}

MinimapTime* MinimapTime::Create()
{
	MinimapTime* Instance = new MinimapTime();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT MinimapTime::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT MinimapTime::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.iMaxFrameIndex = 1;
	spriteDesc.isAnimated = false;
	auto sprite = GetComponent<SpriteComponent>();
	sprite->Initialize(&spriteDesc);
	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_MinimapTime");

	return S_OK;
}

void MinimapTime::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void MinimapTime::Update(_float dt)
{
	__super::Update(dt);

	m_fElapsedTime += dt;

	if (m_fElapsedTime >= 1.f)
	{
		++m_iSecond;
		m_fElapsedTime -= 1.f;

		if (m_iSecond >= 60)
		{
			++m_iMinute;
			m_iSecond = 0;
		}
	}

	m_strText = std::to_wstring(m_iMinute) + L"Ка" + L" " + std::to_wstring(m_iSecond) + L"УЪ";
}

void MinimapTime::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT MinimapTime::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	__super::ExtractRenderProxies(proxies);

	_float3 postion{ m_fX - m_fViewportWidth * 0.5f,-m_fY + m_fViewportHeight * 0.5f,0.f };
	_float3 scae{ m_fSizeX,m_fSizeY,1.f };

	_float3 position = m_pTransform->GetWorldPosition();
	_float3 scale = m_pTransform->GetScale();

	FONT_PROXY font{};
	font.fontTag = "Default_Font";
	font.scale = 0.35f;
	font.screenPosition.x = position.x + WinSizeX * 0.5f - scale.x * 0.29f;
	font.screenPosition.y = WinSizeY * 0.5f - position.y - scale.y * 0.45f;
	font.screenPosition.z = 0.f;
	font.text = m_strText;
	font.color = _float4{ 189.f / 255.f,172.f / 255.f,128.f / 255.f,1.f };

	EngineCore::GetInstance()->AddFontProxy(font);

	return S_OK;
}

Object* MinimapTime::Clone(InitDESC* arg)
{
	MinimapTime* Instance = new MinimapTime(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void MinimapTime::Free()
{
	__super::Free();
}
