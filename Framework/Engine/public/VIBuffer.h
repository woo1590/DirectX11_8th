#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL VIBuffer abstract:
    public Base
{
protected:
    VIBuffer();
    virtual ~VIBuffer() = default;

public:
    virtual HRESULT Initialize();
    virtual void Free()override;

    virtual HRESULT BindBuffers();
    virtual HRESULT Draw();

protected:
    ID3D11Buffer* m_pVB = nullptr;
    ID3D11Buffer* m_pIB = nullptr;

    _uint m_iNumVertices{};
    _uint m_iVertexStride{};
    _uint m_iNumVertexBuffers{};

    _uint m_iNumIndices{};
    _uint m_iIndexStride{};

    DXGI_FORMAT m_eIndexFormat{};
    D3D11_PRIMITIVE_TOPOLOGY m_ePrimitiveTopology{};

    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pDeviceContext = nullptr;

    /*인덱스 버퍼 각 원소의 크기는 2, 4바이트로 지정할 수 있다
    선택 기준? 정점의 갯수가 2바이트 숫자로 표현 할 수 없을 때 -> 65535 이상*/

    /*큐브, 쿼드 메쉬의 경우에는 2바이트로 충분*/
    /*모델 로드하는 경우는 4바이트로 해야할듯?*/

};

NS_END