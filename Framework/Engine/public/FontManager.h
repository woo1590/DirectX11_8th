#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CustomFont;
class FontManager :
    public Base
{
private:
    FontManager();
    virtual ~FontManager() = default;

public:
    static FontManager* Create();
    HRESULT Initialize();

    void AddFont(const _string& fontTag, const _string& filePath);
    void AddProxy(FONT_PROXY proxy);
    HRESULT RenderFont();

    void Free()override;

private:
    HRESULT Draw(const _string& fontTag, const _wstring& text, _float3 position, _float scale, _float4 color);
    CustomFont* FindFont(const _string& fontTag);

    std::unordered_map<_string, CustomFont*> m_Fonts;
    std::queue<FONT_PROXY> m_Proxies;
};

NS_END