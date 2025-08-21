#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class RenderSystem;
class SoundManager;
class TimerManager;
class GraphicDevice;
class LevelManager;
class TaskManager;
class ObjectManager;
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

    /*-----Prototype-----*/
    HRESULT AddPrototype(_uint level, const _string& prototypeTag, Base* prototype);
    Base* ClonePrototype(Prototype type, _uint level, const _string& prototypeTag, void* arg);

    /*-----Object-----*/
    HRESULT AddObject(_uint prototypeLevel, const _string& prototypeTag, _uint layerLevel, const _string& layerTag, void* arg = nullptr);

    /*-----Level-----*/
    void ChangeLevel(_uint levelID, Level* nextLevel);
    void ClearResource(_uint levelID);

    /*----Window----*/
    HWND GetWindowHandle() { return hWnd; }
private:
    /*----Rendering----*/
    HRESULT BeginDraw();
    HRESULT Draw();
    HRESULT EndDraw();

    HWND hWnd{};

    Random*             random = nullptr;
    RenderSystem*       renderSystem = nullptr;
    SoundManager*       soundManager = nullptr;
    TimerManager*       timerManager = nullptr;
    GraphicDevice*      graphicDevice = nullptr;
    LevelManager*       levelManager = nullptr;
    PrototypeManager*   prototypeManager = nullptr;
    ObjectManager*      objectManager = nullptr;
    TaskManager*        taskManager = nullptr;

};

NS_END