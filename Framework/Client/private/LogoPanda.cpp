#include "pch.h"
#include "LogoPanda.h"
#include "MaterialInstance.h"

//component
#include "ModelComponent.h"
#include "AnimatorComponent.h"

LogoPanda::LogoPanda()
	:Object()
{
}

LogoPanda::LogoPanda(const LogoPanda& prototype)
	:Object(prototype)
{
}

LogoPanda* LogoPanda::Create()
{
	LogoPanda* Instance = new LogoPanda();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT LogoPanda::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	AddComponent<AnimatorComponent>();
	m_strInstanceTag = "LogoPanda";

	return S_OK;
}

HRESULT LogoPanda::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	auto model = GetComponent<ModelComponent>();
	model->Initialize(nullptr);
	model->SetModel(ENUM_CLASS(LevelID::Static), "Model_LogoPanda");
	model->GetMaterialInstance()->SetPass("Player_Pass");
	model->GetMaterialInstance()->SetInt("g_ObjectMask", 1);

	auto animator = GetComponent<AnimatorComponent>();
	animator->SetAnimation(ENUM_CLASS(LevelID::Static), "AnimationSet_LogoPanda");

	model->ConnectAnimator();
	animator->ChangeAnimation(0, true);

	m_pOutlineModel = ModelComponent::Create(this);
	m_pOutlineModel->Initialize(nullptr);
	m_pOutlineModel->SetModel(ENUM_CLASS(LevelID::Static), "Model_LogoPanda");
	auto outlineMtrlInstance = m_pOutlineModel->GetMaterialInstance();
	outlineMtrlInstance->SetPass("PlayerOutLine_Pass");
	outlineMtrlInstance->SetFloat4("g_OutLineColor", _float4(0.f, 0.f, 0.f, 1.f));
	outlineMtrlInstance->SetFloat("g_OutLineWidth", 0.1f);

	m_pTransform->SetPosition(_float3{ -177.7f,9.5f,-167.45f });
	m_pTransform->SetScale(_float3{ 1.03f,1.03f,1.03f });

	return S_OK;
}

void LogoPanda::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void LogoPanda::Update(_float dt)
{
	__super::Update(dt);
}

void LogoPanda::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT LogoPanda::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	__super::ExtractRenderProxies(proxies);

	if (m_pOutlineModel)
		m_pOutlineModel->ExtractRenderProxy(m_pTransform, proxies[ENUM_CLASS(RenderGroup::NonLight)]);

	return S_OK;
}

Object* LogoPanda::Clone(InitDESC* arg)
{
	LogoPanda* Instance = new LogoPanda(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void LogoPanda::Free()
{
	__super::Free();
	Safe_Release(m_pOutlineModel);
}
