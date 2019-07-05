#ifndef _TEXTURE_H_
#define _TEXTURE_H

#include <wrl/client.h>
#include <memory>
#include <d3d11.h>

#include "Graphics.h"


#pragma once
class Texture
{
public: 
	Texture();
	~Texture();

	bool Initialize(std::shared_ptr<Graphics> graphics, wchar_t* fileName);

	ID3D11ShaderResourceView* getTexture() const { return m_texture.Get(); };

private:

	bool LoadFromFile(wchar_t* fileName);

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_texture;

	std::shared_ptr<Graphics>							m_Graphics;
};

#endif