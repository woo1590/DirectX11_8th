#include "EnginePCH.h"
#include "InputSystem.h"


InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

InputSystem * InputSystem::Create(HWND hWnd)
{
	InputSystem* Instance = new InputSystem();

	if (FAILED(Instance->Initialize(hWnd)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT InputSystem::Initialize(HWND hWnd)
{
	/*-----Register RawInputDevice-----*/
	RAWINPUTDEVICE device[2]{};

	device[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	device[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	device[0].dwFlags = 0;
	device[0].hwndTarget = hWnd;

	device[1].usUsagePage = HID_USAGE_PAGE_GENERIC;
	device[1].usUsage = HID_USAGE_GENERIC_KEYBOARD;
	device[1].dwFlags = 0;
	device[1].hwndTarget = hWnd;

	if (!RegisterRawInputDevices(device, 2, sizeof(RAWINPUTDEVICE)))
		return E_FAIL;

	m_KeyboardInfo.resize(256);
	m_MouseInfo.resize(2);

	return S_OK;
}

void InputSystem::Update()
{
	/*---Update Keyboard---*/
	for (auto& key : m_KeyboardInfo)
	{
		if (key.currPress)					//누름
		{	
			if (key.prePress)				//이전에도 눌렀음
				key.state = KeyState::Down;
			else
				key.state = KeyState::Press;//이번 프레임에 딱 누르기 시작
		}
		else
		{
			if (key.prePress)				//지금은 안눌렀는데 이전에는 눌렀음
				key.state = KeyState::Release;
			else
				key.state = KeyState::Away;	//지금도 안누르고 이전에도 안누름
		}

		key.prePress = key.currPress;
	}

	/*---Update Mouse---*/
	for (auto& button : m_MouseInfo)
	{
		if (button.currPress)
		{
			if (button.prePress)
				button.state = KeyState::Down;
			else
				button.state = KeyState::Press;
		}
		else
		{
			if (button.prePress)
				button.state = KeyState::Release;
			else
				button.state = KeyState::Away;
		}

		button.prePress = button.currPress;
	}
}

void InputSystem::EndFrame()
{
	m_MouseDelta = { 0.f,0.f };
}

void InputSystem::OnKeyEvent(const RAWKEYBOARD& keyboard)
{
	_ushort flag = keyboard.Flags;
	_ushort vkey = keyboard.VKey;

	_bool keyDown = !(flag & RI_KEY_BREAK);	//0이 눌림 1이 뗌

	if (keyDown)
		m_KeyboardInfo[vkey].currPress = true;
	else
		m_KeyboardInfo[vkey].currPress = false;	
}

void InputSystem::OnMouseEvent(const RAWMOUSE& mouse)
{
	_ushort flag = mouse.usButtonFlags;

	if (flag & RI_MOUSE_LEFT_BUTTON_DOWN)
		m_MouseInfo[0].currPress = true;
	else if (flag & RI_MOUSE_LEFT_BUTTON_UP)
		m_MouseInfo[0].currPress = false;

	if (flag & RI_MOUSE_RIGHT_BUTTON_DOWN)
		m_MouseInfo[1].currPress = true;
	else if (flag & RI_MOUSE_RIGHT_BUTTON_UP)
		m_MouseInfo[1].currPress = false;
}

void InputSystem::SetMouseDelta(_float dx, _float dy)
{
	m_MouseDelta = { dx,dy };
}

_bool InputSystem::IsKeyDown(WPARAM key) const
{
	return m_KeyboardInfo[key].state == KeyState::Down;
}

_bool InputSystem::IsKeyPressed(WPARAM key) const
{
	return m_KeyboardInfo[key].state == KeyState::Press;
}

_bool InputSystem::IsKeyRelease(WPARAM key) const
{
	return m_KeyboardInfo[key].state == KeyState::Release;
}

_bool InputSystem::IsKeyAway(WPARAM key) const
{
	return m_KeyboardInfo[key].state == KeyState::Away;
}

_bool InputSystem::IsMouseDown(MouseButton button) const
{
	return m_MouseInfo[ENUM_CLASS(button)].state == KeyState::Down;
}

_bool InputSystem::IsMousePressed(MouseButton button) const
{
	return m_MouseInfo[ENUM_CLASS(button)].state == KeyState::Press;
}

_bool InputSystem::IsMouseRelease(MouseButton button) const
{
	return m_MouseInfo[ENUM_CLASS(button)].state == KeyState::Release;
}

_bool InputSystem::IsMouseAway(MouseButton button) const
{
	return m_MouseInfo[ENUM_CLASS(button)].state == KeyState::Release;
}

void InputSystem::Free()
{
	__super::Free();
}
