#ifndef _SHADER_H_
#define _SHADER_H_

#include <memory>
#include <string>
#include <wrl\client.h>
#include <d3d11.h>
#include "SimpleMath.h"
#include "Graphics.h"

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

	struct FogBufferType
	{
		float fogStart;
		float fogEnd;
		float padding1, padding2;
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

	bool Initialize(HWND hwnd, std::shared_ptr<Graphics> graphics);
	bool Render(int indexCount,
		DirectX::SimpleMath::Matrix worldMatrix, 
		DirectX::SimpleMath::Matrix viewMatrix, 
		DirectX::SimpleMath::Matrix projectionMatrix,
		ID3D11ShaderResourceView* texture,
		DirectX::SimpleMath::Vector3 lightDirection,
		DirectX::SimpleMath::Vector4 diffuseColor,
		DirectX::SimpleMath::Vector4 ambientColor);

	void Reset();

private:
	bool InitializeShader(HWND hwnd, std::string vertexShaderFile, std::string pixelShaderFile);
	void ResetShader();

	bool SetShaderParameters(DirectX::SimpleMath::Matrix worldMatrix, 
		DirectX::SimpleMath::Matrix viewMatrix, 
		DirectX::SimpleMath::Matrix projectionMatrix,
		ID3D11ShaderResourceView* texture,
		DirectX::SimpleMath::Vector3 lightDirection,
		DirectX::SimpleMath::Vector4 diffuseColor,
		DirectX::SimpleMath::Vector4 ambientColor,
		float fogStart,
		float fogEnd);

	void RenderShader(int indexCount);

	bool GetShaderFile(std::string fileName, ShaderFileType* shaderData);

private:
	// vertexSHader, pixelSHader, layout?, matrixBuffer
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_layout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_matrixBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_lightBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_fogBuffer;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>	m_sampleState;

	std::shared_ptr<Graphics>					m_Graphics;
};

#endif 