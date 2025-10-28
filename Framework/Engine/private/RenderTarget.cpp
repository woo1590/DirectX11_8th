#include "EnginePCH.h"
#include "RenderTarget.h"

RenderTarget::RenderTarget()
{
}

RenderTarget* RenderTarget::Create()
{
	RenderTarget* Instance = new RenderTarget();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT RenderTarget::Initialize()
{
	return S_OK;
}

void RenderTarget::Free()
{
	__super::Free();

	Safe_Release(m_pTexture2D);
	Safe_Release(m_pRTV);
	Safe_Release(m_pSRV);
}
