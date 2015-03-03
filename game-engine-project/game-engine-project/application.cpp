/*
Recommended Resolutions (W x H)
NOTE: 1440 x 900 = 1,296,000 pixels to render each frame

4:3
=============
800 x 600
1024 x 768
1280 x 960
1440 x 1080

16:9
=============
1280 x 720
1366 x 768
1600 x 900
1920 x 1080
*/

#include "application.h"

//globals
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define WINDOW_TITLE L"In Development Game Engine - Editor"
#define WINDOW_RENDER L"Child Window - Render Surface"
bool m_bWindowed = true;

Application* g_EditorApp = 0;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (!g_EditorApp)
		return DefWindowProc(hwnd, msg, wparam, lparam);
	else
		return g_EditorApp->ProcessWindowMsg(hwnd, msg, wparam, lparam);
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//need to later modify this so that the tool bar resizes itself as the window's dimensions change
HWND Application::CreateToolbar(HWND hWndParent)
{
	const int imageListID = 0;
	const int numButtons = 6;
	const int bitmapSize = 1;		//button size (use 2-16 in the future)

	const DWORD buttonStyle = BTNS_AUTOSIZE;

	//create the toolbar
	HWND hWndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
		WS_CHILD | TBSTYLE_WRAPABLE, 0, 0, 0, 0,
		hWndParent, NULL, m_hInstance, NULL);

	if (hWndToolbar == NULL) { return NULL; }

	//create image list
	m_hImageList = ImageList_Create(bitmapSize, bitmapSize,		//dimensions of individual buttons iX, iY
		ILC_COLOR16 | ILC_MASK,									//ensures transparent background
		numButtons, 0);

	//set image list
	SendMessage(hWndToolbar, TB_SETIMAGELIST,
		(WPARAM)imageListID,
		(LPARAM)m_hImageList);

	//load button images
	//to add custom icons to the toolbar refer to this: http://stackoverflow.com/questions/21743082/how-to-use-custom-icons-for-toolbars-in-winapi-programming
	SendMessage(hWndToolbar, TB_LOADIMAGES,
		(WPARAM)IDB_STD_SMALL_COLOR,
		(LPARAM)HINST_COMMCTRL);

	//init button info
	TBBUTTON tbButtons[numButtons] =
	{
		{ MAKELONG(STD_FILENEW, imageListID), ID_FILE_NEW, TBSTATE_ENABLED, buttonStyle, { 0 }, 0, (INT_PTR)L"New" },
		{ MAKELONG(STD_FILEOPEN, imageListID), ID_FILE_OPEN, TBSTATE_ENABLED, buttonStyle, { 0 }, 0, (INT_PTR)L"Open" },
		{ MAKELONG(STD_FILESAVE, imageListID), ID_FILE_SAVE, TBSTATE_ENABLED, buttonStyle, { 0 }, 0, (INT_PTR)L"Save" },
		{ MAKELONG(NULL, imageListID), ID_TOOLS_TRANSLATE, TBSTATE_ENABLED, buttonStyle, { 0 }, 0, (INT_PTR)L"Trans" },
		{ MAKELONG(NULL, imageListID), ID_TOOLS_ROTATE, TBSTATE_ENABLED, buttonStyle, { 0 }, 0, (INT_PTR)L"Rot" },
		{ MAKELONG(NULL, imageListID), ID_TOOLS_SCALE, TBSTATE_ENABLED, buttonStyle, { 0 }, 0, (INT_PTR)L"Scale" }
	};

	//add buttons
	SendMessage(hWndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	SendMessage(hWndToolbar, TB_ADDBUTTONS, (WPARAM)numButtons, (LPARAM)&tbButtons);

	//resize toolbar, then show it
	SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);
	ShowWindow(hWndToolbar, TRUE);

	return hWndToolbar;
}

