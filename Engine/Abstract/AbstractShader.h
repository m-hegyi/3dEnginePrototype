#ifndef _ABSTRACT_SHADER_H_
#define _ABSTRACT_SHADER_H_

#include <d3d11.h>
#include <wrl\client.h>
#include <vector>
#include <map>
#include <SimpleMath.h>

#include "../Graphics.h"

#pragma once

class AbstractShader
{
public:

	struct ShaderFileType
	{
		char* data;
		size_t length;
	};

	virtual bool Initialize(HWND window) = 0;

protected:
	virtual bool CreateBuffers() = 0;
	virtual bool GetShaderFile(std::string fileName, ShaderFileType* shaderData) = 0;

protected:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			m_layout;
	std::shared_ptr<Graphics>							m_Graphics;
	/*Microsoft::WRL::ComPtr<ID3D11Buffer>		m_matrixBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_lightBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_fogBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_reflectionBuffer;*/
};

#endif
