#include "pch.h"
#include "Button.h"
#include "MaterialInstance.h"
#include "VIBuffer.h"
#include "Material.h"
#include "Command_ChangeLevel.h"
#include "LoadingLevel.h"

//component
#include "SpriteComponent.h"

Button::Button()
	:UIObject()
{
}

Button::Button(const Button& prototype)
	:UIObject(prototype)
{
}

Button* Button::Create()
{
	Button* Instance = new Button();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Button::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT Button::Initialize(InitDESC* arg)
{
	UIObject::UIOBJECT_DESC desc{};
	desc.x = 0.f;
	desc.y = 0.f;
	desc.sizeX = 123.f;
	desc.sizeY = 25.f;

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
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_Button");

	auto mtrlInstance = sprite->GetMaterialInstance();
	mtrlInstance->SetPass("UIButton_Pass");
	mtrlInstance->SetTexture("g_UIButtonTexture", engine->GetSRV("Target_UIButton"));

	m_pBuffer = engine->GetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	m_pMaterial = engine->GetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_Button");
	m_pMaterialInstance = MaterialInstance::Create();
	m_pMaterialInstance->SetPass("TargetUIButton_Pass");

	_float2 buttonSize{ 123.f * 3.f,25.f * 3.f };
	m_pMaterialInstance->SetFloat2("g_ButtonSize", buttonSize);
	mtrlInstance->SetFloat2("g_ButtonUVMin", _float2{ 0.5f - (buttonSize.x * 0.5f / WinSizeX) ,0.5f - (buttonSize.y * 0.5f / WinSizeY) });
	mtrlInstance->SetFloat2("g_ButtonUVMax", _float2{ 0.5f + (buttonSize.x * 0.5f / WinSizeX) ,0.5f + (buttonSize.y * 0.5f / WinSizeY) });

	m_pBuffer->AddRef();
	m_pMaterial->AddRef();

	m_pTransform->SetPosition(_float3{ -128.f,26.f,-154.f });
	m_pTransform->SetScale(_float3{ 24.f,4.f,1.f });
	m_pTransform->Rotate(_float3{ 0.f,math::ToRadian(90.f),0.f });

	m_StartScale = _float3{ 24.f,4.f,1.f };
	XMStoreFloat3(&m_TargetScale, XMLoadFloat3(&m_StartScale) * 1.2f);
	m_FontColor = _float4{ 170.f / 255.f,153.f / 255.f,97.f / 255.f,1.f };

	/*LT*/
	m_Offsets.push_back(_float2{ -1.f,-1.f });
	/*TC*/
	m_Offsets.push_back(_float2{ 0.f,-1.f });
	/*RT*/
	m_Offsets.push_back(_float2{ 1.f,-1.f });
	/*RC*/
	m_Offsets.push_back(_float2{ -1.f,0.f });
	/*RB*/
	m_Offsets.push_back(_float2{ 1.f,1.f });
	/*BC*/
	m_Offsets.push_back(_float2{ 0.f,1.f });
	/*LB*/
	m_Offsets.push_back(_float2{ -1.f,1.f });
	/*LC*/
	m_Offsets.push_back(_float2{ -1.f,0.f });

	return S_OK;
}

void Button::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Button::Update(_float dt)
{
	__super::Update(dt);

	if (m_IsActive)
	{
		m_fActiveElapsedTime += dt;
		if (m_fActiveElapsedTime >= m_fActiveDuration)
		{
			auto command = Command_ChangeLevel::Create(LevelID::Loading, LoadingLevel::Create(LevelID::Stage1));

			EngineCore::GetInstance()->RegisterCommand(command);
		}
	}
	
}

void Button::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);

	auto engine = EngineCore::GetInstance();

	CheckCollision();
	if (!m_IsHover)
	{
		m_pMaterialInstance->SetFloat("g_Ratio", 0.f);
	}

	if (m_IsHover)
	{
		m_fElapsedTime += dt;
		_float t = m_fElapsedTime / m_fDuration;
		t = std::clamp(t, 0.f, 1.f);
		m_pMaterialInstance->SetFloat("g_Ratio", t);

		_float3 currScale{};
		XMStoreFloat3(&currScale, XMVectorLerp(XMLoadFloat3(&m_StartScale), XMLoadFloat3(&m_TargetScale), math::EaseOutQuint(t)));

		m_pTransform->SetScale(currScale);

		if (!m_IsActive && engine->IsMousePress(MouseButton::LButton))
		{
			engine->Play2DSound("SFX_MouseClick", 0.8f);
			m_IsActive = true;
		}
	}
}

