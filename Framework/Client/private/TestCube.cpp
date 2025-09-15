#include "pch.h"
#include "TestCube.h"
#include "Material.h"

//component
#include "ModelComponent.h"
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

	auto model = GetComponent<ModelComponent>();
	model->SetOverride(m_pMaterial);
	model->SetModel(ENUM_CLASS(LevelID::GamePlay), "Model_Test");
	
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

	_float3 rotation = m_pTransform->GetRotation();
	rotation.y += dt;
	m_pTransform->SetRotation(rotation);

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
