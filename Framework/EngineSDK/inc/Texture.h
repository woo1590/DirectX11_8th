#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL Texture :
    public Base
{
private:
    Texture();
    virtual ~Texture() = default;

public:
    static Texture* Create(const _string& filePath);
    HRESULT Initialize(const _string& filePath);

    ID3D11ShaderResourceView* GetSRV()const { return m_pSRV; }
    void Free()override;

private:
    ID3D11ShaderResourceView* m_pSRV = nullptr;
};

NS_END