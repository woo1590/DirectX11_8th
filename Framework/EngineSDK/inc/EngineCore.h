#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class RenderSystem;
class SoundManager;
class TimerManager;
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

    /*-----Timer-----*/
    HRESULT AddTimer(const std::string& timerTag);
    void Update(const std::string& timerTag);
    _float GetDeltaTime(const std::string& timerTag);

    /*-----Random-----*/

private:
    HWND hWnd{};

    Random* random = nullptr;
    RenderSystem* renderSystem = nullptr;
    SoundManager* soundManager = nullptr;
    TimerManager* timerManager = nullptr;

};

NS_END