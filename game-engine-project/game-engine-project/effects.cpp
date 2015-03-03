

#include "effects.h"


Effect::Effect(ID3D11Device* device, const std::wstring& filename)
	: m_pFX(0)
{
	std::ifstream fin(filename, std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size,
		0, device, &m_pFX));
}

Effect::~Effect()
{
	SAFE_RELEASE(m_pFX);
}


//child of Effect class
BasicEffect::BasicEffect(ID3D11Device* device, const std::wstring& filename)
	: Effect(device, filename)
{
	Light1Tech = m_pFX->GetTechniqueByName("Light1");
	Light2Tech = m_pFX->GetTechniqueByName("Light2");
	Light3Tech = m_pFX->GetTechniqueByName("Light3");

	Light0TexTech = m_pFX->GetTechniqueByName("Light0Tex");
	Light1TexTech = m_pFX->GetTechniqueByName("Light1Tex");
	Light2TexTech = m_pFX->GetTechniqueByName("Light2Tex");
	Light3TexTech = m_pFX->GetTechniqueByName("Light3Tex");

	WorldViewProj		= m_pFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World				= m_pFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose	= m_pFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	TexTransform		= m_pFX->GetVariableByName("gTexTransform")->AsMatrix();
	EyePosW				= m_pFX->GetVariableByName("gEyePosW")->AsVector();
	DirLights			= m_pFX->GetVariableByName("gDirLights");
	Mat					= m_pFX->GetVariableByName("gMaterial");
	DiffuseMap			= m_pFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
}

BasicEffect::~BasicEffect()
{
}

#pragma region Effects

BasicEffect* Effects::BasicFX = 0;

void Effects::InitAll(ID3D11Device* device)
{
	BasicFX = new BasicEffect(device, L"FX/basic.fxo");
}

void Effects::DestroyAll()
{
	SAFE_DELETE(BasicFX);
}
#pragma endregion