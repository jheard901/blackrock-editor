#pragma once

#include "render_util.h"
#include "texture_manager.h"

//NOTE: using m_p as a prefix if a D3D var is a pointer; else, just use m_

//Useful information on .hlsl and .fx that may come in handy:
//http://stackoverflow.com/questions/27326461/cant-figure-out-shaders-for-directx11
//http://irrlicht.sourceforge.net/forum/viewtopic.php?p=226035
//https://books.google.com/books?id=GY-AAwAAQBAJ&pg=PA62&lpg=PA62&dq=are+.hlsl+files+ever+used?&source=bl&ots=q9TKJkmRDs&sig=4xMOlyOwvt-AXJ0cnqDFXDTa4Mc&hl=en&sa=X&ei=Z0vFVIG8N8qvggSet4OYBQ&ved=0CFUQ6AEwCQ#v=onepage&q=are%20.hlsl%20files%20ever%20used%3F&f=false
//http://stackoverflow.com/questions/12549472/using-directx-effect11-with-visual-studio-2012

//Tutorial Resource for DX11:
//http://www.rastertek.com/tutindex.html
//https://msdn.microsoft.com/en-us/library/windows/desktop/ee663275.aspx

class RenderManager
{
private:
	HWND			mHWND;
	HINSTANCE		mHINSTANCE;
	TextureManager	gTexManager;

	

	void BuildGeometryBuffers();
public:
	RenderManager();
	~RenderManager();
	void SetWindow(HWND &mhWnd, HINSTANCE &mhInstance);
	void SetResolution(int w, int h);
	int  GetResolutionWidth()	{ return mWidth; }
	int  GetResolutionHeight()	{ return mHeight; }
	float AspectRatio() const;
	void OnResize();
	void Startup();
	void Update(float dt);
	void Render();
	void Shutdown();


	ID3D11Device*					GetD3DDevice() { return m_pD3DDevice; }
protected:
	ID3D11Device*					m_pD3DDevice;
	ID3D11DeviceContext*			m_pD3DImmediateContext;
	ID3D11Texture2D*				m_pDepthStencilBuffer;
	ID3D11RenderTargetView*			m_pRenderTargetView;
	ID3D11DepthStencilView*			m_pDepthStencilView;
	IDXGISwapChain*					m_pSwapChain;
	D3D11_VIEWPORT					m_ScreenViewport;
	D3D_DRIVER_TYPE					m_D3DDriverType;
	ID3D11Buffer*					m_pVB;		//vertex buffer
	ID3D11Buffer*					m_pIB;		//index buffer
	ID3DX11EffectMatrixVariable*	m_pFXWorldViewProj;
	ID3D11RasterizerState*			m_pRS;

	UINT						m4xMsaaQuality;
	int							mWidth;
	int							mHeight;
	bool						bEnable4xMsaa;

	DirectionalLight			mDirLights[3];
	Material					mGridMat;
	Material					mBoxMat;
	Material					mCylinderMat;
	Material					mSphereMat;
	Material					mCenterSphereMat;

	//temporary stuff until I get a camera class.
	//temporarily used for the basics of drawing shapes to the render target
	//define transformations from local spaces to world space.
	//these matrices represent the position/orientation of the objects to be drawn
	XMFLOAT4X4 mSphereWorld[10];
	XMFLOAT4X4 mCylWorld[10];
	XMFLOAT4X4 mBoxWorld;
	XMFLOAT4X4 mGridWorld;
	XMFLOAT4X4 mCenterSphereWorld;

	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

	int mBoxVertexOffset;
	int mGridVertexOffset;
	int mSphereVertexOffset;
	int mCylinderVertexOffset;

	UINT mBoxIndexOffset;
	UINT mGridIndexOffset;
	UINT mSphereIndexOffset;
	UINT mCylinderIndexOffset;

	UINT mBoxIndexCount;
	UINT mGridIndexCount;
	UINT mSphereIndexCount;
	UINT mCylinderIndexCount;

	UINT mLightCount;
	XMFLOAT3 mEyePosW;

	//float mTheta;
	//float mPhi;
	//float mRadius;
};

