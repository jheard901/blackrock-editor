#include "input_manager.h"

InputManager::InputManager()
{

}

InputManager::~InputManager()
{

}

void InputManager::Update(float dt)
{

	///////////////////////////
	//Keyboard Input Commands//
	///////////////////////////

	//update keyboard device
	m_pDKeyboard->GetDeviceState(sizeof(m_dKeyboardState), (LPVOID)&m_dKeyboardState);

	//W key pressed
	if (KeyDown(DIK_W))
	{
		//...
	}
	//A key pressed
	if (KeyDown(DIK_A))
	{
		//...
	}
	//S key pressed
	if (KeyDown(DIK_S))
	{
		//...
	}
	//D key pressed
	if (KeyDown(DIK_D))
	{
		//...
	}

	//acquire state of keyboard again
	m_pDKeyboard->Acquire();


	////////////////////////
	//Mouse Input Commands//
	////////////////////////

	//update mouse device
	m_pDMouse->GetDeviceState(sizeof(m_dMouseState), &m_dMouseState);

	//Mouse X pos changed

	//moves right
	if (MouseDX() >= 1.0f)	//the number represents how much the position has to change since the last update for this to occur
	{
		//...
	}
	//moves left
	if (MouseDX() <= -1.0f)
	{
		//...
	}

	//Mouse Y pos changed

	//moves down
	if (MouseDY() >= 1.0f)
	{
		//...
	}
	//moves up
	if (MouseDY() <= -1.0f)
	{
		//...
	}


	//Mouse button 0 pressed
	if (MouseButtonDown(0))
	{
		//...
	}
	//Mouse button 1 pressed
	if (MouseButtonDown(1))
	{
		//...
	}

	//acquire state of mouse again
	m_pDMouse->Acquire();

}

void InputManager::Startup(HWND &hWnd, HINSTANCE &hInst)
{
	//Create DirectInput object
	DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDInput, NULL);

	//Create DirectInput keyboard device
	m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pDKeyboard, NULL);
	m_pDKeyboard->SetDataFormat(&c_dfDIKeyboard);
	m_pDKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	m_pDKeyboard->Acquire();

	//Create DirectInput mouse device
	m_pDInput->CreateDevice(GUID_SysMouse, &m_pDMouse, NULL);
	m_pDMouse->SetDataFormat(&c_dfDIMouse2);
	m_pDMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	m_pDMouse->Acquire();
}

void InputManager::Shutdown()
{
	m_pDKeyboard->Unacquire();
	m_pDMouse->Unacquire();
}

bool InputManager::KeyDown(int key)
{
	if (m_dKeyboardState[key] & 0x80)
	{
		return true;
	}
	else { return false; }
}

bool InputManager::KeyUp(int key)
{
	return false;
}

bool InputManager::KeyDownOnce(int key)
{
	return false;
}

bool InputManager::MouseButtonDown(int button)
{
	if (m_dMouseState.rgbButtons[button] & 0x80)
	{
		return true;
	}
	else { return false; }
}

bool InputManager::MouseButtonUp(int button)
{
	return false;
}

bool InputManager::MouseButtonDownOnce(int button)
{
	return false;
}

//returns change in distance of mouse X position
float InputManager::MouseDX()
{
	return (float)m_dMouseState.lX;
}

//returns change in distance of mouse Y position
float InputManager::MouseDY()
{
	return (float)m_dMouseState.lY;
}



