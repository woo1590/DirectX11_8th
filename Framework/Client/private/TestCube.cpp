#include "pch.h"
#include "TestCube.h"
#include "Material.h"
#include "Random.h"

//component
#include "ModelComponent.h"
#include "AnimatorComponent.h"
#include "TransformComponent.h"
#include "AudioSource.h"
#include "CameraComponent.h"

TestCube::TestCube()
{
}

TestCube* TestCube::Create()
{
	TestCube* Instance = new TestCube();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT TestCube::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_strInstanceTag = "TestCube";

	AddComponent<ModelComponent>();
	AddComponent<AnimatorComponent>();
	AddComponent<AudioSource>();

	return S_OK;
}

HRESULT TestCube::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	/*---юс╫ц©К---*/
	m_pMaterial = Material::Create(EngineCore::GetInstance()->GetShader("Shader_VtxMesh"));
	/*-----------*/
	auto random = EngineCore::GetInstance()->GetRandom();
	
	m_pTransform->SetPosition(_float3(random->get<_float>(-10.f, 10.f), 0.f,random->get<_float>(-15.f,10.f)));

	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::GamePlay), "Model_Test");

	auto animator = GetComponent<AnimatorComponent>();
	animator->SetAnimation(ENUM_CLASS(LevelID::GamePlay), "AnimationSet_Test");

	model->ConnectAnimator();
	animator->ChangeAnimation(random->get<_int>(0,3),true);
	
	return S_OK;
}

void TestCube::Free()
{
	__super::Free();

	Safe_Release(m_pMaterial);
}

void TestCube::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void TestCube::Update(_float dt)
{
	__super::Update(dt);

	if (EngineCore::GetInstance()->IsKeyPressed(VK_UP))
	{
		auto random = EngineCore::GetInstance()->GetRandom();
		_uint randNum = random->get<_int>(0, 20);
		GetComponent<AnimatorComponent>()->ChangeAnimation(randNum, true);
	}
}

void TestCube::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* TestCube::Clone(InitDESC* arg)
{
	TestCube* Instance = new TestCube(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}
