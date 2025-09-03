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
    static Texture* Create(const _string& filePath, _uint numTextures);
    HRESULT Initialize(const _string& filePath, _uint numTextures);

    ID3D11ShaderResourceView* GetSRV(_int index)const;
    void Free()override;

private:
    std::vector<ID3D11ShaderResourceView*> m_SRVs;
};

NS_END