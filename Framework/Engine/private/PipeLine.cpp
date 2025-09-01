#include "EnginePCH.h"
#include "PipeLine.h"
#include "EngineCore.h"

PipeLine::PipeLine()
{
}

PipeLine* PipeLine::Create()
{
	PipeLine* Instance = new PipeLine();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT PipeLine::Initialize()
{
	return S_OK;
}

void PipeLine::Update()
{
	/*Update inverse matrix & cam position*/
	XMStoreFloat4x4(&m_ViewMatrixInverse,XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_ViewMatrix)));
	XMStoreFloat4x4(&m_ProjMatrixInverse,XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_ProjMatrix)));
	
	m_CamPosition = _float3(m_ViewMatrixInverse._41, m_ViewMatrixInverse._42, m_ViewMatrixInverse._43);
}

void PipeLine::Free()
{
	__super::Free();
}