//need to later modify this so that the status bar resizes itself as the window's dimensions change
HWND Application::CreateStatusbar(HWND hWndParent, int idStatus, HINSTANCE hInst, int cParts)
{
	//optional tutorials:	http://winapi.foosyerdoos.org.uk/code/commoncntrls/htm/createstatusbar.php
	//						http://winprog.org/tutorial/app_three.html
	HWND hWndStatus;
	RECT rcClient;
	HLOCAL hloc;
	PINT paParts;
	int i, nWidth;

	//MSDN explains what this is: http://msdn.microsoft.com/en-us/library/windows/desktop/bb775697%28v=vs.85%29.aspx
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icc.dwICC = ICC_BAR_CLASSES;
	InitCommonControlsEx(&icc);

	//alternatively, I could of just typed this one line: InitCommonControls();
	//however, InitCommonControls() is outdated and not recommended

	//create status bar
	hWndStatus = CreateWindowEx(
		0,											//no extended styles
		STATUSCLASSNAME,							//the windows status bar class
		(PCTSTR)NULL,								//no text when first created
		SBARS_SIZEGRIP | WS_CHILD | WS_VISIBLE,		//size grip and visible child window
		0, 0, 0, 0,									//ignore size and position
		hWndParent,									//handle to parent window
		(HMENU)idStatus,							//child window identifier
		hInst,										//handle to application instance
		NULL);										//no window creation data

	//get coords of parent window's rect
	GetClientRect(hWndParent, &rcClient);

	//allocate an array for holding the right edge coordinates
	hloc = LocalAlloc(LHND, sizeof(int) * cParts);
	paParts = (PINT)LocalLock(hloc);

	//calculate the right edge coordinate for each part of status bar, and copy the coordinates to the array
	nWidth = rcClient.right / cParts;
	int rightEdge = nWidth;
	for (i = 0; i < cParts; i++) {
		paParts[i] = rightEdge;
		rightEdge += nWidth;
	}

	//tell status bar to create window parts
	SendMessage(hWndStatus, SB_SETPARTS, (WPARAM)cParts, (LPARAM)paParts);

	//put text in the status bar
	//NOTE: cannot put variables in the status bar this way; more research needs to be done
	SendMessage(hWndStatus, SB_SETTEXT, 0, reinterpret_cast<LPARAM>(L"Status Bar: 1st Part"));
	SendMessage(hWndStatus, SB_SETTEXT, 1, reinterpret_cast<LPARAM>(L"2nd Part"));
	//SendMessage(hWndStatus, SB_SETTEXT, 2, nWidth);
	//SendMessage(hWndStatus, SB_SETTEXT, 3 | SBT_POPOUT, reinterpret_cast<LPARAM>(L"Pop-out style"));

	//free array and return
	LocalUnlock(hloc);
	LocalFree(hloc);
	return hWndStatus;
}

//may be relevant information to creating a child window: http://www.winprog.org/tutorial/app_four.html
//second resource: http://blogs.msdn.com/b/oldnewthing/archive/2005/04/22/410773.aspx
//***very relevant resource on getting Wnd width/height: http://stackoverflow.com/questions/615551/how-to-get-width-and-height-from-createwindowex-window-c
//This function is optimized to create the rendering window only

