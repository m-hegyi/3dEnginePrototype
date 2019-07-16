#ifndef _REFLECTION_SHADER_H_
#define _REFLECTION_SHADER_H_

#include "LightShader.h"

#pragma once
class ReflectionShader : public LightShader
{
public:
	ReflectionShader();
	ReflectionShader(std::shared_ptr<Graphics> graphics);
	~ReflectionShader();

	struct ReflectionBufferType
	{
		DirectX::SimpleMath::Matrix reflectionMatrix;
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
		DirectX::SimpleMath::Vector4 ambientColor,
		ID3D11ShaderResourceView* reflectionTexture,
		DirectX::SimpleMath::Matrix reflectionMatrix);

protected:

	bool InitializeShader(HWND, std::string vertexShaderFile, std::string pixelShaderFile);

	bool CreateBuffers();
	bool CreateReflectionBuffer();

private:
	bool SetShaderParameters(DirectX::SimpleMath::Matrix worldMatrix,
		DirectX::SimpleMath::Matrix viewMatrix,
		DirectX::SimpleMath::Matrix projectionMatrix,
		ID3D11ShaderResourceView* texture,
		DirectX::SimpleMath::Vector3 lightDirection,
		DirectX::SimpleMath::Vector4 diffuseColor,
		DirectX::SimpleMath::Vector4 ambientColor,
		ID3D11ShaderResourceView* reflectionTexture,
		DirectX::SimpleMath::Matrix reflectionMatrix);

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_reflectionBuffer;
};


#endif
