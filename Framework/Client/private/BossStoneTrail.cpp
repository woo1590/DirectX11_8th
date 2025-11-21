#include "pch.h"
#include "BossStoneTrail.h"
#include "MaterialInstance.h"

//componenet
#include "ModelComponent.h"

BossStoneTrail::BossStoneTrail()
	:Object()
{
}

BossStoneTrail::BossStoneTrail(const BossStoneTrail& prototype)
	:Object(prototype)
{
}

BossStoneTrail* BossStoneTrail::Create()
{
	BossStoneTrail* Instance = new BossStoneTrail();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT BossStoneTrail::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	m_eRenderGroup = RenderGroup::Blend;

	return S_OK;
}

HRESULT BossStoneTrail::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*model*/
	auto model = GetComponent<ModelComponent>();
	model->Initialize(nullptr);
	model->SetModel(ENUM_CLASS(LevelID::Static), "Model_BossPunch");

	auto mtrlInstance = model->GetMaterialInstance();
	mtrlInstance->SetPass("BossStone_Pass");

	m_pTransform->SetScale(_float3{ 3.f,3.f,6.f });

	return S_OK;
}

HRESULT BossStoneTrail::LateInitialize()
{
	_float3 playerPosition = EngineCore::GetInstance()->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player")->GetComponent<TransformComponent>()->GetPosition();
	_float3 position = m_pTransform->GetPosition();
	_float3 forward{};
	XMStoreFloat3(&forward, XMVector3Normalize(XMLoadFloat3(&playerPosition) - XMLoadFloat3(&position)));

	m_pTransform->SetForward(forward);

	return S_OK;
}

void BossStoneTrail::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void BossStoneTrail::Update(_float dt)
{
	__super::Update(dt);

	auto mtrlInstance = GetComponent<ModelComponent>()->GetMaterialInstance();

	m_fSpriteElapsedTime += dt * m_fSpeed;
	if (m_fSpriteElapsedTime >= 1.f)
	{
		++m_iCurrFrame;
		if (m_iCurrFrame >= m_iMaxFrame)
			m_iCurrFrame = 0;

		m_fSpriteElapsedTime -= 1.f;
	}

	mtrlInstance->SetFloat("g_UVRatio", 1.f / m_iMaxFrame);
	mtrlInstance->SetInt("g_FrameIndex", m_iCurrFrame);
}

void BossStoneTrail::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* BossStoneTrail::Clone(InitDESC* arg)
{
	BossStoneTrail* Instance = new BossStoneTrail(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void BossStoneTrail::Free()
{
	__super::Free();
}
