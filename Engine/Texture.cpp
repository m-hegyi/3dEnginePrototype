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

bool Texture::Initialize(std::shared_ptr<Graphics> graphics, wchar_t* fileName)
{
	m_Graphics = graphics;
	if (!LoadFromFile(fileName)) {
		return false;
	}

	return true;
}

bool Texture::LoadFromFile(wchar_t* fileName)
{
	auto device = m_Graphics->getRenderer()->getDevice();

	DX::ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, fileName, nullptr, m_texture.ReleaseAndGetAddressOf()));
	//DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device, fileName, nullptr, m_texture.ReleaseAndGetAddressOf()));
	return true;
}
