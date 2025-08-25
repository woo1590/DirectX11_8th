#include "EnginePCH.h"
#include "PipeLine.h"

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

void XM_CALLCONV PipeLine::SetViewMatrix(_fmatrix viewMatrix)
{
	XMStoreFloat4x4(&m_ViewMatrix, viewMatrix);
}

void XM_CALLCONV PipeLine::SetProjMatrix(_fmatrix projMatrix)
{
	XMStoreFloat4x4(&m_ProjMatrix, projMatrix);
}

_matrix XM_CALLCONV PipeLine::GetViewMatirx() const
{
	return XMLoadFloat4x4(&m_ViewMatrix);
}

_matrix XM_CALLCONV PipeLine::GetViewMatrixInverse() const
{
	return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_ViewMatrix));
}

_matrix XM_CALLCONV PipeLine::GetProjMatrix() const
{
	return XMLoadFloat4x4(&m_ProjMatrix);
}

_matrix XM_CALLCONV PipeLine::GetProjMatrixInverse() const
{
	return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_ProjMatrix));
}

void PipeLine::Free()
{
	__super::Free();
}
