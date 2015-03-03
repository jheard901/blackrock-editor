//class for initializing the renderer

#include "render_manager.h"
#include "geometry_factory.h"
#include "math_ex.h"
#include "effects.h"
#include "vertex.h"

//NOTE: It is essential to update this function to reflect the Vertex structure being used in your .fx file
void RenderManager::BuildGeometryBuffers()
{
	GeometryFactory::MeshData box;
	GeometryFactory::MeshData grid;
	GeometryFactory::MeshData sphere;
	GeometryFactory::MeshData cylinder;

	GeometryFactory gFactory;
	gFactory.CreateBox(1.0f, 1.0f, 1.0f, box);
	gFactory.CreateGrid(20.0f, 30.0f, 60, 40, grid);
	gFactory.CreateSphere(0.5f, 20, 20, sphere);
	//gFactory.CreateGeosphere(0.5f, 2, sphere);
	gFactory.CreateCylinder(0.5f, 0.3f, 3.0f, 20, 20, cylinder);

	// Cache the vertex offsets to each object in the concatenated vertex buffer.
	mBoxVertexOffset = 0;
	mGridVertexOffset = box.Vertices.size();
	mSphereVertexOffset = mGridVertexOffset + grid.Vertices.size();
	mCylinderVertexOffset = mSphereVertexOffset + sphere.Vertices.size();

	// Cache the index count of each object.
	mBoxIndexCount = box.Indices.size();
	mGridIndexCount = grid.Indices.size();
	mSphereIndexCount = sphere.Indices.size();
	mCylinderIndexCount = cylinder.Indices.size();

	// Cache the starting index for each object in the concatenated index buffer.
	mBoxIndexOffset = 0;
	mGridIndexOffset = mBoxIndexCount;
	mSphereIndexOffset = mGridIndexOffset + mGridIndexCount;
	mCylinderIndexOffset = mSphereIndexOffset + mSphereIndexCount;

	UINT totalVertexCount =
		box.Vertices.size() +
		grid.Vertices.size() +
		sphere.Vertices.size() +
		cylinder.Vertices.size();

	UINT totalIndexCount =
		mBoxIndexCount +
		mGridIndexCount +
		mSphereIndexCount +
		mCylinderIndexCount;

	//
	// Extract the vertex elements we are interested in and pack the
	// vertices of all the meshes into one vertex buffer.
	//

	std::vector<Vertex::Basic32> vertices(totalVertexCount);

	XMFLOAT4 black(0.0f, 0.0f, 0.0f, 1.0f);

	UINT k = 0;
	for (size_t i = 0; i < box.Vertices.size(); i++, k++)
	{
		vertices[k].Pos = box.Vertices[i].Position;
		vertices[k].Normal = box.Vertices[i].Normal;
		//vertices[k].Tex = box.Vertices[i].TexC;
	}

	for (size_t i = 0; i < grid.Vertices.size(); i++, k++)
	{
		vertices[k].Pos = grid.Vertices[i].Position;
		vertices[k].Normal = grid.Vertices[i].Normal;
		vertices[k].Tex = grid.Vertices[i].TexC;
	}

	for (size_t i = 0; i < sphere.Vertices.size(); i++, k++)
	{
		vertices[k].Pos = sphere.Vertices[i].Position;
		vertices[k].Normal = sphere.Vertices[i].Normal;
		//vertices[k].Tex = sphere.Vertices[i].TexC;
	}

	for (size_t i = 0; i < cylinder.Vertices.size(); i++, k++)
	{
		vertices[k].Pos = cylinder.Vertices[i].Position;
		vertices[k].Normal = cylinder.Vertices[i].Normal;
		//vertices[k].Tex = cylinder.Vertices[i].TexC;
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::Basic32) * totalVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(m_pD3DDevice->CreateBuffer(&vbd, &vinitData, &m_pVB));

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	std::vector<UINT> indices;
	indices.insert(indices.end(), box.Indices.begin(), box.Indices.end());
	indices.insert(indices.end(), grid.Indices.begin(), grid.Indices.end());
	indices.insert(indices.end(), sphere.Indices.begin(), sphere.Indices.end());
	indices.insert(indices.end(), cylinder.Indices.begin(), cylinder.Indices.end());

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * totalIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_pD3DDevice->CreateBuffer(&ibd, &iinitData, &m_pIB));
}

