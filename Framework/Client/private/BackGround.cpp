#include "pch.h"
#include "BackGround.h"
#include "Material.h"

BackGround::BackGround()
{
}

BackGround* BackGround::Create()
{
	BackGround* Instance = new BackGround();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT BackGround::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT BackGround::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	return S_OK;
}

void BackGround::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void BackGround::Update(_float dt)
{
	__super::Update(dt);

}

void BackGround::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT BackGround::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
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

Object* BackGround::Clone(InitDESC* arg)
{
	BackGround* Instance = new BackGround(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void BackGround::Free()
{
	__super::Free();

	Safe_Release(m_pMaterial);
}
