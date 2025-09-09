#include "pch.h"
#include "LoadingAnim.h"
#include "Material.h"
#include "VIBuffer.h"

LoadingAnim::LoadingAnim()
	:UIObject()
{
}

LoadingAnim::LoadingAnim(const LoadingAnim& prototype)
	:UIObject(prototype)
{
}

LoadingAnim* LoadingAnim::Create()
{
	LoadingAnim* Instance = new LoadingAnim();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT LoadingAnim::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT LoadingAnim::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	return S_OK;
}

void LoadingAnim::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void LoadingAnim::Update(_float dt)
{
	__super::Update(dt);

	if (m_fElaspedTime >= 0.05f)
	{
		m_iTexNum++;
		if (m_iTexNum >= 100)
			m_iTexNum = 0;

		m_fElaspedTime = 0.f;
	}

	m_fElaspedTime += dt;
}

void LoadingAnim::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT LoadingAnim::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	CBPerObject cb{};
	cb.worldMatrix = m_pTransform->GetWorldMatrix();
	cb.worldMatrixInverse = m_pTransform->GetWorldMatrixInverse();

	RenderProxy proxy{};
	proxy.buffer = m_pVIBuffer;
	proxy.material = m_pMaterial;
	proxy.cbPerObject = cb;
	proxy.group = RenderGroup::UI;
	proxy.frameIndex = m_iTexNum;
	proxy.passTag = "Default";

	proxies[ENUM_CLASS(RenderGroup::UI)].push_back(proxy);

	return S_OK;
}

Object* LoadingAnim::Clone(InitDESC* arg)
{
	LoadingAnim* Instance = new LoadingAnim(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void LoadingAnim::Free()
{
	__super::Free();

	Safe_Release(m_pMaterial);
	Safe_Release(m_pVIBuffer);
}
