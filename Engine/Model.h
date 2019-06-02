#ifndef _MODEL_H_
#define _MODEL_H_

#include <memory>
#include <vector>
#include <d3d11.h>
#include <wrl\client.h>
#include <SimpleMath.h>
#include "Texture.h"

#pragma once
class Model
{
private:
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

public:
	Model();
	~Model();

	bool Initialize(ID3D11Device* device, char* fileName, wchar_t* textureFileName);
	void Reset();

	bool Render(ID3D11DeviceContext* deviceContext);

	int getIndexCount() const { return m_indexCount; };
	ID3D11ShaderResourceView* getTexture() const { return m_Texture->getTexture(); };

private:
	bool InitializeBuffer(ID3D11Device* device);
	bool RenderBuffer(ID3D11DeviceContext* deviceContext);
	void ResetBuffer();

	bool LoadModel(char* fileName);
	void ResetModel();

	bool LoadTexture(ID3D11Device* device, wchar_t* textureFileName);
	void ResetTexture();

private:
	int m_vertexCount;
	int m_indexCount;

	std::unique_ptr<ModelType[]>			m_model;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_indexBuffer;

	std::unique_ptr<Texture>				m_Texture;
};


#endif