HWND Application::CreateRenderWindow(HWND hWndParent, HINSTANCE hInst)
{
	WNDCLASSEX wndRenderClass;
	ZeroMemory(&wndRenderClass, sizeof(wndRenderClass));

	//set up the window
	wndRenderClass.cbSize = sizeof(WNDCLASSEX);					// size of window structure
	wndRenderClass.lpfnWndProc = (WNDPROC)WndProc;				// message callback
	wndRenderClass.hInstance = m_hInstance;						// handle to the application
	wndRenderClass.style = CS_HREDRAW | CS_VREDRAW;				// style
	wndRenderClass.lpszClassName = WINDOW_RENDER;				// class name
	wndRenderClass.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME);	// background brush
	wndRenderClass.hCursor = LoadCursor(NULL, IDC_ARROW);		// default cursor
	wndRenderClass.hIcon = NULL;								// default icon
	wndRenderClass.hIconSm = NULL;								// default small icon

	//register new type of window
	RegisterClassEx(&wndRenderClass);

	HWND hWndChild;
	RECT childRect;
	RECT rP, rT, rS;

	GetWindowRect(m_hToolWnd, &rT);		//toolbar
	GetWindowRect(m_hStatWnd, &rS);		//status bar
	GetClientRect(hWndParent, &rP);		//parent window

	childRect.left = rP.left;				//same as parent left
	childRect.top = rT.bottom - rT.top;		//height of toolbar: reps where this window should start on x
	childRect.right = rP.right - rP.left;	//same as parent right/width
	LONG a = rT.bottom - rT.top;			//height of toolbar
	LONG b = rS.bottom - rS.top;			//height of status bar
	LONG c = rP.bottom - rP.top;			//height of parent window
	childRect.bottom = c - (a + b);			//the area between toolbar and status bar of the parent window

	hWndChild = CreateWindowEx(
		0,											//no extended styles
		WINDOW_RENDER,								//class name of window
		(PCTSTR)NULL,								//no text when first created
		WS_CHILD | WS_VISIBLE,						//specify DWORD styles
		childRect.left,								//origin of window on x
		childRect.top,								//origin of window on y
		childRect.right,							//width of window
		childRect.bottom,							//height of window
		hWndParent,									//handle to parent window
		(HMENU)ID_CHILD_WND1,						//child window identifier
		hInst,										//handle to application instance
		NULL);										//no window creation data

	ShowWindow(hWndChild, SW_SHOW);

	return hWndChild;
}

void Application::InitMainWindow()
{
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));

	// set up the window
	wndClass.cbSize = sizeof(WNDCLASSEX);					// size of window structure
	wndClass.lpfnWndProc = (WNDPROC)WndProc;				// message callback
	wndClass.hInstance = m_hInstance;						// handle to the application
	wndClass.style = CS_HREDRAW | CS_VREDRAW;				// style
	wndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);		// create menu
	wndClass.lpszClassName = WINDOW_TITLE;					// class name
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME);	// background brush
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);			// default cursor
	wndClass.hIcon = NULL;									// default icon
	wndClass.hIconSm = NULL;								// default small icon

	// register a new type of window
	RegisterClassEx(&wndClass);

	m_hWnd = CreateWindow(
		WINDOW_TITLE, mMainWndCaption.c_str(), 							// window class name, window title
		m_bWindowed ? WS_OVERLAPPEDWINDOW : (WS_EX_TOPMOST | WS_POPUP),	// window style
		CW_USEDEFAULT, CW_USEDEFAULT,									// x and y coordinates
		m_bWindowed ? SCREEN_WIDTH : GetSystemMetrics(SM_CXSCREEN),		// window width
		m_bWindowed ? SCREEN_HEIGHT : GetSystemMetrics(SM_CYSCREEN),	// window height
		NULL, NULL,														// parent window and menu
		m_hInstance,													// handle to application
		NULL);

	// Display the window
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

}

Application::Application(HINSTANCE hInstance)
{
	m_hInstance = hInstance;
	m_hImageList = NULL;	//for toolbar
	sParts = 4;				//for status bar
	bAppPaused = false;
	bMinimized = false;
	bMaximized = false;
	bResizing = false;
	mMainWndCaption = L"Game Editor";

	//create the main window
	InitMainWindow();
	m_hToolWnd = CreateToolbar(m_hWnd);
	m_hStatWnd = CreateStatusbar(m_hWnd, ID_STATUS, m_hInstance, sParts);

	//create the render window
	m_hRenderWnd = CreateRenderWindow(m_hWnd, m_hInstance);

	//send necessary info to render manager
	//changed from using m_hWnd to m_hRenderWnd, however, I have lost some functionality such as alt+enter for fullscreen as
	//a result. This needs to be recoded back in to work with m_hRenderWnd. For now, use just m_hWnd only when you want to see
	//the app ran in full screen with resolution resizing by dragging the side bars of the window.
	gRenderManager.SetWindow(m_hRenderWnd, m_hInstance);
	gRenderManager.SetResolution(SCREEN_WIDTH, SCREEN_HEIGHT);

	//need to create a subsystem manager to init and shutdown subsystems of the engine properly
}

