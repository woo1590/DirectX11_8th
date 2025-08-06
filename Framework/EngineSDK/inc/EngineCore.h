#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class RenderSystem;
class SoundManager;
class TimerManager;
class GraphicDevice;
class LevelManager;
class Random;
class ENGINE_DLL EngineCore final :
    public Base
{
    DECLARE_SINGLETON(EngineCore);
private:
    EngineCore();
    virtual ~EngineCore() = default;

public:
    HRESULT Initialize(const EngineDESC& desc);
    void Free()override;
    void Tick(_float dt);

    /*-----Timer-----*/
    HRESULT AddTimer(const std::string& timerTag);
    void UpdateTimer(const std::string& timerTag);
    _float GetDeltaTime(const std::string& timerTag);

    /*-----Random-----*/


    /*---Graphic Device---*/
    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetDeviceContext();

private:
    /*----Rendering----*/
    HRESULT DrawBegin();
    HRESULT Draw();
    HRESULT DrawEnd();

    HWND hWnd{};

    Random* random = nullptr;
    RenderSystem* renderSystem = nullptr;
    SoundManager* soundManager = nullptr;
    TimerManager* timerManager = nullptr;
    GraphicDevice* graphicDevice = nullptr;
    LevelManager* levelManager = nullptr;

};

NS_END