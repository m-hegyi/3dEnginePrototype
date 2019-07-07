#ifndef _DEBUG_WINDOW_H_
#define _DEBUG_WINDOW_H_

#include <d3d11.h>
#include <SimpleMath.h>

#include "Graphics.h"

#pragma once
class DebugWindow
{
private:
	struct VertexType
	{
		DirectX::SimpleMath::Vector3 position;
		DirectX::SimpleMath::Vector2 texture;
	};

public:
	DebugWindow();
	~DebugWindow();

	bool Initialize(std::shared_ptr<Graphics> graphics, int bitmapWidth, int bitmapHeight);

	bool Render();

	bool Update(int positionX, int positionY);

	int GetIndexCount() const { return m_indexCount; };
	DirectX::SimpleMath::Vector2 GetPosition() const { return DirectX::SimpleMath::Vector2(m_positionX, m_positionY); };

	DirectX::SimpleMath::Matrix GetWorldMatrix() const { return m_worldMatrix; };

private:
	bool InitializeBuffers();

	bool RenderBuffers();
	bool UpdateBuffers(int positionX, int positionY);

private:
	int m_positionX, m_positionY, m_indexCount, m_vertexCount,
		m_bitmapWidth, m_bitmapHeight,
		m_previousPosX, m_previousPosY,
		m_previousScreenWidth, m_previousScreenHeight;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_indexBuffer;

	std::shared_ptr<Graphics>				m_Graphics;

	DirectX::SimpleMath::Matrix				m_worldMatrix;
};

#endif
