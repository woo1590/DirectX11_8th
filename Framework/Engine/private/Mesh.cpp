#include "EnginePCH.h"
#include "Mesh.h"
#include "EngineCore.h"

Mesh::Mesh()
	:m_pDevice(EngineCore::GetInstance()->GetDevice())
	,m_pDeviceContext(EngineCore::GetInstance()->GetDeviceContext())
{
	m_pDevice->AddRef();
	m_pDeviceContext->AddRef();
}

HRESULT Mesh::Initialize()
{
	return S_OK;
}

void Mesh::Free()
{
	__super::Free();

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}
