#pragma once
#include "Base.h"

NS_BEGIN(Engine)

#ifdef USE_IMGUI
class ImGuiManager;
#endif

class RenderSystem;
class SoundManager;
class TimerManager;
class GraphicDevice;
class LevelManager;
class TaskManager;
class ObjectManager;
class ResourceManager;
class PrototypeManager;
class PipeLine;
class Level;
class Random;
class Renderer;

class VIBuffer;
class Shader;
class Object;
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

#ifdef USE_IMGUI
    /*---ImGui---*/
    _bool WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void AddImGuiWindow(const _string& tag, std::function<void()> window);
#endif

    /*-----Timer-----*/
    HRESULT AddTimer(const std::string& timerTag);
    void UpdateTimer(const std::string& timerTag);
    _float GetDeltaTime(const std::string& timerTag);

    /*-----Random-----*/

    /*----Sound----*/
    void LoadSound(const std::string& key, const std::string& filepath, bool loop = false);
    void PlaySFX(const std::string& key);
    void PlayBGM(const std::string& key);
    void StopSound(const std::string& key);

    /*---Graphic Device---*/
    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetDeviceContext();

    /*----PipeLine----*/
    void SetViewMatrix(_float4x4 viewMatrix);
    void SetProjMatrix(_float4x4 projMatrix);
    _float4x4 GetViewMatrix();
    _float4x4 GetViewMatrixInverse();
    _float4x4 GetProjMatrix();
    _float4x4 GetProjMatrixInverse();
    _float3 GetCamPosition();

    /*----Resource----*/
    HRESULT LoadBuffer(_uint levelID, const _string& key, VIBuffer* pBuffer);
    HRESULT LoadShader(_uint levelID, const _string& filePath, const _string& key, const D3D11_INPUT_ELEMENT_DESC* pElement, _uint numElement);
    VIBuffer* GetBuffer(_uint levelID, const _string& key);
    Shader* GetShader(_uint levelID, const _string& key);

    /*-----Prototype-----*/
    HRESULT AddPrototype(_uint level, const _string& prototypeTag, Object* prototype);
    Object* ClonePrototype(_uint level, const _string& prototypeTag, InitDESC* arg);

    /*-----Object-----*/
    HRESULT AddObject(_uint prototypeLevel, const _string& prototypeTag, _uint layerLevel, const _string& layerTag, InitDESC* arg = nullptr);

    /*-----Level-----*/
    void ChangeLevel(_uint levelID, Level* nextLevel);
    void ClearResource(_uint levelID);

    /*----Window----*/
    HWND GetWindowHandle() { return hWnd; }

    /*----RenderSystem----*/
    Renderer* GetRenderer()const;

private:
    /*----Rendering----*/
    HRESULT BeginRender();
    HRESULT Render();
    HRESULT EndRender();

    HWND hWnd{};

    Random*             m_pRandom = nullptr;
    RenderSystem*       m_pRenderSystem = nullptr;
    SoundManager*       m_pSoundManager = nullptr;
    TimerManager*       m_pTimerManager = nullptr;
    GraphicDevice*      m_pGraphicDevice = nullptr;

#ifdef USE_IMGUI
    ImGuiManager*       m_pImGuiManager = nullptr;
#endif

    LevelManager*       m_pLevelManager = nullptr;
    PrototypeManager*   m_pPrototypeManager = nullptr;
    ObjectManager*      m_pObjectManager = nullptr;
    TaskManager*        m_pTaskManager = nullptr;
    PipeLine*           m_pPipeLine = nullptr;
    ResourceManager*    m_pResourceManager = nullptr;
};

NS_END