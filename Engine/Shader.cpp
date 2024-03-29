#include "Shader.h"
#include <fstream>

using namespace DirectX;

Shader::Shader()
{
}

Shader::~Shader()
{
}

bool Shader::Initialize(HWND hwnd, std::shared_ptr<Graphics> graphics, std::string vertexShaderFile, std::string pixelShaderFile)
{
	m_Graphics = graphics;

	if (!InitializeShader(hwnd, vertexShaderFile, pixelShaderFile)) {
		return false;
	}

	return true;
}

bool Shader::InitializeShader(HWND hwnd, std::string vertexShaderFile, std::string pixelShaderFile)
{
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D10Blob> errorMessage;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[4];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC fogBufferDesc;
	D3D11_BUFFER_DESC reflectionBufferDesc;

	// Initialize the pointers this function will use to null.
	errorMessage.Reset();
	auto vertexShader = std::make_unique<ShaderFileType>();
	auto pixelShader = std::make_unique<ShaderFileType>();

	if (!GetShaderFile(vertexShaderFile, vertexShader.get())) {
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

	// Now setup the layout of the data that goes into the shader.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	
	/*
	//
	// TODO: changeable vertexshader type
	//
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;
	*/
	
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

	polygonLayout[2].SemanticName = "TEXCOORD";
	polygonLayout[2].SemanticIndex = 1;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 1;
	polygonLayout[2].AlignedByteOffset = 0;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygonLayout[2].InstanceDataStepRate = 1;

	polygonLayout[3].SemanticName = "NORMAL";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;


	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShader->data,
		vertexShader->length, &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, m_matrixBuffer.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

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

	result = device->CreateSamplerState(&samplerDesc, m_sampleState.ReleaseAndGetAddressOf());
	if (FAILED(result)) {
		return false;
	}

	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&lightBufferDesc, NULL, m_lightBuffer.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the dynamic fog constant buffer that is in the vertex shader.
	fogBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	fogBufferDesc.ByteWidth = sizeof(FogBufferType);
	fogBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	fogBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	fogBufferDesc.MiscFlags = 0;
	fogBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&fogBufferDesc, NULL, m_fogBuffer.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the reflection dynamic constant buffer that is in the vertex shader.
	reflectionBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	reflectionBufferDesc.ByteWidth = sizeof(ReflectionBufferType);
	reflectionBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	reflectionBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	reflectionBufferDesc.MiscFlags = 0;
	reflectionBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&reflectionBufferDesc, NULL, m_reflectionBuffer.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	delete vertexShader->data;
	delete pixelShader->data;

	return true;
}

bool Shader::Render(int vertexCount, int instanceCount, 
	SimpleMath::Matrix worldMatrix, SimpleMath::Matrix viewMatrix, SimpleMath::Matrix projectionMatrix,
	ID3D11ShaderResourceView* texture, 
	SimpleMath::Vector3 lightDirection, SimpleMath::Vector4 diffuseColor, DirectX::SimpleMath::Vector4 ambientColor,
	ID3D11ShaderResourceView* reflectionTexture, SimpleMath::Matrix reflectionMatrix)
{

	if (!SetShaderParameters(worldMatrix, viewMatrix, projectionMatrix, texture, 
		lightDirection, diffuseColor, ambientColor, 0.0f, 25.0f, reflectionTexture, reflectionMatrix)) {
		return false;
	}

	RenderShader(vertexCount, instanceCount);

	return true;
}

void Shader::RenderShader(int vertexCount, int instanceCount)
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
	deviceContext->PSSetSamplers(0, 1, m_sampleState.GetAddressOf());

	// Render
	deviceContext->DrawInstanced(vertexCount, instanceCount, 0, 0);
	//deviceContext->DrawIndexed(indexCount, 0, 0);

	m_Graphics->UpdateStat(1);
}

bool Shader::SetShaderParameters(SimpleMath::Matrix worldMatrix, SimpleMath::Matrix viewMatrix, SimpleMath::Matrix projectionMatrix,
	ID3D11ShaderResourceView* texture, SimpleMath::Vector3 lightDirection, 
	SimpleMath::Vector4 diffuseColor, DirectX::SimpleMath::Vector4 ambientColor,
	float fogStart, float fogEnd,
	ID3D11ShaderResourceView* reflectionTexture, SimpleMath::Matrix reflectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightBufferType* dataPtr2;
	FogBufferType* dataPtr3;
	ReflectionBufferType* dataPtr4;
	unsigned int bufferNumber;

	// Transpose the matrices to prepare them for the shader.
	worldMatrix			= worldMatrix.Transpose();
	viewMatrix			= viewMatrix.Transpose();
	projectionMatrix	= projectionMatrix.Transpose();
	reflectionMatrix	= reflectionMatrix.Transpose();

	auto deviceContext = m_Graphics->getRenderer()->getContext();

	////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////

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

	////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////

	// Lock the light constant buffer so it can be written to.
	result = deviceContext->Map(m_lightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr2 = (LightBufferType*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.
	dataPtr2->ambientColor = ambientColor;
	dataPtr2->diffuseColor = diffuseColor;
	dataPtr2->lightDirection = lightDirection;
	dataPtr2->padding = 0.0f;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_lightBuffer.Get(), 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, m_lightBuffer.GetAddressOf());

	////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////
	// Lock the fog constant buffer so it can be written to.
	result = deviceContext->Map(m_fogBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr3 = (FogBufferType*)mappedResource.pData;

	// Copy the fog information into the fog constant buffer.
	dataPtr3->fogStart = fogStart;
	dataPtr3->fogEnd = fogEnd;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_fogBuffer.Get(), 0);

	// Set the position of the fog constant buffer in the vertex shader.
	bufferNumber = 1;

	// Now set the fog buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, m_fogBuffer.GetAddressOf());

	////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////
	// Lock the reflection constant buffer so it can be written to.
	result = deviceContext->Map(m_reflectionBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the matrix constant buffer.
	dataPtr4 = (ReflectionBufferType*)mappedResource.pData;

	// Copy the matrix into the reflection constant buffer.
	dataPtr4->reflectionMatrix = reflectionMatrix;

	// Unlock the reflection constant buffer.
	deviceContext->Unmap(m_reflectionBuffer.Get(), 0);

	// Set the position of the reflection constant buffer in the vertex shader.
	bufferNumber = 1;

	// Now set the reflection constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, m_reflectionBuffer.GetAddressOf());

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);

	// Set the reflection texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(1, 1, &reflectionTexture);

	return true;
}


void Shader::Reset()
{
	ResetShader();
}

void Shader::ResetShader()
{
	m_vertexShader.Reset();
	m_pixelShader.Reset();
	m_layout.Reset();
	m_matrixBuffer.Reset();
	m_sampleState.Reset();
	m_fogBuffer.Reset();
	m_reflectionBuffer.Reset();
}

bool Shader::GetShaderFile(std::string fileName, ShaderFileType* shaderData)
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
