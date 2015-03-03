
#pragma once

#include "vertex.h"
#include "effects.h"

//pragma region compartmentalizes code so it can be opened/closed like how VS2013 automatically does with functions (i.e. this is the manual way of doing it)
#pragma region InputLayoutDesc

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::Basic32[3] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

ID3D11InputLayout* InputLayouts::Basic32 = 0;

#pragma endregion

void InputLayouts::InitAll(ID3D11Device* device)
{
	D3DX11_PASS_DESC passDesc;
	//NOTE: you can only init an effect once to an InputLayout; repeat inits of an effect result in errors.
	//		Repeat usage of the passDesc variable is acceptable for creating multiple input layouts.


	//
	// Basic32
	//

	Effects::BasicFX->Light1Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(device->CreateInputLayout(InputLayoutDesc::Basic32, 3, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &Basic32));
}

void InputLayouts::DestroyAll()
{
	SAFE_RELEASE(Basic32);
}