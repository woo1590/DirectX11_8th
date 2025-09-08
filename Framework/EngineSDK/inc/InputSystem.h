#pragma once
#include "Base.h"

NS_BEGIN(Engine)

enum class KeyState { Press, Down, Release, Away, Count };

struct KeyInfo
{
    _bool currPress = false;
    _bool prePress = false;
    KeyState state = KeyState::Count;
};

class InputSystem :
    public Base
{
private:
    InputSystem();
    virtual ~InputSystem();

public:
    static InputSystem * Create(HWND hWnd);
    HRESULT Initialize(HWND hWnd);

    void Update();
    void EndFrame();

	void OnKeyEvent(const RAWKEYBOARD& keyboard);
    void OnMouseEvent(const RAWMOUSE& mouse);

    _bool IsKeyDown(WPARAM key)const;
    _bool IsKeyPressed(WPARAM key)const;
    _bool IsKeyRelease(WPARAM key)const;
    _bool IsKeyAway(WPARAM key)const;

    _bool IsMouseDown(MouseButton button)const;
    _bool IsMousePressed(MouseButton button)const;
    _bool IsMouseRelease(MouseButton button)const;
    _bool IsMouseAway(MouseButton button)const;

    void SetMouseDelta(_float dx, _float dy);
    _float2 GetMouseDelta()const { return m_MouseDelta; }

    void Free()override;

private:
    std::vector<KeyInfo> m_KeyboardInfo;
    std::vector<KeyInfo> m_MouseInfo;

    _float2 m_MouseDelta{};
    _float2 m_MousePosition{};

};

NS_END