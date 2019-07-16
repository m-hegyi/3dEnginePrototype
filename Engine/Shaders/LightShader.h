#ifndef _LIGHT_SHADER_H_
#define _LIGHT_SHADER_H_

#include "BasicShader.h"

#pragma once
class LightShader : public BasicShader
{
public:
	LightShader();
	LightShader(std::shared_ptr<Graphics> graphics);
	~LightShader();

	struct LightBufferType
	{
		DirectX::SimpleMath::Vector4 ambientColor;
		DirectX::SimpleMath::Vector4 diffuseColor;
		DirectX::SimpleMath::Vector3 lightDirection;
		float padding; // need to be multiple of 16 for createbuffer
	};

	bool Initialize(HWND hwnd);

	bool Render(int vertexCount,
		int instanceCount,
		DirectX::SimpleMath::Matrix worldMatrix,
		DirectX::SimpleMath::Matrix viewMatrix,
		DirectX::SimpleMath::Matrix projectionMatrix,
		ID3D11ShaderResourceView* texture,
		DirectX::SimpleMath::Vector3 lightDirection,
		DirectX::SimpleMath::Vector4 diffuseColor,
		DirectX::SimpleMath::Vector4 ambientColor);

protected:

	bool InitializeShader(HWND hwnd, std::string vertexShaderFile, std::string pixelShaderFile);

	bool CreateBuffers();
	bool CreateLightBuffer();

private:
	bool SetShaderParameters(int vertexCount,
		int instanceCount,
		DirectX::SimpleMath::Matrix worldMatrix,
		DirectX::SimpleMath::Matrix viewMatrix,
		DirectX::SimpleMath::Matrix projectionMatrix,
		ID3D11ShaderResourceView* texture,
		DirectX::SimpleMath::Vector3 lightDirection,
		DirectX::SimpleMath::Vector4 diffuseColor,
		DirectX::SimpleMath::Vector4 ambientColor);

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_lightBuffer;
};

#endif
