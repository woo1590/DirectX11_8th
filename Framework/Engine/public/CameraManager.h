#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CameraComponent;
class CameraManager :
    public Base
{
private:
    CameraManager();
    virtual ~CameraManager() = default;

public:
    static CameraManager* Create();
    HRESULT Initialize();
    void Update();

    void SetMainCamera(const _string& cameraTag);
    void SetShadowCamera(const _string& cameraTag);
    void AddCamera(const _string& cameraTag, CameraComponent* component);

    CAMERA_CONTEXT GetCameraContext()const { return m_MainCameraContext; }
    CAMERA_CONTEXT GetShadowCameraContext()const { return m_ShadowCameraContext; }
    _float3 WorldToScreen(_float3 worldPosition);
    void Free()override;

private:
    std::unordered_map<_string, CameraComponent*> m_Cameras;

    CameraComponent* m_pMainCamera = nullptr;
    CameraComponent* m_pShadowCamera = nullptr;
    CAMERA_CONTEXT m_MainCameraContext{};
    CAMERA_CONTEXT m_ShadowCameraContext{};
};

NS_END