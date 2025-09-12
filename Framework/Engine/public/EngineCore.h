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
class InputSystem;
class Renderer;
class LightManager;

class VIBuffer;
class Shader;
class Texture;
class Material;
class Object;
class Layer;
class Model;
class LightComponent;
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

#pragma region ImGui
    _bool WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#pragma endregion

#endif

#pragma region Timer
    HRESULT AddTimer(const std::string& timerTag);
    void UpdateTimer(const std::string& timerTag);
    _float GetDeltaTime(const std::string& timerTag);
#pragma endregion

#pragma region Sound
    void Load3DSound(const _string& key, const _string& filePath, _bool loop = false);
    void Load2DSound(const _string& key, const _string& filePath, _bool loop = false);
    FMOD::Channel* PlaySFX(const _string& soundTag);
    void PlayBGM(const _string& soundTag);
#pragma endregion

#pragma region GraphicDevice
    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetDeviceContext();
#pragma endregion

#pragma region PipeLine
    void SetViewMatrix(_float4x4 viewMatrix);
    void SetProjMatrix(_float4x4 projMatrix);
    _float4x4 GetViewMatrix();
    _float4x4 GetViewMatrixInverse();
    _float4x4 GetProjMatrix();
    _float4x4 GetProjMatrixInverse();
    _float3 GetCamPosition();
#pragma endregion

#pragma region Resource
    HRESULT LoadBuffer(_uint levelID, const _string& key, VIBuffer* pBuffer);
    HRESULT LoadShaderFromFile(_uint levelID, const _string& filePath, const _string& key,
                               const D3D11_INPUT_ELEMENT_DESC* pElement, _uint numElement);
    HRESULT LoadMaterialFromJson(_uint levelID, const _string& filePath, const _string& key);
    HRESULT LoadModelFromFile(_uint levelID, const _string& filePath, const _string& key);

    VIBuffer* GetBuffer(_uint levelID, const _string& key);
    Shader* GetShader(_uint levelID, const _string& key);
    Material* GetMaterial(_uint levelID, const _string& key);
    Model* GetModel(_uint levelID, const _string& key);
#pragma endregion

#pragma region Prototype
    HRESULT AddPrototype(_uint level, const _string& prototypeTag, Object* prototype);
    Object* ClonePrototype(_uint level, const _string& prototypeTag, InitDESC* arg);
#pragma endregion

#pragma region Object
    HRESULT AddObject(_uint prototypeLevel, const _string& prototypeTag, _uint layerLevel, const _string& layerTag, InitDESC* arg = nullptr);
    std::unordered_map<_string, Layer*>& GetLayers(_uint levelID);
#pragma endregion

#pragma region Level
    void ChangeLevel(_uint levelID, Level* nextLevel);
    Level* GetCurrLevel()const;
    void ClearResource(_uint levelID);
#pragma endregion

#pragma region RenderSystem
    Renderer* GetRenderer()const;
#pragma endregion

#pragma region InputSystem
    void OnKeyEvent(const RAWKEYBOARD& keyboard);
    void OnMouseEvent(const RAWMOUSE& mouse);
    void SetMouseDelta(_float dx, _float dy);
    _float2 GetMouseDelta()const;

    _bool IsKeyDown(WPARAM key)const;
    _bool IsKeyPressed(WPARAM key)const;
    _bool IsKeyRelease(WPARAM key)const;
    _bool IsKeyAway(WPARAM key)const;

    _bool IsMouseDown(MouseButton button)const;
    _bool IsMousePress(MouseButton button)const;
    _bool IsMouseRelease(MouseButton button)const;
    _bool IsMouseAway(MouseButton button)const;
#pragma endregion

#pragma region Light
    void RegisterLight(LightComponent* light);
    void UnRegisterLight(LightComponent* light);
#pragma endregion

    /*----Window----*/
    HWND GetWindowHandle()const { return m_hWnd; }
    D3D11_VIEWPORT GetViewport()const { return m_Viewport; }
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    /*----Rendering----*/
    HRESULT BeginRender();
    HRESULT Render();
    HRESULT EndRender();

    HWND m_hWnd{};
    D3D11_VIEWPORT m_Viewport{};

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
    InputSystem*        m_pInputSystem = nullptr;
    LightManager*       m_pLightManager = nullptr;
};

NS_END