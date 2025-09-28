#include "EnginePCH.h"
#include "Renderer.h"
#include "EngineCore.h"
#include "VIBuffer.h"
#include "Material.h"
#include "LightProxy.h"

Renderer::Renderer()
	:m_pDevice(EngineCore::GetInstance()->GetDevice()),
	m_pDeviceContext(EngineCore::GetInstance()->GetDeviceContext())
{
	m_pDevice->AddRef();
	m_pDeviceContext->AddRef();
}

Renderer* Renderer::Create()
{
	Renderer* Instance = new Renderer();
	
	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Renderer::Initialize()
{
	/*----Constant Buffer Per Frame----*/
	D3D11_BUFFER_DESC cbPerFrameDesc{};
	cbPerFrameDesc.ByteWidth = sizeof(CBPerFrame);
	cbPerFrameDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbPerFrameDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbPerFrameDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbPerFrameDesc.MiscFlags = 0;
	cbPerFrameDesc.StructureByteStride = 0;

	if (FAILED(m_pDevice->CreateBuffer(&cbPerFrameDesc, nullptr, &m_pCBPerFrame)))
		return E_FAIL;

	/*----Constant Buffer Per Object----*/
	D3D11_BUFFER_DESC cbPerObjectDesc{};
	cbPerObjectDesc.ByteWidth = sizeof(CBPerObject);
	cbPerObjectDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbPerObjectDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbPerObjectDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbPerObjectDesc.MiscFlags = 0;
	cbPerObjectDesc.StructureByteStride = 0;

	if (FAILED(m_pDevice->CreateBuffer(&cbPerObjectDesc, nullptr, &m_pCBPerObject)))
		return E_FAIL;

	/*----Constant Buffer For Bones----*/
	D3D11_BUFFER_DESC cbBones{};
	cbBones.ByteWidth = sizeof(_float4x4) * MAX_BONES;
	cbBones.Usage = D3D11_USAGE_DYNAMIC;
	cbBones.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbBones.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbBones.MiscFlags = 0;
	cbBones.StructureByteStride = 0;

	if (FAILED(m_pDevice->CreateBuffer(&cbBones, nullptr, &m_pCBBonePalatte)))
		return E_FAIL;

	return S_OK;
}

HRESULT Renderer::BeginFrame(std::vector<LightProxy>& lights)
{
	/* 이번 프레임을 그릴 때 사용할 카메라 view, proj   라이트 view proj 세팅*/

	auto engine = EngineCore::GetInstance();

	/* 멀티스레드 확장 시 rendersystem에서 큐로 받아와야 함 -> 게임 로직에 직접 접근 ㄴㄴ */
	{
		D3D11_MAPPED_SUBRESOURCE cbPerFrameData{};
		CBPerFrame perFrame{};

		/*View, Proj Matrix*/
		perFrame.viewMatrix = engine->GetViewMatrix();
		perFrame.projMatrix = engine->GetProjMatrix();
		_float3 campos = engine->GetCamPosition();
		perFrame.camPosition = _float4(campos.x, campos.y, campos.z, 1.f);
	
		/*Light Data -> 이후 지연 렌더링으로 넘어가면서 구조 변경 예정*/
		/*방향성 조명 1개만*/

		if (lights.size())
		{
			perFrame.lightColor = lights[0].lightColor;
			perFrame.lightDirection = lights[0].lightDirection;
		}

		m_pDeviceContext->Map(m_pCBPerFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &cbPerFrameData);
		memcpy_s(cbPerFrameData.pData, sizeof(CBPerFrame), &perFrame, sizeof(CBPerFrame));
		m_pDeviceContext->Unmap(m_pCBPerFrame, 0);
	}

	return S_OK;
}

HRESULT Renderer::RenderPriority(const std::vector<RenderProxy>& proxies)
{
	return S_OK;
}

HRESULT Renderer::RenderNonBlend(const std::vector<RenderProxy>& proxies)
{
	for (const auto& proxy : proxies)
		DrawProxy(proxy,"NonBlend_Pass");

	return S_OK;
}

HRESULT Renderer::RenderBlend(const std::vector<RenderProxy>& proxies)
{
	for (const auto& proxy : proxies)
		DrawProxy(proxy, "Blend_Pass");

	return S_OK;
}

HRESULT Renderer::RenderUI(const std::vector<RenderProxy>& proxies)
{
	/* ui 전용 view proj생성 (임시용) */
	D3D11_MAPPED_SUBRESOURCE cbPerFrameData{};
	CBPerFrame perFrame{};
	XMStoreFloat4x4(&perFrame.viewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&perFrame.projMatrix, XMMatrixOrthographicLH(1280.f, 720.f, 0.f, 1.f));

	m_pDeviceContext->Map(m_pCBPerFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &cbPerFrameData);
	memcpy_s(cbPerFrameData.pData, sizeof(CBPerFrame), &perFrame, sizeof(CBPerFrame));
	m_pDeviceContext->Unmap(m_pCBPerFrame, 0);

	for (const auto& proxy : proxies)
		DrawProxy(proxy,"UI_Pass");

	return S_OK;
}

HRESULT Renderer::DrawProxy(const RenderProxy& proxy,const _string& passTag)
{
	{
		D3D11_MAPPED_SUBRESOURCE perObjectData{};
		CBPerObject perObject{};
		perObject.worldMatrix = proxy.worldMatrix;
		 
		m_pDeviceContext->Map(m_pCBPerObject, 0, D3D11_MAP_WRITE_DISCARD, 0, &perObjectData);
		memcpy_s(perObjectData.pData, sizeof(CBPerObject), &perObject, sizeof(CBPerObject));
		m_pDeviceContext->Unmap(m_pCBPerObject, 0);

		if (proxy.numBones)
		{
			D3D11_MAPPED_SUBRESOURCE bonesData{};
			m_pDeviceContext->Map(m_pCBBonePalatte, 0, D3D11_MAP_WRITE_DISCARD, 0, &bonesData);
			memcpy_s(bonesData.pData, sizeof(_float4x4) * MAX_BONES, proxy.boneMatrices, sizeof(_float4x4) * proxy.numBones);
			m_pDeviceContext->Unmap(m_pCBBonePalatte, 0);
		}
	}

	if (FAILED(proxy.buffer->BindBuffers()))
		return E_FAIL;
	
	if (FAILED(proxy.material->BindMaterial(passTag, proxy.frameIndex)))
		return E_FAIL;

	return proxy.buffer->Draw();
}

HRESULT Renderer::ConnectConstantBuffer(ID3DX11Effect* pEffect)
{
	auto perFrame = pEffect->GetConstantBufferByIndex(0);
	if (FAILED(perFrame->SetConstantBuffer(m_pCBPerFrame)))
		return E_FAIL;

	auto perObject = pEffect->GetConstantBufferByIndex(2);
	if (FAILED(perObject->SetConstantBuffer(m_pCBPerObject)))
		return E_FAIL;

	auto boneMatrices = pEffect->GetConstantBufferByIndex(3);
	if (FAILED(boneMatrices->SetConstantBuffer(m_pCBBonePalatte)))
		return E_FAIL;

	return S_OK;
}

void Renderer::Free()
{
	__super::Free();

	Safe_Release(m_pCBPerFrame);
	Safe_Release(m_pCBPerObject);
	Safe_Release(m_pCBBonePalatte);

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}