Application::~Application()
{
	
}

void Application::Run()
{
	// Use this msg structure to catch window messages
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	gTimer.Reset();

	//Calculate sec per count
	//__int64 countspersec;
	//QueryPerformanceFrequency((LARGE_INTEGER*)&countspersec);
	//float secpercount = 1.0f / (float)countspersec;

	//Get timestamp
	//__int64 prevtimestamp;
	//QueryPerformanceCounter((LARGE_INTEGER*)&prevtimestamp);
	//__int64 currenttimestamp;
	//float dt;

	msg.message = WM_NULL;

	//startup engine systems in proper order
	gMemoryManager.Startup();
	gFileSystemManager.Startup();
	gVideoManager.Startup();
	//gTextureManager.Startup();
	gInputManager.Startup(m_hWnd, m_hInstance);
	gGameManager.Startup();
	gRenderManager.Startup();
	gAnimationManager.Startup();
	gPhysicsManager.Startup();

	////////////////////////////////////////////////////////////////////////
	//MAIN LOOP
	////////////////////////////////////////////////////////////////////////
	while (msg.message != WM_QUIT)
	{
		//Handle window messages
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			//gCoreMgr->HandleMessage(new SMessageWindow(msg));
			DispatchMessage(&msg);
		}
		else
		{
			gTimer.Tick();
			
			if (!bAppPaused)
			{
				CalculateFrameStats();

				//check for user input
				gInputManager.Update(gTimer.DeltaTime());

				//update the status bar
				//SendMessage(m_hStatWnd, SB_SETTEXT, 0, reinterpret_cast<LPARAM>(L"Updated 1st ") << (LPARAM)(INT)SCREEN_WIDTH);
				//SendMessage(m_hStatWnd, SB_SETTEXT, 1, reinterpret_cast<LPARAM>(L"Updated from application.cpp main loop"));

				//update the game and all systems here
				gRenderManager.Update(gTimer.DeltaTime());
				gRenderManager.Render();

			}
			else
			{
				Sleep(100);
			}
			// Get the current time stamp and find the difference from the prev
			//QueryPerformanceCounter((LARGE_INTEGER*)&currenttimestamp);
			//dt = (float)(currenttimestamp - prevtimestamp) * secpercount;

			//update the game here
			//gCoreMgr->Update(dt);

			// Update the prev time stamp to current
			//prevtimestamp = currenttimestamp;
		}
	}

	//do shutdown stuff here
	//...

	//shutdown engine systems in reverse order
	gPhysicsManager.Shutdown();
	gAnimationManager.Shutdown();
	gRenderManager.Shutdown();
	gGameManager.Shutdown();
	gInputManager.Shutdown();
	//gTextureManager.Shutdown();
	gVideoManager.Shutdown();
	gFileSystemManager.Shutdown();
	gMemoryManager.Shutdown();

	//unregister windows
	UnregisterClass(WINDOW_TITLE, m_hInstance);
	UnregisterClass(WINDOW_RENDER, m_hInstance);
}

