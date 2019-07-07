#ifndef _RENDER_TEXTURE_H_
#define _RENDER_TEXTURE_H_

#include <d3d11.h>
#include "../pch.h"

#include "Graphics.h"

#pragma once
class RenderTexture 
{
public:
	RenderTexture(std::shared_ptr<Graphics> graphics);
	~RenderTexture();

	bool Initialize(int textureWidth, int textureHeight);

	void SetRenderTarget();
	void ClearRenderTarget(float red, float green, float blue, float alpha);
	ID3D11ShaderResourceView* GetShaderResourceView() const { return m_shaderResourceView; };

private:
	ID3D11Texture2D*			m_renderTargetTexture;
	ID3D11RenderTargetView*		m_renderTargetView;
	ID3D11ShaderResourceView*	m_shaderResourceView;

	std::shared_ptr<Graphics>	m_Graphics;
};

#endif
