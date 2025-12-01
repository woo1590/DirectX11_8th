#include "pch.h"
#include "LoadingPanel.h"
#include "MaterialInstance.h"

//component
#include "SpriteComponent.h"

LoadingPanel::LoadingPanel()
	:UIObject()
{
}

LoadingPanel::LoadingPanel(const LoadingPanel& prototype)
	:UIObject(prototype)
{
}

LoadingPanel* LoadingPanel::Create()
{
	LoadingPanel* Instance = new LoadingPanel();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT LoadingPanel::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT LoadingPanel::Initialize(InitDESC* arg)
{
	UIObject::UIOBJECT_DESC desc{};
	desc.x = WinSizeX * 0.5f;
	desc.y = WinSizeY * 0.9f;
	desc.sizeX = 892.f;
	desc.sizeY = 60.f;

	if (FAILED(__super::Initialize(&desc)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	LOADING_PANEL_DESC* panelDesc = static_cast<LOADING_PANEL_DESC*>(arg);
	m_eNextLevelID = panelDesc->nextLevelID;

	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.iMaxFrameIndex = 1;
	spriteDesc.isAnimated = false;
	auto sprite = GetComponent<SpriteComponent>();
	sprite->Initialize(&spriteDesc);
	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_LoadingPanel");

	m_pLoadingBar = SpriteComponent::Create(this);
	m_pLoadingBar->Initialize(&spriteDesc);
	m_pLoadingBar->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	m_pLoadingBar->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_LoadingBar");
	m_pLoadingBar->GetMaterialInstance()->SetPass("LoadingBar_Pass");

	_float3 position = m_pTransform->GetPosition();
	_float3 scale = m_pTransform->GetScale();

	_float3 barPosition = position;
	barPosition.x -= 7.f;
	barPosition.y -= 1.f;
	m_pLoadingBarTransform = TransformComponent::Create(this);
	m_pLoadingBarTransform->SetScale(_float3{ 766.f,5.f,1.f });
	m_pLoadingBarTransform->SetPosition(barPosition);

	switch (m_eNextLevelID)
	{
	case Client::LevelID::Stage1:
		m_pLoadingBar->GetMaterialInstance()->SetFloat("g_Ratio", 0.f);
		break;
	case Client::LevelID::Stage2:
		m_pLoadingBar->GetMaterialInstance()->SetFloat("g_Ratio", 0.3f);
		break;
	case Client::LevelID::Stage3:
		m_pLoadingBar->GetMaterialInstance()->SetFloat("g_Ratio", 0.7f);
		break;
	case Client::LevelID::StageBoss:
		m_pLoadingBar->GetMaterialInstance()->SetFloat("g_Ratio", 1.f);
		break;
	default:
		break;
	}

	m_pStartIcon = SpriteComponent::Create(this);
	m_pStartIcon->Initialize(&spriteDesc);
	m_pStartIcon->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	m_pStartIcon->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_LoadingStartIcon");

	_float3 startIconPosition = position;
	startIconPosition.x = position.x - scale.x * 0.47f;
	startIconPosition.y -= 1.f;
	m_pStartIconTransform = TransformComponent::Create(this);
	m_pStartIconTransform->SetPosition(_float3{startIconPosition});
	m_pStartIconTransform->SetScale(_float3{ 100.f,100.f,1.f });

	m_pStage2Icon = SpriteComponent::Create(this);
	m_pStage2Icon->Initialize(&spriteDesc);
	m_pStage2Icon->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	m_pStage2Icon->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_LoadingStageIcon");

	_float3 stage2IconPosition = position;
	stage2IconPosition.x = position.x - scale.x * 0.16f;
	m_pStage2Transform = TransformComponent::Create(this);
	m_pStage2Transform->SetPosition(stage2IconPosition);
	m_pStage2Transform->SetScale(_float3{ 64.f,60.f,1.f });

	m_pStage3Icon = SpriteComponent::Create(this);
	m_pStage3Icon->Initialize(&spriteDesc);
	m_pStage3Icon->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	m_pStage3Icon->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_LoadingStageIcon");

	_float3 stage3IconPosition = position;
	stage3IconPosition.x = position.x + scale.x * 0.145f;
	m_pStage3Transform = TransformComponent::Create(this);
	m_pStage3Transform->SetPosition(stage3IconPosition);
	m_pStage3Transform->SetScale(_float3{ 64.f,60.f,1.f });

	m_pBossIcon = SpriteComponent::Create(this);
	m_pBossIcon->Initialize(&spriteDesc);
	m_pBossIcon->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	m_pBossIcon->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_LoadingBossIcon");

	_float3 bossIconPosition = position;
	bossIconPosition.x = position.x + scale.x * 0.464f;
	m_pBossTransform = TransformComponent::Create(this);
	m_pBossTransform->SetPosition(bossIconPosition);
	m_pBossTransform->SetScale(_float3{ 90.f,90.f,1.f });

	return S_OK;
}

void LoadingPanel::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void LoadingPanel::Update(_float dt)
{
	__super::Update(dt);
}

void LoadingPanel::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT LoadingPanel::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	__super::ExtractRenderProxies(proxies);

	if (m_pLoadingBar)
		m_pLoadingBar->ExtractRenderProxy(m_pLoadingBarTransform, proxies[ENUM_CLASS(RenderGroup::UI)]);

	switch (m_eNextLevelID)
	{
	case Client::LevelID::StageBoss:
		m_pBossIcon->ExtractRenderProxy(m_pBossTransform, proxies[ENUM_CLASS(RenderGroup::UI)]);
	case Client::LevelID::Stage3:
		m_pStage3Icon->ExtractRenderProxy(m_pStage3Transform, proxies[ENUM_CLASS(RenderGroup::UI)]);
	case Client::LevelID::Stage2:
		m_pStage2Icon->ExtractRenderProxy(m_pStage2Transform, proxies[ENUM_CLASS(RenderGroup::UI)]);
	case Client::LevelID::Stage1:
		m_pStartIcon->ExtractRenderProxy(m_pStartIconTransform, proxies[ENUM_CLASS(RenderGroup::UI)]);
	default:
		break;
	}

	return S_OK;
}

Object* LoadingPanel::Clone(InitDESC* arg)
{
	LoadingPanel* Instance = new LoadingPanel(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void LoadingPanel::Free()
{
	__super::Free();

	Safe_Release(m_pLoadingBar);
	Safe_Release(m_pLoadingBarTransform);

	Safe_Release(m_pStartIcon);
	Safe_Release(m_pStartIconTransform);

	Safe_Release(m_pStage2Icon);
	Safe_Release(m_pStage2Transform);

	Safe_Release(m_pStage3Icon);
	Safe_Release(m_pStage3Transform);

	Safe_Release(m_pBossIcon);
	Safe_Release(m_pBossTransform);
}
