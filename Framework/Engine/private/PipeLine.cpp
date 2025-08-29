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
	/*view, proj ¼¼ÆÃ*/
}

void PipeLine::SetViewMatrix(_fmatrix viewMatrix)
{
	XMStoreFloat4x4(&m_ViewMatrix, viewMatrix);
}

void PipeLine::SetProjMatrix(_fmatrix projMatrix)
{
	XMStoreFloat4x4(&m_ProjMatrix, projMatrix);
}

_matrix PipeLine::GetViewMatirx() const
{
	return XMLoadFloat4x4(&m_ViewMatrix);
}

_matrix PipeLine::GetViewMatrixInverse() const
{
	return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_ViewMatrix));
}

_matrix PipeLine::GetProjMatrix() const
{
	return XMLoadFloat4x4(&m_ProjMatrix);
}

_matrix PipeLine::GetProjMatrixInverse() const
{
	return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_ProjMatrix));
}

void PipeLine::Free()
{
	__super::Free();
}
