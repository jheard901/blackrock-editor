/*
Started on 12/24/14
Created by Javier Heard

use this in header files so you do not need to hardcode paths
#pragma comment(lib, "filename.lib")


===========================================
Coding Conventions Used
===========================================
Class Names:		ExampleClass
Function Names:		ExampleFunction()
Variable Names:		exampleVarName
Const Var Names:	EXAMPLEVARNAME


NOTES:

1/15/15
For now, this application will not use multithreaded code until I become
familiar with the Boost library and get far enough with developing it to
the point where I can actually benefit from having multithreading (i.e.
start doing lots of rendering).

1/21/15
I had a bug where I got 112 warning that were all macro redifinitions. I
found out this was caused because in the property manager of Microsoft.Cpp.Win32.user
the include and lib directories inherited values from VS2013 naturally which
are the latest directories to use: $(VC_IncludePath) and $(WindowsSDK_IncludePath).
These conflicted with the macro definitions from the DirectX SDK since the DX SDK is
considered outdated compared to the Windows SDK. I fixed it by exclusively specifying
to use the Direct X directories: $(IncludePath) and $(DXSDK_DIR)Include. Additionally,
an alternative fix was to specify in the project's include: $(WindowsSDK_IncludePath).
This causes the newer SDK to be the default, preventing conflict errors since it will
not use the older SDK. For more info on this refer to the following links:
http://stackoverflow.com/questions/12663091/macro-definition-clash-between-directx-headers-and-winerror-h
https://msdn.microsoft.com/en-us/library/windows/desktop/ee663275%28v=vs.85%29.aspx#sdk_vs11
*/

//shared libs linked here
#pragma comment(lib, "dxguid.lib")



#include "windows.h"
#include "application.h"

// Entry point for the game or application.
int WINAPI wWinMain(HINSTANCE hInstance,	// Handle to the application
	HINSTANCE hPrevInstance,				// Handle to the previous app
	LPTSTR lpCmdLine,						// Command line string
	int lpCmdShow);							// Show window value




int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	Application g_EdApp(hInstance);
	g_EditorApp = &g_EdApp;
	g_EdApp.Run();	
}

