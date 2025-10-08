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
    void AddCamera(const _string& cameraTag, CameraComponent* component);
    CAMERA_CONTEXT GetCameraContext()const { return m_MainCameraContext; }
    void Free()override;

private:
    std::unordered_map<_string, CameraComponent*> m_Cameras;

    CameraComponent* m_MainCamera = nullptr;
    CAMERA_CONTEXT m_MainCameraContext{};
};

NS_END