RenderManager::RenderManager()
{

	m_D3DDriverType = D3D_DRIVER_TYPE_HARDWARE;
	bEnable4xMsaa = false;
	m_pRenderTargetView = 0;
	m_pDepthStencilView = 0;
	m_pDepthStencilBuffer = 0;
	ZeroMemory(&m_ScreenViewport, sizeof(D3D11_VIEWPORT));

	m_pVB = NULL;
	m_pIB = NULL;
	m_pFXWorldViewProj = NULL;
	m_pRS = NULL;
	//mTheta = 1.5f * MathEX::Pi;
	//mPhi = 0.1f * MathEX::Pi;
	//mRadius = 15.0f;

	//1 = front light only; 2 = front & side lights; 3 = front, side, and rear lights (the standard used in films and CG Animation)
	//RE: As of 2/7/15 this var does not impact the lights used anymore
	mLightCount = 3;

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mGridWorld, I);
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProj, I);

	XMMATRIX boxScale = XMMatrixScaling(2.0f, 1.0f, 2.0f);
	XMMATRIX boxOffset = XMMatrixTranslation(0.0f, 0.5f, 0.0f);
	XMStoreFloat4x4(&mBoxWorld, XMMatrixMultiply(boxScale, boxOffset));

	XMMATRIX centerSphereScale = XMMatrixScaling(2.0f, 2.0f, 2.0f);
	XMMATRIX centerSphereOffset = XMMatrixTranslation(0.0f, 2.0f, 0.0f);
	XMStoreFloat4x4(&mCenterSphereWorld, XMMatrixMultiply(centerSphereScale, centerSphereOffset));

	for (int i = 0; i < 5; i++)
	{
		XMStoreFloat4x4(&mCylWorld[i * 2 + 0], XMMatrixTranslation(-5.0f, 1.5f, -10.0f + i*5.0f));
		XMStoreFloat4x4(&mCylWorld[i * 2 + 1], XMMatrixTranslation(+5.0f, 1.5f, -10.0f + i*5.0f));

		XMStoreFloat4x4(&mSphereWorld[i * 2 + 0], XMMatrixTranslation(-5.0f, 3.5f, -10.0f + i*5.0f));
		XMStoreFloat4x4(&mSphereWorld[i * 2 + 1], XMMatrixTranslation(+5.0f, 3.5f, -10.0f + i*5.0f));
	}

	//add lighting details for all objects in scene
	mDirLights[0].Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[0].Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLights[0].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLights[0].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	mDirLights[1].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[1].Diffuse = XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f);
	mDirLights[1].Specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	mDirLights[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

	mDirLights[2].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Direction = XMFLOAT3(0.0f, -0.707f, -0.707f);

	mGridMat.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mGridMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mGridMat.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);

	mCylinderMat.Ambient = XMFLOAT4(0.7f, 0.85f, 0.7f, 1.0f);
	mCylinderMat.Diffuse = XMFLOAT4(0.7f, 0.85f, 0.7f, 1.0f);
	mCylinderMat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);

	mSphereMat.Ambient = XMFLOAT4(0.1f, 0.2f, 0.3f, 1.0f);
	mSphereMat.Diffuse = XMFLOAT4(0.2f, 0.4f, 0.6f, 1.0f);
	mSphereMat.Specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 16.0f);

	mBoxMat.Ambient = XMFLOAT4(0.651f, 0.5f, 0.392f, 1.0f);
	mBoxMat.Diffuse = XMFLOAT4(0.651f, 0.5f, 0.392f, 1.0f);
	mBoxMat.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);

	mCenterSphereMat.Ambient = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	mCenterSphereMat.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	mCenterSphereMat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);
}

RenderManager::~RenderManager()
{
	Shutdown();
}

//sets the window that will be rendered to; this needs to be set prior to Startup()
void RenderManager::SetWindow(HWND &mhWnd, HINSTANCE &mhInstance)
{
	mHWND = mhWnd;
	mHINSTANCE = mhInstance;
}

//this needs to be set prior to Startup()
void RenderManager::SetResolution(int w, int h)
{
	mWidth = w;
	mHeight = h;
}

float RenderManager::AspectRatio() const
{
	return static_cast<float>(mWidth) / mHeight;
}

