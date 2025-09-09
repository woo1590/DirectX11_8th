#include "pch.h"
#include "Terrain.h"
#include "EngineCore.h"
#include "Material.h"

Terrain::Terrain()
	:Object()
{
}

Terrain::Terrain(const Terrain& prototype)
	:Object(prototype)
{
}

Terrain* Terrain::Create()
{
	Terrain* Instance = new Terrain();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Terrain::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_strInstanceTag = "Terrain";

	return S_OK;
}

HRESULT Terrain::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;
	
	auto engine = EngineCore::GetInstance();

	m_pVIBuffer = engine->GetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Terrain");
	m_pMaterial = engine->GetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_Terrain");

	return S_OK;
}

void Terrain::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Terrain::Update(_float dt)
{
	__super::Update(dt);
}

void Terrain::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT Terrain::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
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
	proxy.passTag = "Terrain";

	proxies[ENUM_CLASS(RenderGroup::NonBlend)].push_back(proxy);

	return S_OK;
}

void Terrain::Free()
{
	__super::Free();

	Safe_Release(m_pMaterial);
}

Object* Terrain::Clone(InitDESC* arg)
{
	Terrain* Instance = new Terrain(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}