LRESULT Application::ProcessWindowMsg(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// attempt to handle your messages
	switch (msg)
	{
		case WM_ACTIVATE: //sent when user activates or deactivate this window
		{
			if (LOWORD(wparam) == WA_INACTIVE)
			{
				bAppPaused = true;
				gTimer.Stop();
			}
			else
			{
				bAppPaused = false;
				gTimer.Start();
			}
		}
		break;
		case WM_SIZE: //sent when the user resizes the window
		{
			//save the new client area dimensions
			gRenderManager.SetResolution(LOWORD(lparam), HIWORD(lparam));

			if (gRenderManager.GetD3DDevice())
			{
				if (wparam == SIZE_MINIMIZED)
				{
					bAppPaused = true;
					bMinimized = true;
					bMaximized = false;
				}
				else if (wparam == SIZE_MAXIMIZED)
				{
					bAppPaused = false;
					bMinimized = false;
					bMaximized = true;
					gRenderManager.OnResize();
				}
				else if (wparam == SIZE_RESTORED)
				{

					//restoring from minimized state?
					if (bMinimized)
					{
						bAppPaused = false;
						bMinimized = false;
						gRenderManager.OnResize();
					}

					//restoring from maximized state?
					else if (bMaximized)
					{
						bAppPaused = false;
						bMaximized = false;
						gRenderManager.OnResize();
					}
					else if (bResizing)
					{
						// If user is dragging the resize bars, we do not resize 
						// the buffers here because as the user continuously 
						// drags the resize bars, a stream of WM_SIZE messages are
						// sent to the window, and it would be pointless (and slow)
						// to resize for each WM_SIZE message received from dragging
						// the resize bars.  So instead, we reset after the user is 
						// done resizing the window and releases the resize bars, which 
						// sends a WM_EXITSIZEMOVE message.
					}
					else //API call such as SetWindowPos or mSwapChain->SetFullscreenState.
					{
						gRenderManager.OnResize();
					}
				}
			}
		}
		break;
		case WM_ENTERSIZEMOVE: //sent when user grabs resize bars
		{
			bAppPaused = true;
			bResizing = true;
			gTimer.Stop();
		}
		break;
		case WM_EXITSIZEMOVE: //sent when user releases resize bars; reset everything based on new window dimensions
		{
			bAppPaused = false;
			bResizing = false;
			gTimer.Start();
			gRenderManager.OnResize();
		}
		break;
		case WM_CLOSE: //Close window
		{
			DestroyWindow(m_hWnd);
			//gCoreMgr->HandleMessage(new SMessageEndApp());
			return 0;
		}
		break;
		case WM_DESTROY: //End program
		{
			PostQuitMessage(0);
			return 0;
		}
		break;
		case WM_MENUCHAR: //sent when a menu is active and user presses a key that does not correspond to any mnemonic or accelerator key
		{
			return MAKELRESULT(0, MNC_CLOSE); //don't beep when we alt-enter
		}
		break;
		//catch this message to prevent the window from becoming too small
		case WM_GETMINMAXINFO:
		{
			((MINMAXINFO*)lparam)->ptMinTrackSize.x = 400;
			((MINMAXINFO*)lparam)->ptMinTrackSize.y = 400;
		}
		break;
		case(WM_KEYDOWN):
		{
			switch (wparam)
			{
				// Remove/Modify this call when creating a real game with menus, etc.
				case VK_ESCAPE:
				{
					//PostQuitMessage(0);
					break;
				}
			}
		}
		break;
		case(WM_COMMAND): //catches msgs (i.e. clicks) sent from menu
		{
			//do stuff here for your menu
			switch (LOWORD(wparam))
			{
				case ID_FILE_NEW:
				{
					//do something...
					break;
				}
				case ID_FILE_OPEN:
				{
					//do something...
					break;
				}
				case ID_FILE_SAVE:
				{
					//do something...
					break;
				}
				case ID_FILE_EXIT:
				{
					PostQuitMessage(0);
					break;
				}
				//do the rest of the menu IDs later
				//toolbar stuff is done here as well
				case ID_TOOLS_TRANSLATE:
				{
					//PostQuitMessage(0);
					break;
				}
			}
		}
	}
	return DefWindowProc(hwnd, msg, wparam, lparam); //supposedly I need to use DefFrameProc() for multiple windows...
}

void Application::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if ((gTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision(6);
		outs << mMainWndCaption << L"    "
			<< L"FPS: " << fps << L"    "
			<< L"Frame Time: " << mspf << L" (ms)";
		SetWindowText(m_hWnd, outs.str().c_str());

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}


