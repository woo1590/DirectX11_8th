#pragma once
#include "VIBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL Mesh :
    public VIBuffer
{
private:
    Mesh();
    virtual ~Mesh() = default;

public:
    static Mesh* Create(const MESH_FORMAT& meshFormat, std::vector<VTX_FORMAT>& vertexFormats,std::vector<_uint>& indexFormats);
    HRESULT Initialize(const MESH_FORMAT& meshFormat, std::vector<VTX_FORMAT>& vertexFormats, std::vector<_uint>& indexFormats);

    _uint GetMaterialIndex()const { return m_iMaterialIndex; }
    void Free()override;

private:
    _uint m_iMaterialIndex{};
};

NS_END