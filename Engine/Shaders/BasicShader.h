#ifndef _BASE_SHADER_H_
#define _BASE_SHADER_H_

#include "../Abstract/AbstractShader.h"

#pragma once
class BasicShader : public AbstractShader
{
public:
	BasicShader();
	BasicShader(std::shared_ptr<Graphics> graphics);
	~BasicShader();

	struct MatrixBufferType
	{
		DirectX::SimpleMath::Matrix world;
		DirectX::SimpleMath::Matrix view;
		DirectX::SimpleMath::Matrix projection;
	};

	bool Initialize(HWND hwnd);

	bool Render(int vertexCount,
		int instanceCount,
		DirectX::SimpleMath::Matrix worldMatrix,
		DirectX::SimpleMath::Matrix viewMatrix,
		DirectX::SimpleMath::Matrix projectionMatrix,
		ID3D11ShaderResourceView* texture);

protected:
	bool InitializeShader(HWND hwnd, std::string vertexShaderFile, std::string pixelShaderFile);
	void ResetShader();

	void RenderShader(int vertexCount, int instanceCount);

	bool GetShaderFile(std::string fileName, ShaderFileType* shaderData) final;

	bool CreateBuffers();
	bool CreateSamplerState();
	bool CreateMatrixBuffer();

private:
	bool SetShaderParameters(int vertexCount,
		int instanceCount,
		DirectX::SimpleMath::Matrix worldMatrix,
		DirectX::SimpleMath::Matrix viewMatrix,
		DirectX::SimpleMath::Matrix projectionMatrix,
		ID3D11ShaderResourceView* texture);

protected:
	int m_vertexCount;

	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_matrixBuffer;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>	m_samplerState;
	//Microsoft::WRL::ComPtr<ID3D11Buffer>		m_lightBuffer;
	//Microsoft::WRL::ComPtr<ID3D11Buffer>		m_fogBuffer;
	//Microsoft::WRL::ComPtr<ID3D11Buffer>		m_reflectionBuffer;
};


#endif 
