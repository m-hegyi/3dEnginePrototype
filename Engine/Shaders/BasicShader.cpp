#include "BasicShader.h"
#include <fstream>

BasicShader::BasicShader()
{

};

BasicShader::BasicShader(std::shared_ptr<Graphics> graphics)
{
	m_Graphics = graphics;
};

BasicShader::~BasicShader()
{
};

bool BasicShader::Initialize(HWND hwnd)
{
	if (!InitializeShader(hwnd, "BaseVertexTexture.cso", "BasePixelTexture.cso"))
	{
		return false;
	}

	return true;
}

bool BasicShader::Render(int vertexCount, int instanceCount,
	DirectX::SimpleMath::Matrix worldMatrix, DirectX::SimpleMath::Matrix viewMatrix, DirectX::SimpleMath::Matrix projectionMatrix, 
	ID3D11ShaderResourceView * texture)
{

	if (!SetShaderParameters(vertexCount, instanceCount, worldMatrix, viewMatrix, projectionMatrix, texture))
	{
		return false;
	}

	RenderShader(vertexCount, instanceCount);

	return true;
}

bool BasicShader::InitializeShader(HWND hwnd, std::string vertexShaderFile, std::string pixelShaderFile)
{
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D10Blob> errorMessage;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;

	// Initialize the pointers this function will use to null.
	errorMessage.Reset();
	auto vertexShader = std::make_unique<ShaderFileType>();
	auto pixelShader = std::make_unique<ShaderFileType>();

	if (!GetShaderFile(vertexShaderFile, vertexShader.get()))
	{
		return false;
	}

	auto device = m_Graphics->getRenderer()->getDevice();

	result = device->CreateVertexShader(vertexShader->data, vertexShader->length, NULL, m_vertexShader.ReleaseAndGetAddressOf());
	if (FAILED(result)) {
		return false;
	}

	if (!GetShaderFile(pixelShaderFile, pixelShader.get())) {
		return false;
	}

	result = device->CreatePixelShader(pixelShader->data, pixelShader->length, NULL, m_pixelShader.ReleaseAndGetAddressOf());
	if (FAILED(result)) {
		return false;
	}

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShader->data,
		vertexShader->length, m_layout.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	if (!CreateBuffers())
	{
		return false;
	}

	if (!CreateSamplerState())
	{
		return false;
	}
	

	delete vertexShader->data;
	delete pixelShader->data;

	return true;
}

void BasicShader::ResetShader()
{
	m_vertexShader->Release();
	m_vertexShader.Reset();
	m_pixelShader->Release();
	m_pixelShader.Reset();
	m_layout->Release();
	m_layout.Reset();
	m_samplerState->Release();
	m_samplerState.Reset();
}

void BasicShader::RenderShader(int vertexCount, int instanceCount)
{
	auto deviceContext = m_Graphics->getRenderer()->getContext();

	//Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout.Get());

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(m_vertexShader.Get(), NULL, 0);
	deviceContext->PSSetShader(m_pixelShader.Get(), NULL, 0);

	// Set the sampler state 
	//
	// TODO: switchable
	//
	deviceContext->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());

	// Render
	deviceContext->DrawInstanced(vertexCount, instanceCount, 0, 0);
	//deviceContext->DrawIndexed(indexCount, 0, 0);

	//
	// TODO: properly count the rendered instance
	//
	m_Graphics->UpdateStat(instanceCount);
}

bool BasicShader::CreateBuffers()
{
	if (!CreateMatrixBuffer())
	{
		return false;
	}

	return true;
}

bool BasicShader::CreateSamplerState()
{
	D3D11_SAMPLER_DESC samplerDesc;
	HRESULT result;

	// Sampler Description
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = m_Graphics->getRenderer()->getDevice()->CreateSamplerState(&samplerDesc, m_samplerState.ReleaseAndGetAddressOf());
	if (FAILED(result)) {
		return false;
	}

	return true;
}

bool BasicShader::CreateMatrixBuffer()
{
	HRESULT result;
	D3D11_BUFFER_DESC matrixBufferDesc;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = m_Graphics->getRenderer()->getDevice()->CreateBuffer(&matrixBufferDesc, NULL, m_matrixBuffer.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool BasicShader::SetShaderParameters(int vertexCount, int instanceCount,
	DirectX::SimpleMath::Matrix worldMatrix, DirectX::SimpleMath::Matrix viewMatrix, DirectX::SimpleMath::Matrix projectionMatrix, 
	ID3D11ShaderResourceView * texture)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	// Transpose the matrices to prepare them for the shader.
	worldMatrix = worldMatrix.Transpose();
	viewMatrix = viewMatrix.Transpose();
	projectionMatrix = projectionMatrix.Transpose();

	auto deviceContext = m_Graphics->getRenderer()->getContext();

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer.Get(), 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, m_matrixBuffer.GetAddressOf());

	// Set shader texture resource in the pixel shader
	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}


bool BasicShader::GetShaderFile(std::string fileName, ShaderFileType * shaderData)
{
	int length;

	std::ifstream file(fileName.c_str(), std::ios::binary);

	if (file.bad()) {
		file.close();
		return false;
	}

	file.seekg(0, std::ios::end);
	length = size_t(file.tellg());

	auto buffer = new char[length];

	file.seekg(0, std::ios::beg);
	file.read(buffer, length);

	shaderData->data = buffer;
	shaderData->length = length;

	file.close();
	return true;
}