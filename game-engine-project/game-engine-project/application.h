#pragma once

#pragma comment(lib, "Comctl32.lib")	//for toolbar

#include <windows.h>
//#include <windowsx.h>					//this is not needed because it is a bunch of pre-defined macros; you can read up on it here: http://support2.microsoft.com/default.aspx?scid=KB;en-us;q83456
#include <sstream>						//used in CalculateFrameStats()
#include <string>
#include <CommCtrl.h>					//for toolbar

//game subsystems
#include "animation_manager.h"
#include "filesystem_manager.h"
#include "memory_manager.h"
#include "physics_manager.h"
#include "render_manager.h"
#include "texture_manager.h"
#include "video_manager.h"
#include "game_manager.h"
#include "input_manager.h"

#include "resource.h"
#include "game_timer.h"




class Application
{
private:
	HWND CreateToolbar(HWND hWndParent);
	HWND CreateStatusbar(HWND hWndParent, int idStatus, HINSTANCE hInst, int cParts); //cParts = how many parts to divide status bar into; idStatus = child window identifier of status bar
	HWND CreateRenderWindow(HWND hWndParent, HINSTANCE hInst);
	void InitMainWindow();

	HINSTANCE	m_hInstance;
	HIMAGELIST	m_hImageList;
	int			sParts; //number of sections for status bar

	//windows to manage; I need to research a better way of managing multiple windows in an application
	HWND		m_hWnd;
	HWND		m_hToolWnd;
	HWND		m_hStatWnd;
	HWND		m_hRenderWnd;
protected:
	bool		bAppPaused;
	bool		bMinimized;
	bool		bMaximized;
	bool		bResizing;
public:
	Application(HINSTANCE hInstance);
	~Application();
	void Run();
	LRESULT ProcessWindowMsg(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void CalculateFrameStats();

	//create subsytems
	AnimationManager		gAnimationManager;
	FileSystemManager		gFileSystemManager;
	MemoryManager			gMemoryManager;
	PhysicsManager			gPhysicsManager;
	RenderManager			gRenderManager;
	//TextureManager		gTextureManager;	//placed inside of render_manager.h/cpp
	VideoManager			gVideoManager;
	GameManager				gGameManager;
	InputManager			gInputManager;

	GameTimer				gTimer;
	std::wstring			mMainWndCaption;
};

//let other files be able to use gEditorApp
extern Application* g_EditorApp;
