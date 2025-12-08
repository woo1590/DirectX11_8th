#include "EnginePCH.h"
#include "FontManager.h"
#include "CustomFont.h"

FontManager::FontManager()
{
}

FontManager* FontManager::Create()
{
	FontManager* Instance = new FontManager();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT FontManager::Initialize()
{
	return S_OK;
}

void FontManager::AddFont(const _string& fontTag, const _string& filePath)
{
	auto font = FindFont(fontTag);

	if (font)
		return;

	font = CustomFont::Create(filePath);
	if (!font)
		return;

	m_Fonts.emplace(fontTag, font);
}

void FontManager::AddProxy(FONT_PROXY proxy)
{
	m_Proxies.push(proxy);
}

HRESULT FontManager::DrawFont(FONT_PROXY proxy)
{
	return Draw(proxy.fontTag, proxy.text, proxy.screenPosition, proxy.scale, proxy.color);;
}

HRESULT FontManager::RenderFont()
{
	while (!m_Proxies.empty())
	{
		FONT_PROXY proxy = m_Proxies.front();
		m_Proxies.pop();

		Draw(proxy.fontTag, proxy.text, proxy.screenPosition, proxy.scale, proxy.color);
	}

	return S_OK;
}

HRESULT FontManager::Draw(const _string& fontTag, const _wstring& text, _float3 position, _float scale, _float4 color)
{
	auto font = FindFont(fontTag);
	if (!font)
	{
		MSG_BOX("Font is no exist");
		return E_FAIL;
	}

	return font->Draw(text, position,scale, color);
}

void FontManager::Free()
{
	__super::Free();

	for (auto& pair : m_Fonts)
		Safe_Release(pair.second);
	m_Fonts.clear();
}

CustomFont* FontManager::FindFont(const _string& fontTag)
{
	auto iter = m_Fonts.find(fontTag);

	if (iter == m_Fonts.end())
		return nullptr;
	
	return iter->second;
}
