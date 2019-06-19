#include "../pch.h"

#include "Texture.h"
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

Texture::Texture()
{
}

Texture::~Texture()
{
	m_texture.Reset();
}

bool Texture::Initialize(ID3D11Device* device, wchar_t* fileName)
{
	if (!LoadFromFile(device, fileName)) {
		return false;
	}

	return true;
}

bool Texture::LoadFromFile(ID3D11Device* device, wchar_t* fileName)
{
	DX::ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, fileName, nullptr, m_texture.ReleaseAndGetAddressOf()));
	//DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device, fileName, nullptr, m_texture.ReleaseAndGetAddressOf()));
	return true;
}
