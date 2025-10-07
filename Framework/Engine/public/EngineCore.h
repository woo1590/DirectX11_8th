#pragma once
#include "Base.h"

NS_BEGIN(Engine)

#ifdef USE_IMGUI
class ImGuiManager;
#endif

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
    void AddPanel(class IPanel* panel);
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
    HRESULT LoadBuffer(_uint levelID, const _string& key, class VIBuffer* pBuffer);
    HRESULT LoadShaderFromFile(const _string& filePath, const _string& key,
                               const D3D11_INPUT_ELEMENT_DESC* pElement, _uint numElement);
    HRESULT LoadMaterialFromJson(_uint levelID, const _string& filePath, const _string& key);
    HRESULT LoadModelFromFile(_uint levelID, const _string& filePath, const _string& key);
    HRESULT LoadAnimationSetFromFile(_uint levelID, const _string& filePath, const _string& key);

    class VIBuffer* GetBuffer(_uint levelID, const _string& key);
    class Shader* GetShader(const _string& key);
    class Material* GetMaterial(_uint levelID, const _string& key);
    class Model* GetModel(_uint levelID, const _string& key);
    ANIMATION_SET GetAnimation(_uint levelID, const _string& key);
#pragma endregion

#pragma region Prototype
    HRESULT AddPrototype(_uint level, const _string& prototypeTag, class Object* prototype);
    class Object* ClonePrototype(_uint level, const _string& prototypeTag, InitDESC* arg);
#pragma endregion

#pragma region Object
    HRESULT AddObject(_uint prototypeLevel, const _string& prototypeTag, _uint layerLevel, const _string& layerTag, InitDESC* arg = nullptr);
    std::unordered_map<_string, class Layer*>& GetLayers(_uint levelID);
#pragma endregion

#pragma region Level
    void ChangeLevel(_uint levelID, class Level* nextLevel);
    class Level* GetCurrLevel()const;
    void ClearResource(_uint levelID);
#pragma endregion

#pragma region RenderSystem
    class Renderer* GetRenderer()const;
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

#pragma region NavigationSystem
    void RegisterNavigation(class NavigationComponent* component);
    void UnRegisterNavigation(class NavigationComponent* component);
#pragma endregion

#pragma region LightSystem
    void RegisterLight(class LightComponent* light);
    void UnRegisterLight(class LightComponent* light);
#pragma endregion

    class Random* GetRandom()const { return m_pRandom; }
    
    /*Debug setting*/
    void DebugEnable() { m_isDebugEnable = true; }
    void DebugDisable() { m_isDebugEnable = false; }
    _bool IsDebugEnable()const { return m_isDebugEnable; }

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

    class Random*             m_pRandom = nullptr;
    class RenderSystem*       m_pRenderSystem = nullptr;
    class SoundManager*       m_pSoundManager = nullptr;
    class TimerManager*       m_pTimerManager = nullptr;
    class GraphicDevice*      m_pGraphicDevice = nullptr;

#ifdef USE_IMGUI
    ImGuiManager*       m_pImGuiManager = nullptr;
#endif

    class LevelManager*       m_pLevelManager = nullptr;
    class PrototypeManager*   m_pPrototypeManager = nullptr;
    class ObjectManager*      m_pObjectManager = nullptr;
    class TaskManager*        m_pTaskManager = nullptr;
    class PipeLine*           m_pPipeLine = nullptr;
    // 경인이 왔다감
    class ResourceManager*    m_pResourceManager = nullptr;
    class InputSystem*        m_pInputSystem = nullptr;
    class LightSystem*       m_pLightManager = nullptr;
    class NavigationSystem*   m_pNavigationSystem = nullptr;

    _bool m_isDebugEnable = true;
};

NS_END