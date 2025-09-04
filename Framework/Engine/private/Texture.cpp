#include "EnginePCH.h"
#include "Texture.h"
#include "EngineCore.h"

Texture::Texture()
{
}

Texture* Texture::Create(const _string& filePath, _uint numTextures)
{
	Texture* Instance = new Texture();

	if (FAILED(Instance->Initialize(filePath, numTextures)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Texture::Initialize(const _string& filePath, _uint numTextures)
{
	std::filesystem::path p = std::filesystem::u8path(filePath);
	_wstring path = p.wstring();
	_wstring ext = p.extension().wstring();

	auto device = EngineCore::GetInstance()->GetDevice();

	for (_uint i = 0; i < numTextures; ++i)
	{
		_tchar szFileName[MAX_PATH] = TEXT("");
		wsprintf(szFileName, path.c_str(), i);

		ID3D11ShaderResourceView* srv = nullptr;

		if (ext == L".dds")
		{
			if (FAILED(CreateDDSTextureFromFile(device, szFileName, nullptr, &srv)))
				return E_FAIL;
		}
		else if (ext == L".tga")
		{
			MSG_BOX("Create Failed : Texture");
		}
		else
		{
			if (FAILED(CreateWICTextureFromFile(device, szFileName, nullptr, &srv)))
				return E_FAIL;
		}

		m_SRVs.push_back(srv);
	}

	return S_OK;
}

ID3D11ShaderResourceView* Texture::GetSRV(_int index) const
{
	if (index >= m_SRVs.size())
		return nullptr;

	return m_SRVs[index];
}

void Texture::Free()
{
	__super::Free();

	for (auto& srv : m_SRVs)
		Safe_Release(srv);
	m_SRVs.clear();
}
