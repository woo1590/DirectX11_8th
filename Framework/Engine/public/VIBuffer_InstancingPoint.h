#pragma once
#include "VIBuffer.h"

NS_BEGIN(Engine)

class VIBuffer_InstancingPoint :
    public VIBuffer
{
private:
    VIBuffer_InstancingPoint();
    virtual ~VIBuffer_InstancingPoint() = default;

public:
    static VIBuffer_InstancingPoint* Create(_uint bufferLevelID, const _string& bufferTag, _uint numInstance, VTX_INSTANCE_POINT* instanceInitData);
    HRESULT Initialize(_uint bufferLevelID, const _string& bufferTag, _uint numInstance, VTX_INSTANCE_POINT* instanceInitData);

    HRESULT BindBuffers();
    HRESULT Draw();

    void UploadInstanceData(std::vector<VTX_INSTANCE_POINT>& instanceDatas, _uint numInstance);

    void Free()override;
private:
    VIBuffer* m_pBaseBuffer = nullptr;
    ID3D11Buffer* m_pInstanceBuffer = nullptr;

    _uint m_iNumMaxInstance{};
    _uint m_iNumCurrInstance{};
    _uint m_iInstanceStride{};
};

NS_END