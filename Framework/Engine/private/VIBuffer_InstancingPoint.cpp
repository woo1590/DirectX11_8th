#include "EnginePCH.h"
#include "VIBuffer_InstancingPoint.h"
#include "EngineCore.h"

VIBuffer_InstancingPoint::VIBuffer_InstancingPoint()
	:VIBuffer()
{
}

VIBuffer_InstancingPoint* VIBuffer_InstancingPoint::Create(_uint bufferLevelID, const _string& bufferTag, _uint numInstance, VTX_INSTANCE_POINT* instanceInitData)
{
	VIBuffer_InstancingPoint* Instance = new VIBuffer_InstancingPoint();

	if (FAILED(Instance->Initialize(bufferLevelID,bufferTag,numInstance,instanceInitData)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT VIBuffer_InstancingPoint::Initialize(_uint bufferLevelID, const _string& bufferTag, _uint numInstance, VTX_INSTANCE_POINT* instanceInitData)
{
	auto engine = EngineCore::GetInstance();

	m_pBaseBuffer = engine->GetBuffer(bufferLevelID, bufferTag);
	if (!m_pBaseBuffer)
		return E_FAIL;
	m_pBaseBuffer->AddRef();

	m_iNumVertexBuffers = 2;
	m_iNumVertices = 1;
	m_iVertexStride = sizeof(VTXPOS);

	m_iNumIndices = 0;
	m_iIndexStride = 0;
	m_eIndexFormat = DXGI_FORMAT_UNKNOWN;
	m_ePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	m_iNumInstance = numInstance;
	m_iInstanceStride = sizeof(VTX_INSTANCE_POINT);

	/*---Instance buffer---*/
	D3D11_BUFFER_DESC instanceDesc{};
	instanceDesc.ByteWidth = m_iNumInstance * m_iInstanceStride;
	instanceDesc.Usage = D3D11_USAGE_DYNAMIC;
	instanceDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	instanceDesc.MiscFlags = 0;
	instanceDesc.StructureByteStride = m_iInstanceStride;

	D3D11_SUBRESOURCE_DATA instanceData{};
	instanceData.pSysMem = instanceInitData;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	if (FAILED(m_pDevice->CreateBuffer(&instanceDesc, &instanceData, &m_pInstanceBuffer)))
		return E_FAIL;

	return S_OK;
}


HRESULT VIBuffer_InstancingPoint::BindBuffers()
{
	ID3D11Buffer* vertexBuffers[] =
	{
		m_pBaseBuffer->GetVB(),
		m_pInstanceBuffer
	};

	_uint vertexStrides[] =
	{
		m_iVertexStride,
		m_iInstanceStride
	};

	_uint vertexOffsets[] =
	{
		0,
		0
	};

	m_pDeviceContext->IASetVertexBuffers(0, m_iNumVertexBuffers, vertexBuffers, vertexStrides, vertexOffsets);
	m_pDeviceContext->IASetPrimitiveTopology(m_ePrimitiveTopology);

	return S_OK;
}

HRESULT VIBuffer_InstancingPoint::Draw()
{
	m_pDeviceContext->DrawInstanced(1, m_iNumInstance, 0, 0);

	return S_OK;
}

void VIBuffer_InstancingPoint::Free()
{
	__super::Free();

	Safe_Release(m_pBaseBuffer);
}
