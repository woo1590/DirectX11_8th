#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class RenderSystem;
class SoundManager;
class TimerManager;
class GraphicDevice;
class LevelManager;
class TaskManager;
class PrototypeManager;
class Level;
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

    /*----Sound----*/
    void LoadSound(const std::string& key, const std::string& filepath, bool loop = false);
    void PlaySFX(const std::string& key);
    void PlayBGM(const std::string& key);
    void Stop(const std::string& key);

    /*---Graphic Device---*/
    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetDeviceContext();

    /*-----Level-----*/
    void ChangeLevel(Level* nextLevel);

private:
    /*----Rendering----*/
    HRESULT BeginDraw();
    HRESULT Draw();
    HRESULT EndDraw();

    HWND hWnd{};

    Random* random = nullptr;
    RenderSystem* renderSystem = nullptr;
    SoundManager* soundManager = nullptr;
    TimerManager* timerManager = nullptr;
    GraphicDevice* graphicDevice = nullptr;
    LevelManager* levelManager = nullptr;
    PrototypeManager* prototypeManager = nullptr;
    TaskManager* taskManager = nullptr;

};

NS_END