#pragma once
#include "Base.h"

NS_BEGIN(Engine)

struct GPUBufferData
{
    ID3D11Buffer* VB{};
    ID3D11Buffer* IB{};
    _uint numVertices{};
    _uint numIndices{};

    DXGI_FORMAT indexFormat{};
    D3D11_PRIMITIVE_TOPOLOGY primitiveTopology;
};

class VIBuffer;
class Shader;
class ENGINE_DLL Renderer :
    public Base
{
private:
    Renderer(ID3D11DeviceContext* pDeviceContext);
    virtual ~Renderer() = default;

public:
    static Renderer* Create(ID3D11DeviceContext* pDeviceContext);
    HRESULT Initialize();

    _int UploadBuffer(VIBuffer* pBuffer);
    void Free()override;

private:
    HRESULT DrawMesh(_uint meshID);
private:
    /*Renderer는 렌더링 전용 스레드에서만 사용됨 -> context도 다른 스레드에서는 사용 x 여기서만 사용해야됨*/
    ID3D11DeviceContext* m_pDeviceContext = nullptr;

    _int m_iNextBufferID = 0;
    std::vector<GPUBufferData> m_GPUBufferDatas;
    std::unordered_map<_string, Shader*> m_ChachedShaders;
    
};

NS_END