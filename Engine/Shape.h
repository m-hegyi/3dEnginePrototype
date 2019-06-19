#ifndef _SHAPE_H_
#define _SHAPE_H_

#include <memory>
#include <d3d11.h>
#include <wrl\client.h>
#include <SimpleMath.h>

#pragma once
class Shape
{
public:
	Shape();
	~Shape();

	struct VertexType
	{
		DirectX::SimpleMath::Vector3 position;
		DirectX::SimpleMath::Vector4 color;
	};

	struct VertexTextureType
	{
		DirectX::SimpleMath::Vector3 position;
		DirectX::SimpleMath::Vector2 texture;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	bool Initialize(ID3D11Device* device, char* fileName);

	bool Render(ID3D11DeviceContext* deviceContext);

private:
	bool LoadModel(char* fileName);
	bool InitializeBuffer(ID3D11Device* device);
	bool RenderBuffer(ID3D11DeviceContext* deviceContext);

private:
	int m_vertexCount;
	int m_indexCount;


	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_indexBuffer;

	std::unique_ptr<ModelType[]> m_model;
};

#endif // !_SHAPE_H_
