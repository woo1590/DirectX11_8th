#include "pch.h"
#include "TestCube.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "Material.h"

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

	return S_OK;
}

HRESULT TestCube::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	m_pVIBuffer = EngineCore::GetInstance()->GetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	m_pMaterial = Material::Create(EngineCore::GetInstance()->GetShader(ENUM_CLASS(LevelID::Static), "Shader_VtxTex"));
	m_pTransform->SetPosition(_float3(0.f, 0.f, 50.f));
	m_pTransform->SetScale(_float3(100.f, 100.f, 1.f));

	return S_OK;
}

void TestCube::Free()
{
	__super::Free();

	Safe_Release(m_pMaterial);
}

void TestCube::Update(_float dt)
{
	__super::Update(dt);

	if (GetAsyncKeyState('W'))
	{
		_float3 velocity{ 0.f,0.f,100.f };
		m_pTransform->Translate(XMLoadFloat3(&velocity) * dt);
	}

	if (GetAsyncKeyState('A'))
	{
		_float3 velocity{ -100.f,0.f,0.f };
		m_pTransform->Translate(XMLoadFloat3(&velocity) * dt);
	}

	if (GetAsyncKeyState('D'))
	{
		_float3 velocity{ 100.f,0.f,0.f };
		m_pTransform->Translate(XMLoadFloat3(&velocity) * dt);
	}

	if (GetAsyncKeyState('S'))
	{
		_float3 velocity{ 0.f,0.f,-100.f };
		m_pTransform->Translate(XMLoadFloat3(&velocity) * dt);
	}

}

void TestCube::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT TestCube::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	CBPerObject cb{};
	XMStoreFloat4x4(&cb.worldMatrix,m_pTransform->GetWorldMatrix());
	XMStoreFloat4x4(&cb.worldMatrixInverse,m_pTransform->GetWorldMatrixInverse());

	RenderProxy proxy{};
	proxy.buffer = m_pVIBuffer;
	proxy.material = m_pMaterial;
	proxy.cbPerObject = cb;
	proxy.group = RenderGroup::NonBlend;

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
