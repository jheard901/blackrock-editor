//class for initializing the Textureer

#include "texture_manager.h"



TextureManager::TextureManager()
{
	//grid tex
	mGridMapSRV = NULL;
}

TextureManager::~TextureManager()
{
	Shutdown();
}

void TextureManager::Startup(ID3D11Device* &device)
{
	//grid tex
	//XMMATRIX gridTexScale = XMMatrixScaling(5.0f, 5.0f, 0.0f);	//a bigger scale makes tex appear smaller
	XMMATRIX gridTexScale = XMMatrixScaling(2.0f, 2.0f, 0.0f);
	XMStoreFloat4x4(&mGridTexTransform, gridTexScale);

	HR(D3DX11CreateShaderResourceViewFromFile(device,
		L"Textures/grid_alt.bmp", 0, 0, &mGridMapSRV, 0));
}

void TextureManager::Shutdown()
{
	SAFE_RELEASE(mGridMapSRV);
}

XMFLOAT4X4* TextureManager::RetGridTex()
{
	return &mGridTexTransform;
}

ID3D11ShaderResourceView* TextureManager::RetGridSRV()
{
	return mGridMapSRV;
}
