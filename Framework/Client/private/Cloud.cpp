#include "pch.h"
#include "Cloud.h"
#include "MaterialInstance.h"

//component
#include "ModelComponent.h"

Cloud::Cloud()
	:Object()
{
}

Cloud::Cloud(const Cloud& prototype)
	:Object(prototype)
{
}

Cloud* Cloud::Create()
{
	Cloud* Instance = new Cloud();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Cloud::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	m_eRenderGroup = RenderGroup::Priority;

	return S_OK;
}

HRESULT Cloud::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*model*/
	auto model = GetComponent<ModelComponent>();
	model->Initialize(nullptr);
	model->SetModel(ENUM_CLASS(LevelID::Static), "Model_Cloud");

	auto mtrlInstance = model->GetMaterialInstance();
	mtrlInstance->SetPass("Cloud_Pass");
	mtrlInstance->SetFloat2("g_UVOffset", _float2{ 0.f,0.f });

	return S_OK;
}

void Cloud::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Cloud::Update(_float dt)
{
	__super::Update(dt);

	m_UVOffset.x += dt * 0.02f;

	auto mtrlInstance = GetComponent<ModelComponent>()->GetMaterialInstance();
	mtrlInstance->SetFloat2("g_UVOffset", m_UVOffset);
}

void Cloud::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);

	CAMERA_CONTEXT camContext = EngineCore::GetInstance()->GetCameraContext();
	m_pTransform->SetPosition(camContext.camPosition);
}

Object* Cloud::Clone(InitDESC* arg)
{
	Cloud* Instance = new Cloud(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Cloud::Free()
{
	__super::Free();
}
