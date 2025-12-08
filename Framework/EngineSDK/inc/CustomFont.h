#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CustomFont :
    public Base
{
private:
    CustomFont();
    virtual ~CustomFont() = default;

public:
    static CustomFont* Create(const _string& filePath);
    HRESULT Initialize(const _string& filePath);

    HRESULT Draw(const _wstring& text, _float3 position, _float scale, _float4 color);
    void Free()override;

private:
    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pDeviceContext = nullptr;

    SpriteBatch* m_pBatch = nullptr;
    SpriteFont* m_pFont = nullptr;

    /*staet*/   
    CommonStates* m_pState = nullptr;
};

NS_END