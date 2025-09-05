#pragma once
#include "VIBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL VIBufferTerrain :
    public VIBuffer
{
private:
    VIBufferTerrain();
    virtual ~VIBufferTerrain() = default;

public:
    static VIBufferTerrain* Create(const _string& filePath);
    HRESULT Initialize(const _string& filePath);

private:
    _uint m_iNumVerticesX{};
    _uint m_iNumVerticesZ{};
};

NS_END