#pragma once

#include "render_util.h"
//#include "d3dx11Effect.h"
//#include <xnamath.h>

class TextureManager
{
private:
	//grid
	XMFLOAT4X4					mGridTexTransform;
	ID3D11ShaderResourceView*	mGridMapSRV;

public:
	TextureManager();
	~TextureManager();
	void Startup(ID3D11Device* &device);
	void Shutdown();

	//grid
	XMFLOAT4X4*					RetGridTex();
	ID3D11ShaderResourceView*	RetGridSRV();
};