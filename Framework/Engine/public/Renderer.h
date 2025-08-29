#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL Renderer :
    public Base
{
private:
    Renderer();
    virtual ~Renderer() = default;

public:
    static Renderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
    HRESULT Initialize();

    void Free()override;

private:
    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pDeviceContext = nullptr;
};

NS_END