#pragma once

#if defined(DEBUG) || defined(_DEBUG)
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
#endif

//#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d11.lib")

// If in DEBUG mode, turn on D3D_DEBUG_INFO flag enable DXTrace, and d3dx11d.lib (debug version)
#if defined(DEBUG) | defined(_DEBUG)
	#ifndef D3D_DEBUG_INFO
		#define D3D_DEBUG_INFO
	#endif

	#pragma comment(lib, "d3dx11d.lib")
#else
	#pragma comment(lib, "d3dx11.lib")
#endif

#pragma comment(lib, "D3DCompiler.lib")

// If in DEBUG mode, enable Effects11d.lib (debug version)
#if defined(DEBUG) | defined(_DEBUG)
	#pragma comment(lib, "Effects11d.lib")
#else
	#pragma comment(lib, "Effects11.lib")
#endif

#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "dxgi.lib")
//#pragma comment(lib, "dxguid.lib") //moved this to main.cpp since this lib is shared with direct input

#include <D3D11.h>
#include <D3DX11.h>
#include "d3dx11Effect.h"
#include <xnamath.h> //for creating colors and XMVectors
#include <DxErr.h>
#include <cassert> //for assert(x) macro
#include <fstream>	//used in BuildFX() function in rendermanager.cpp
#include <vector>
#include "light_helper.h"

// Macro to release COM objects fast and safely
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}

//macro to delete objects
#define SAFE_DELETE(x) { delete x; x = 0; }

// HRESULT macro that prints an error window
//#ifndef HR
//#define HR(x)												\
//		{                                                   \
//		HRESULT hr = x;									    \
//		if(FAILED(hr))									    \
//			{                                               \
//			DXTraceA(__FILE__, __LINE__, hr, #x, TRUE);	    \
//			}                                               \
//		}                                                   
//#endif

//alternate HRESULT macro that checks for errors; used by book
#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)                                              \
	{                                                          \
		HRESULT hr = (x);                                      \
		if(FAILED(hr))                                         \
		{                                                      \
			DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true); \
		}                                                      \
	}
	#endif
#else
	#ifndef HR
		#define HR(x) (x)
	#endif
#endif 

namespace Colors
{
	XMGLOBALCONST XMVECTORF32 White = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };

	XMGLOBALCONST XMVECTORF32 Silver = { 0.75f, 0.75f, 0.75f, 1.0f };
	XMGLOBALCONST XMVECTORF32 LightSteelBlue = { 0.69f, 0.77f, 0.87f, 1.0f };	//this works good as well
	XMGLOBALCONST XMVECTORF32 ClearColor = { 0.569f, 0.569f, 0.569f, 1.0f };	//preferred color to use for back buffer
	XMGLOBALCONST XMVECTORF32 ClearColorB = { 0.0f, 0.4f, 0.8f, 1.0f };			//blue clear color
	XMGLOBALCONST XMVECTORF32 ClearColorC = { 0.0f, 0.35f, 0.55f, 1.0f };		//alternate blue clear color
}

