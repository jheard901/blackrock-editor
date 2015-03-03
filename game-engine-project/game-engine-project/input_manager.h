//using dinput for keyboard and mouse, and probably XInput
//for gamepad support. Therefore, I should make a separate header file and
//class for XInput.

#pragma once

#pragma comment(lib, "dinput8.lib")		//dxguid.lib is linked from main.cpp

#define DIRECTINPUT_VERSION 0x0800	//use the latest version of DInput

#include <dinput.h>

//handles getting input
class InputManager
{
private:
	IDirectInput8*			m_pDInput;				// Direct input device
	IDirectInputDevice8*	m_pDKeyboard;			// Keyboard device
	char					m_dKeyboardState[256];	// Records state of keyboard
	IDirectInputDevice8*	m_pDMouse;				// Mouse device
	DIMOUSESTATE2			m_dMouseState;			// Records state of mouse
	
protected:
	bool		bPressed;
	bool		bReleased;	//might need multiple bPressed vars if I want to handle allowing more than one input at a time
	float		mouseX;
	float		mouseY;
public:
	InputManager();
	~InputManager();
	void Update(float dt);	//might need to make this a virtual function in order to make a child class from this class that customizes it controls specific to the application / classes that need to use it
	void Startup(HWND &hWnd, HINSTANCE &hInst);
	void Shutdown();

	//keyboard
	bool KeyDown(int key);
	bool KeyUp(int key);
	bool KeyDownOnce(int key);

	//mouse
	bool MouseButtonDown(int button);
	bool MouseButtonUp(int button);
	bool MouseButtonDownOnce(int button);
	float MouseDX();	//used for checking change in X position of mouse
	float MouseDY();	//used for checking change in Y position of mouse
};