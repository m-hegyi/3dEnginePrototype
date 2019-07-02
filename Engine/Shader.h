#ifndef _SHADER_H_
#define _SHADER_H_

#include <memory>
#include <string>
#include <wrl\client.h>
#include <d3d11.h>
#include "SimpleMath.h"

#pragma once
class Shader
{
public:
	struct MatrixBufferType
	{
		DirectX::SimpleMath::Matrix world;
		DirectX::SimpleMath::Matrix view;
		DirectX::SimpleMath::Matrix projection;
	};

	struct ShaderFileType
	{
		char* data;
		size_t length;
	};

	struct LightBufferType {
		DirectX::SimpleMath::Vector4 ambientColor;
		DirectX::SimpleMath::Vector4 diffuseColor;
		DirectX::SimpleMath::Vector3 lightDirection;
		float padding; // need to be multiple of 16 for createbuffer
	};

	Shader();
	~Shader();

	bool Initialize(ID3D11Device* device, HWND hwnd);
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount,
		DirectX::SimpleMath::Matrix worldMatrix, 
		DirectX::SimpleMath::Matrix viewMatrix, 
		DirectX::SimpleMath::Matrix projectionMatrix,
		ID3D11ShaderResourceView* texture,
		DirectX::SimpleMath::Vector3 lightDirection,
		DirectX::SimpleMath::Vector4 diffuseColor,
		DirectX::SimpleMath::Vector4 ambientColor);

	void Reset();

private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, std::string vertexShaderFile, std::string pixelShaderFile);
	void ResetShader();

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext,
		DirectX::SimpleMath::Matrix worldMatrix, 
		DirectX::SimpleMath::Matrix viewMatrix, 
		DirectX::SimpleMath::Matrix projectionMatrix,
		ID3D11ShaderResourceView* texture,
		DirectX::SimpleMath::Vector3 lightDirection,
		DirectX::SimpleMath::Vector4 diffuseColor,
		DirectX::SimpleMath::Vector4 ambientColor);

	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

	bool GetShaderFile(std::string fileName, ShaderFileType* shaderData);

private:
	// vertexSHader, pixelSHader, layout?, matrixBuffer
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_layout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_matrixBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_lightBuffer;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>	m_sampleState;
};

#endif 