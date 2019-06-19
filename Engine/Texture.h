#ifndef _TEXTURE_H_
#define _TEXTURE_H

#include <wrl/client.h>
#include <d3d11.h>


#pragma once
class Texture
{
public: 
	Texture();
	~Texture();

	bool Initialize(ID3D11Device* device, wchar_t* fileName);

	ID3D11ShaderResourceView* getTexture() const { return m_texture.Get(); };

private:

	bool LoadFromFile(ID3D11Device* device, wchar_t* fileName);

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
};

#endif