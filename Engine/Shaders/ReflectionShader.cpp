#include "ReflectionShader.h"

ReflectionShader::ReflectionShader()
{
}

ReflectionShader::ReflectionShader(std::shared_ptr<Graphics> graphics)
{
	m_Graphics = graphics;
}

ReflectionShader::~ReflectionShader()
{
}

bool ReflectionShader::Initialize(HWND hwnd)
{
	if (!InitializeShader(hwnd, "ReflectionVertex.cso", "ReflectionPixel.cso"))
	{
		return false;
	}

	return true;
}

bool ReflectionShader::Render(int vertexCount, int instanceCount,
	DirectX::SimpleMath::Matrix worldMatrix, DirectX::SimpleMath::Matrix viewMatrix, DirectX::SimpleMath::Matrix projectionMatrix,
	ID3D11ShaderResourceView* texture, 
	DirectX::SimpleMath::Vector3 lightDirection, DirectX::SimpleMath::Vector4 diffuseColor, DirectX::SimpleMath::Vector4 ambientColor,
	ID3D11ShaderResourceView* reflectionTexture, DirectX::SimpleMath::Matrix reflectionMatrix)
{
	if (!SetShaderParameters(worldMatrix, viewMatrix, projectionMatrix, texture, 
		lightDirection, diffuseColor, ambientColor, reflectionTexture, reflectionMatrix))
	{
		return false;
	}

	RenderShader(vertexCount, instanceCount);

	return true;
}

bool ReflectionShader::InitializeShader(HWND, std::string vertexShaderFile, std::string pixelShaderFile)
{
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D10Blob> errorMessage;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[4];
	unsigned int numElements;

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
	polygonLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[2].InputSlot = 1;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygonLayout[2].InstanceDataStepRate = 1;

	polygonLayout[3].SemanticName = "NORMAL";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

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

bool ReflectionShader::CreateBuffers()
{
	if (!CreateMatrixBuffer())
	{
		return false;
	}

	if (!CreateLightBuffer())
	{
		return false;
	}

	if (!CreateReflectionBuffer())
	{
		return false;
	}

	return true;
}

bool ReflectionShader::CreateReflectionBuffer()
{
	HRESULT result;
	D3D11_BUFFER_DESC reflectionBufferDesc;

	// Setup the description of the reflection dynamic constant buffer that is in the vertex shader.
	reflectionBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	reflectionBufferDesc.ByteWidth = sizeof(ReflectionBufferType);
	reflectionBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	reflectionBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	reflectionBufferDesc.MiscFlags = 0;
	reflectionBufferDesc.StructureByteStride = 0;
	
	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = m_Graphics->getRenderer()->getDevice()->CreateBuffer(&reflectionBufferDesc, NULL, m_reflectionBuffer.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool ReflectionShader::SetShaderParameters(DirectX::SimpleMath::Matrix worldMatrix, DirectX::SimpleMath::Matrix viewMatrix, DirectX::SimpleMath::Matrix projectionMatrix,
	ID3D11ShaderResourceView* texture,
	DirectX::SimpleMath::Vector3 lightDirection, DirectX::SimpleMath::Vector4 diffuseColor, DirectX::SimpleMath::Vector4 ambientColor,
	ID3D11ShaderResourceView* reflectionTexture, DirectX::SimpleMath::Matrix reflectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightBufferType* dataPtr2;
	ReflectionBufferType* dataPtr3;
	unsigned int bufferNumber;

	// Transpose the matrices to prepare them for the shader.
	worldMatrix = worldMatrix.Transpose();
	viewMatrix = viewMatrix.Transpose();
	projectionMatrix = projectionMatrix.Transpose();

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
	// Lock the reflection constant buffer so it can be written to.
	result = deviceContext->Map(m_reflectionBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the matrix constant buffer.
	dataPtr3 = (ReflectionBufferType*)mappedResource.pData;

	// Copy the matrix into the reflection constant buffer.
	dataPtr3->reflectionMatrix = reflectionMatrix;

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
