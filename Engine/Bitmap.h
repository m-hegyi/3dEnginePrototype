#ifndef _BITMAP_H_
#define _BITMAP_H_


#include <d3d11.h>
#include <SimpleMath.h>

#include "Graphics.h"
#include "Texture.h"

#pragma once
class Bitmap
{
private:
	struct VertexType
	{
		DirectX::SimpleMath::Vector3 position;
		DirectX::SimpleMath::Vector2 texture;
	};

public:
	Bitmap();
	~Bitmap();

	bool Initialize(std::shared_ptr<Graphics> graphics, WCHAR* textureFileName, int bitmapWidth, int bitmapHeight);

	bool Render();

	bool Update(int positionX, int positionY);

	int GetIndexCount() const { return m_indexCount; };
	ID3D11ShaderResourceView* GetTexture() const { return m_Texture->getTexture(); };
	DirectX::SimpleMath::Vector2 GetPosition() const { return DirectX::SimpleMath::Vector2(m_positionX, m_positionY); };

	DirectX::SimpleMath::Matrix GetWorldMatrix() const { return m_worldMatrix; };

private:
	bool InitializeBuffers();

	bool RenderBuffers();
	bool UpdateBuffers(int positionX, int positionY);

	bool LoadTexture(WCHAR* textureFileName);

private:
	int m_positionX, m_positionY, m_indexCount, m_vertexCount, 
		m_bitmapWidth, m_bitmapHeight, 
		m_previousPosX, m_previousPosY, 
		m_previousScreenWidth, m_previousScreenHeight;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_indexBuffer;

	std::shared_ptr<Graphics>				m_Graphics;
	std::unique_ptr<Texture>				m_Texture;

	DirectX::SimpleMath::Matrix				m_worldMatrix;
};

#endif 