HRESULT Button::DrawButton()
{
	auto engine = EngineCore::GetInstance();
	engine->BeginMRT("MRT_UIButton");

	m_pBuffer->BindBuffers();
	m_pMaterial->BindMaterial(0, m_pMaterialInstance);
	m_pBuffer->Draw();

	for (_uint i = 0; i < 8; ++i)
	{
		_float2 offset = m_Offsets[i];

		FONT_PROXY font{};
		font.fontTag = "Nexon_Font";
		font.color = _float4(0.f, 0.f, 0.f, 1.f);
		font.scale = 1.f;
		font.screenPosition = _float3{ WinSizeX * 0.5f + offset.x * 1.5f - 150.f,WinSizeY * 0.5f + offset.y * 1.5f - 30.f,0.f };
		font.text = L"새로운 모험";
		EngineCore::GetInstance()->DrawFont(font);
	}

	FONT_PROXY font{};
	font.fontTag = "Nexon_Font";
	font.color = m_FontColor;
	font.text = L"새로운 모험";
	font.screenPosition = _float3{ WinSizeX * 0.5f - 150.f,WinSizeY * 0.5f - 30.f,0.f };
	font.scale = 1.f;
	EngineCore::GetInstance()->DrawFont(font);

	engine->EndMRT();
	return S_OK;
}

HRESULT Button::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	RenderProxy proxy{};
	proxy.renderFunc = [this]()->HRESULT {return DrawButton(); };
	proxies[ENUM_CLASS(RenderGroup::CustomPass)].push_back(proxy);

	__super::ExtractRenderProxies(proxies);

	return S_OK;
}

Object* Button::Clone(InitDESC* arg)
{
	Button* Instance = new Button(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Button::Free()
{
	__super::Free();
	Safe_Release(m_pBuffer);
	Safe_Release(m_pMaterial);
	Safe_Release(m_pMaterialInstance);
}

void Button::CheckCollision()
{
	auto engine = EngineCore::GetInstance();
	CAMERA_CONTEXT camContext = engine->GetCameraContext();

	_float4x4 worldMatInverse = m_pTransform->GetWorldMatrixInverse();
	D3D11_VIEWPORT viewPort = engine->GetViewport();
	POINT mousePos{};
	GetCursorPos(&mousePos);
	ScreenToClient(engine->GetWindowHandle(), &mousePos);
	_vector worldPos = XMVector3Unproject(XMVectorSet(mousePos.x, mousePos.y, 0.f, 1.f),
		viewPort.TopLeftX, viewPort.TopLeftY, viewPort.Width, viewPort.Height,
		viewPort.MinDepth, viewPort.MaxDepth,
		XMLoadFloat4x4(&camContext.projMatrix),
		XMLoadFloat4x4(&camContext.viewMatrix),
		XMMatrixIdentity());

	_vector origin = XMLoadFloat3(&camContext.camPosition);
	_vector dir = XMVector3Normalize(worldPos - origin);
	
	origin = XMVector3TransformCoord(origin, XMLoadFloat4x4(&worldMatInverse));
	dir = XMVector3Normalize(XMVector3TransformNormal(dir, XMLoadFloat4x4(&worldMatInverse)));

	_bool isHit = false;
	_float distance{};
	isHit |= TriangleTests::Intersects(origin, dir, XMVectorSet(-0.5f, 0.5f, 0.f, 1.f), XMVectorSet(0.5f, 0.5f, 0.f, 1.f), XMVectorSet(0.5f, -0.5f, 0.f, 1.f), distance);
	isHit |= TriangleTests::Intersects(origin, dir, XMVectorSet(-0.5f, 0.5f, 0.f, 1.f), XMVectorSet(0.5f, -0.5f, 0.f, 1.f), XMVectorSet(-0.5f, -0.5f, 0.f, 1.f), distance);

	if (isHit)
	{
		if (!m_IsHover)
		{
			m_IsHover = true;
			m_fElapsedTime = 0.f;
			m_FontColor = _float4{ 205.f / 255.f,186.f / 255.f,129.f / 255.f,1.f };
			engine->Play2DSound("SFX_MouseHover", 0.8f);
		}
	}
	else
	{
		m_IsHover = false;
		m_FontColor = _float4{ 170.f / 255.f,153.f / 255.f,97.f / 255.f,1.f };
		m_pTransform->SetScale(m_StartScale);
	}
}
