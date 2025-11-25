#include "EnginePCH.h"
#include "CustomFont.h"
#include "EngineCore.h"

CustomFont::CustomFont()
{
}

CustomFont* CustomFont::Create(const _string& filePath)
{
	CustomFont* Instance = new CustomFont();

	if (FAILED(Instance->Initialize(filePath)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT CustomFont::Initialize(const _string& filePath)
{
	auto engine = EngineCore::GetInstance();
	_wstring path{};
	path.assign(filePath.begin(), filePath.end());

	m_pDevice = engine->GetDevice();
	m_pDeviceContext = engine->GetDeviceContext();

	m_pDevice->AddRef();
	m_pDeviceContext->AddRef();

	m_pBatch = new SpriteBatch(m_pDeviceContext);
	m_pFont = new SpriteFont(m_pDevice, path.c_str());

	D3D11_BLEND_DESC blendDesc{};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	D3D11_RENDER_TARGET_BLEND_DESC& rt = blendDesc.RenderTarget[0];
	rt.BlendEnable = TRUE;
	rt.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	rt.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	rt.BlendOp = D3D11_BLEND_OP_ADD;
	rt.SrcBlendAlpha = D3D11_BLEND_ONE;
	rt.DestBlendAlpha = D3D11_BLEND_ZERO;
	rt.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(m_pDevice->CreateBlendState(&blendDesc, &m_pBlendState)))
		return E_FAIL;

	return S_OK;
}

HRESULT CustomFont::Draw(const _wstring& text, _float3 position, _float scale, _float4 color)
{
	m_pDeviceContext->GSSetShader(nullptr, nullptr, NULL);

	m_pBatch->Begin(SpriteSortMode_Deferred, m_pBlendState);

	m_pFont->DrawString(m_pBatch, text.c_str(), XMLoadFloat3(&position), XMLoadFloat4(&color), 0.f, XMVectorSet(0.f, 0.f, 0.f, 1.f), scale);

	m_pBatch->End();

	return S_OK;
}
void CustomFont::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pBlendState);

	Safe_Delete(m_pBatch);
	Safe_Delete(m_pFont);
}
