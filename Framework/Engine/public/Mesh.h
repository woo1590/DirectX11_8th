#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL Mesh :
    public Base
{
protected:
    Mesh();
    virtual ~Mesh() = default;

public:
    virtual HRESULT Initialize();
    virtual void Free()override;

protected:
    ID3D11Buffer* m_pVB = nullptr;
    ID3D11Buffer* m_pIB = nullptr;

    _uint m_iVertexCnt{};
    _uint m_iVertexStride{};
    _uint m_iIndexCnt{};
    _uint m_iIndexStride{};
    DXGI_FORMAT m_eIndexFormat{};
    D3D11_PRIMITIVE_TOPOLOGY m_ePrimitiveTopology{};

    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pDeviceContext = nullptr;

};

NS_END