void RenderManager::OnResize()
{
	assert(m_pD3DImmediateContext);
	assert(m_pD3DDevice);
	assert(m_pSwapChain);

	//release old views and buffers since they will be destroyed
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDepthStencilBuffer);

	//resize the swap chain and recreate the render target view
	HR(m_pSwapChain->ResizeBuffers(1, mWidth, mHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ID3D11Texture2D* backBuffer;
	HR(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	HR(m_pD3DDevice->CreateRenderTargetView(backBuffer, 0, &m_pRenderTargetView));
	SAFE_RELEASE(backBuffer);

	//create the depth/stencil buffer and view
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = mWidth;
	depthStencilDesc.Height = mHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//use 4X MSAA; must match swap chain MSAA values
	if (bEnable4xMsaa)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	//no MSAA
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HR(m_pD3DDevice->CreateTexture2D(&depthStencilDesc, 0, &m_pDepthStencilBuffer));
	HR(m_pD3DDevice->CreateDepthStencilView(m_pDepthStencilBuffer, 0, &m_pDepthStencilView));

	//bind the render target view and depth/stencil view to the pipeline
	m_pD3DImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	//set the viewport transform
	m_ScreenViewport.TopLeftX = 0;
	m_ScreenViewport.TopLeftY = 0;
	m_ScreenViewport.Width = static_cast<float>(mWidth);		//static_cast<float> converts the value to a float
	m_ScreenViewport.Height = static_cast<float>(mHeight);
	m_ScreenViewport.MinDepth = 0.0f;
	m_ScreenViewport.MaxDepth = 1.0f;

	m_pD3DImmediateContext->RSSetViewports(1, &m_ScreenViewport);

	//draws objects into perspective
	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*MathEX::Pi, AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);
}

void RenderManager::Startup()
{
	// Create the device and device context.
	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(
		NULL,						//specify display adapter (NULL = default/primary)
		m_D3DDriverType,			//specify driver type to use for rendering
		NULL,						//specify a software driver to use if m_D3DDriverType is software based 
		createDeviceFlags,			//optional device creation flags
		NULL,						//specify D3D feature level array; NULL uses the highest level supported which is D3D_FEATURE_LEVEL_11_0
		0,							//specify number of D3D feature levels
		D3D11_SDK_VERSION,			//specify SDK version to use
		&m_pD3DDevice,				//return the device
		&featureLevel,				//return the first supported feature level
		&m_pD3DImmediateContext);	//return the device context

	//check 4X MSAA quality support for our back buffer format
	HR(m_pD3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality));
	assert(m4xMsaaQuality > 0);

	//fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = mWidth;
	sd.BufferDesc.Height = mHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//use 4X MSAA
	if (bEnable4xMsaa)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	//no MSAA
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		//specify usage of buffer; in this case it is the rendering target
	sd.BufferCount = 1;										//specify # of back buffers to use; 2 is max for triple buffering
	sd.OutputWindow = mHWND;								//handle to window being rendered into
	sd.Windowed = true;										//run in windowed or fullscreen mode
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;				//specify presentation method
	sd.Flags = 0;											//optional flags

	//to correctly create a swap chain, use the IDXGIFactory that was used to create the device
	IDXGIDevice* dxgiDevice = 0;
	HR(m_pD3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdapter = 0;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory* dxgiFactory = 0;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	HR(dxgiFactory->CreateSwapChain(m_pD3DDevice, &sd, &m_pSwapChain));

	SAFE_RELEASE(dxgiDevice);
	SAFE_RELEASE(dxgiAdapter);
	SAFE_RELEASE(dxgiFactory);

	OnResize();		//this should be called once here and also each time the window is resized

	// Must init Effects first since InputLayouts depend on shader signatures.
	Effects::InitAll(m_pD3DDevice);
	InputLayouts::InitAll(m_pD3DDevice);

	//load textures
	gTexManager.Startup(m_pD3DDevice);

	//setup for drawing shapes
	BuildGeometryBuffers();

	D3D11_RASTERIZER_DESC rasDesc;
	ZeroMemory(&rasDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasDesc.FillMode = D3D11_FILL_SOLID;	//declare fill mode here (SOLID or WIREFRAME)
	rasDesc.CullMode = D3D11_CULL_BACK;
	rasDesc.FrontCounterClockwise = false;
	rasDesc.DepthClipEnable = true;

	HR(m_pD3DDevice->CreateRasterizerState(&rasDesc, &m_pRS));
}

void RenderManager::Update(float dt)
{
	//temporary code until I get to making the camera class
	float x, y, z;
	x = 2.0f; y = 8.0f; z = -20.f;

	// Build the view matrix.
	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX V = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView, V);

	//temporary until I figure what this is used for
	mEyePosW = XMFLOAT3(x, y, z);
}

void RenderManager::Render()
{
	assert(m_pD3DImmediateContext);
	assert(m_pSwapChain);

	m_pD3DImmediateContext->ClearRenderTargetView(m_pRenderTargetView, reinterpret_cast<const float*>(&Colors::LightSteelBlue));
	m_pD3DImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//setup here
	m_pD3DImmediateContext->IASetInputLayout(InputLayouts::Basic32);
	m_pD3DImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pD3DImmediateContext->RSSetState(m_pRS);

	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	// Set constants

	XMMATRIX view = XMLoadFloat4x4(&mView);
	XMMATRIX proj = XMLoadFloat4x4(&mProj);
	XMMATRIX viewProj = view*proj;

	// Set per frame constants.
	Effects::BasicFX->SetDirLights(mDirLights);
	Effects::BasicFX->SetEyePosW(mEyePosW);

	// Figure out which technique to use.
	//ID3DX11EffectTechnique* activeTech = Effects::BasicFX->Light1Tech;
	//
	//switch (mLightCount)
	//{
	//case 1:
	//	activeTech = Effects::BasicFX->Light1Tech;
	//	break;
	//case 2:
	//	activeTech = Effects::BasicFX->Light2Tech;
	//	break;
	//case 3:
	//	activeTech = Effects::BasicFX->Light3Tech;
	//	break;
	//}

	ID3DX11EffectTechnique* activeTech = Effects::BasicFX->Light3TexTech;

	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; p++)
	{
		m_pD3DImmediateContext->IASetVertexBuffers(0, 1, &m_pVB, &stride, &offset);
		m_pD3DImmediateContext->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);

		// Draw the grid.
		XMMATRIX world = XMLoadFloat4x4(&mGridWorld);
		XMMATRIX worldInvTranspose = MathEX::InverseTranspose(world);
		XMMATRIX worldViewProj = world*view*proj;

		Effects::BasicFX->SetWorld(world);
		Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicFX->SetWorldViewProj(worldViewProj);
		Effects::BasicFX->SetTexTransform(XMLoadFloat4x4(gTexManager.RetGridTex()));
		Effects::BasicFX->SetMaterial(mGridMat);
		Effects::BasicFX->SetDiffuseMap(gTexManager.RetGridSRV());

		activeTech->GetPassByIndex(p)->Apply(0, m_pD3DImmediateContext);
		m_pD3DImmediateContext->DrawIndexed(mGridIndexCount, mGridIndexOffset, mGridVertexOffset);

		// Draw the box.
		world = XMLoadFloat4x4(&mBoxWorld);
		worldInvTranspose = MathEX::InverseTranspose(world);
		worldViewProj = world*view*proj;

		Effects::BasicFX->SetWorld(world);
		Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicFX->SetWorldViewProj(worldViewProj);
		Effects::BasicFX->SetMaterial(mBoxMat);

		activeTech->GetPassByIndex(p)->Apply(0, m_pD3DImmediateContext);
		m_pD3DImmediateContext->DrawIndexed(mBoxIndexCount, mBoxIndexOffset, mBoxVertexOffset);

		// Draw center sphere.
		world = XMLoadFloat4x4(&mCenterSphereWorld);
		worldInvTranspose = MathEX::InverseTranspose(world);
		worldViewProj = world*view*proj;

		Effects::BasicFX->SetWorld(world);
		Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicFX->SetWorldViewProj(worldViewProj);
		Effects::BasicFX->SetMaterial(mCenterSphereMat);

		activeTech->GetPassByIndex(p)->Apply(0, m_pD3DImmediateContext);
		m_pD3DImmediateContext->DrawIndexed(mSphereIndexCount, mSphereIndexOffset, mSphereVertexOffset);

		// Draw the cylinders.
		for (int i = 0; i < 10; ++i)
		{
			world = XMLoadFloat4x4(&mCylWorld[i]);
			worldInvTranspose = MathEX::InverseTranspose(world);
			worldViewProj = world*view*proj;

			Effects::BasicFX->SetWorld(world);
			Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::BasicFX->SetWorldViewProj(worldViewProj);
			Effects::BasicFX->SetMaterial(mCylinderMat);

			activeTech->GetPassByIndex(p)->Apply(0, m_pD3DImmediateContext);
			m_pD3DImmediateContext->DrawIndexed(mCylinderIndexCount, mCylinderIndexOffset, mCylinderVertexOffset);
		}

		// Draw the spheres.
		for (int i = 0; i < 10; ++i)
		{
			world = XMLoadFloat4x4(&mSphereWorld[i]);
			worldInvTranspose = MathEX::InverseTranspose(world);
			worldViewProj = world*view*proj;

			Effects::BasicFX->SetWorld(world);
			Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::BasicFX->SetWorldViewProj(worldViewProj);
			Effects::BasicFX->SetMaterial(mSphereMat);

			activeTech->GetPassByIndex(p)->Apply(0, m_pD3DImmediateContext);
			m_pD3DImmediateContext->DrawIndexed(mSphereIndexCount, mSphereIndexOffset, mSphereVertexOffset);
		}
	}


	HR(m_pSwapChain->Present(0, 0));
}

void RenderManager::Shutdown()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
	gTexManager.Shutdown();
	Effects::DestroyAll();
	InputLayouts::DestroyAll();
	SAFE_RELEASE(m_pRS);

	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDepthStencilBuffer);

	//restore all default settings.
	if (m_pD3DImmediateContext)
		m_pD3DImmediateContext->ClearState();

	SAFE_RELEASE(m_pD3DImmediateContext);
	SAFE_RELEASE(m_pD3DDevice);
}


