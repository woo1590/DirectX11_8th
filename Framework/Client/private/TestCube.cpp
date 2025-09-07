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

	m_pVIBuffer = EngineCore::GetInstance()->GetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Cube");

	m_pMaterial = Material::Create(EngineCore::GetInstance()->GetShader(ENUM_CLASS(LevelID::Static), "Shader_VtxCube"));
	m_pMaterial->SetTexture("g_DiffuseMap", EngineCore::GetInstance()->GetTexture(ENUM_CLASS(LevelID::Static), "Texture_TestCube"));

	m_pTransform->SetPosition(_float3(0.f, 0.f, 100.f));
	m_pTransform->SetScale(_float3(60.f, 60.f, 60.f));

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

HRESULT TestCube::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	CBPerObject cb{};
	cb.worldMatrix = m_pTransform->GetWorldMatrix();
	cb.worldMatrixInverse = m_pTransform->GetWorldMatrixInverse();

	RenderProxy proxy{};
	proxy.buffer = m_pVIBuffer;
	proxy.material = m_pMaterial;
	proxy.cbPerObject = cb;
	proxy.group = RenderGroup::NonBlend;
	proxy.frameIndex = 0;

	proxies[ENUM_CLASS(RenderGroup::NonBlend)].push_back(proxy);

	return S_OK;
}

Object* TestCube::Clone(InitDESC* arg)
{
	TestCube* Instance = new TestCube(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}
