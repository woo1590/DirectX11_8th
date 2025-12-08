#include "pch.h"
#include "LogoPlayer.h"
#include "MaterialInstance.h"
//component
#include "ModelComponent.h"
#include "AnimatorComponent.h"

LogoPlayer::LogoPlayer()
	:Object()
{
}

LogoPlayer::LogoPlayer(const LogoPlayer& prototype)
	:Object(prototype)
{
}

LogoPlayer* LogoPlayer::Create()
{
	LogoPlayer* Instance = new LogoPlayer();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT LogoPlayer::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	AddComponent<AnimatorComponent>();
	m_strInstanceTag = "LogoPlayer";

	return S_OK;
}

HRESULT LogoPlayer::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	auto model = GetComponent<ModelComponent>();
	model->Initialize(nullptr);
	model->SetModel(ENUM_CLASS(LevelID::Static), "Model_LogoPlayer");
	model->GetMaterialInstance()->SetPass("Player_Pass");
	model->GetMaterialInstance()->SetInt("g_ObjectMask", 1);

	auto animator = GetComponent<AnimatorComponent>();
	animator->SetAnimation(ENUM_CLASS(LevelID::Static), "AnimationSet_LogoPlayer");

	model->ConnectAnimator();
	animator->ChangeAnimation(0, true);

	m_pOutlineModel = ModelComponent::Create(this);
	m_pOutlineModel->Initialize(nullptr);
	m_pOutlineModel->SetModel(ENUM_CLASS(LevelID::Static), "Model_LogoPlayer");
	auto outlineMtrlInstance = m_pOutlineModel->GetMaterialInstance();
	outlineMtrlInstance->SetPass("PlayerOutLine_Pass");
	outlineMtrlInstance->SetFloat4("g_OutLineColor", _float4(0.f, 0.f, 0.f, 1.f));
	outlineMtrlInstance->SetFloat("g_OutLineWidth", 0.1f);

	m_pTransform->SetScale(_float3{ 1.03f,1.03f,1.03f });

	return S_OK;
}

void LogoPlayer::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void LogoPlayer::Update(_float dt)
{
	__super::Update(dt);
}

void LogoPlayer::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT LogoPlayer::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	__super::ExtractRenderProxies(proxies);

	if (m_pOutlineModel)
		m_pOutlineModel->ExtractRenderProxy(m_pTransform, proxies[ENUM_CLASS(RenderGroup::NonLight)]);

	return S_OK;
}

Object* LogoPlayer::Clone(InitDESC* arg)
{
	LogoPlayer* Instance = new LogoPlayer(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void LogoPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pOutlineModel);
}
