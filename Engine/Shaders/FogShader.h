#ifndef _FOG_SHADER_H_
#define _FOG_SHADER_H_

#include "LightShader.h"

#pragma once
class FogShader : public LightShader
{
public:
	FogShader();
	FogShader(std::shared_ptr<Graphics> graphics);
	~FogShader();

	struct FogBufferType
	{
		float fogStart;
		float fogEnd;
		float padding1, padding2;
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
		float fogStart,
		float fogEnd);

protected:
	bool InitializeShader(HWND hwnd, std::string vertexShaderFile, std::string pixelShaderFile);

	bool CreateBuffers();
	bool CreateFogBuffer();

private:
	bool SetShaderParameters(int vertexCount,
		int instanceCount,
		DirectX::SimpleMath::Matrix worldMatrix,
		DirectX::SimpleMath::Matrix viewMatrix,
		DirectX::SimpleMath::Matrix projectionMatrix,
		ID3D11ShaderResourceView* texture,
		DirectX::SimpleMath::Vector3 lightDirection,
		DirectX::SimpleMath::Vector4 diffuseColor,
		DirectX::SimpleMath::Vector4 ambientColor,
		float fogStart,
		float fogEnd);

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_fogBuffer;

};

#endif
