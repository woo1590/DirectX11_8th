#include "EnginePCH.h"
#include "Texture.h"
#include "EngineCore.h"

Texture::Texture()
{
}

Texture* Texture::Create(const _string& filePath)
{
	Texture* Instance = new Texture();

	if (FAILED(Instance->Initialize(filePath)))
		Safe_Release(Instance);

	return Instance;

}

HRESULT Texture::Initialize(const _string& filePath)
{
	namespace fs = std::filesystem;
	auto device = EngineCore::GetInstance()->GetDevice();

	fs::path fullPath = filePath;

	HRESULT hr{};
	_wstring ext = fullPath.extension().wstring();
	if (ext == L".dds")
		hr = CreateDDSTextureFromFile(device, fullPath.wstring().c_str(), nullptr, &m_pSRV);
	else if (ext == L".tga")
	{
		MSG_BOX("Failed to load : Invalid file type");
		return E_FAIL;
	}
	else
		hr = CreateWICTextureFromFile(device, fullPath.wstring().c_str(), nullptr, &m_pSRV);

	return hr;
}

void Texture::Free()
{
	__super::Free();
	
	Safe_Release(m_pSRV);
}
