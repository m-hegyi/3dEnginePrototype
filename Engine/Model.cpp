#include "Model.h"
#include <fstream>

using namespace DirectX::SimpleMath;

Model::Model() 
{
}

Model::~Model()
{
}

bool Model::Initialize(std::shared_ptr<Graphics> graphics, char* fileName, wchar_t* textureFileName)
{
	// TODO
	// Load model from file properly
	m_Graphics = graphics;

	if (!LoadModel(fileName)) {
		return false;
	}

	if (!InitializeBuffer()) {
		return false;
	}

	if (!LoadTexture(textureFileName)) {
		return false;
	}

	m_position = Vector3(0.0f, 0.0f, 0.0f);
	m_rotation = Vector3(0.0, 0.0f, 0.0f);

	return true;
}

bool Model::InitializeBuffer()
{
	//
	// TODO: DX::ThrowIfFailed -> errorhandling
	//

	//
	// TODO: multiple vertexType loading
	//


	std::unique_ptr<VertexTextureType[]> vertices;
	std::unique_ptr<InstanceType[]> instances;
	//std::unique_ptr<unsigned long[]> indices;
	D3D11_BUFFER_DESC vertexBufferDesc, /*indexBufferDesc,*/ instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, /*indexData,*/ instanceData;
	HRESULT result;

	vertices = std::make_unique<VertexTextureType[]>(m_vertexCount);
	if (!vertices) {
		return false;
	}

	/*indices = std::make_unique<unsigned long[]>(m_indexCount);
	if (!indices) {
		return false;
	}*/

	for (int i = 0; i < m_vertexCount; i++) {
		vertices[i].position	= Vector3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture		= Vector2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal		= Vector3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		//indices[i] = i;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexTextureType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices.get();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	auto device = m_Graphics->getRenderer()->getDevice();

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, m_vertexBuffer.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	m_instanceCount = 4;

	instances = std::make_unique<InstanceType[]>(m_instanceCount);
	if (!instances) 
	{
		return false;
	}

	// Load the instance array with data.
	instances[0].position = Vector3(-1.5f, -1.5f, 5.0f);
	instances[1].position = Vector3(-1.5f, 1.5f, 5.0f);
	instances[2].position = Vector3(1.5f, -1.5f, 5.0f);
	instances[3].position = Vector3(1.5f, 1.5f, 5.0f);

	// Set up the description of the instance buffer.
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * m_instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the instance data.
	instanceData.pSysMem = instances.get();
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	// Create the instance buffer.
	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, m_instanceBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	/*
	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices.get();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, m_indexBuffer.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return false;
	}*/

	return true;
}

bool Model::Render()
{
	if (!RenderBuffer()) {
		return false;
	}

	return true;
}

void Model::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void Model::SetRotation(float yaw, float pitch, float roll)
{
	m_rotation.x = yaw;
	m_rotation.y = pitch;
	m_rotation.z = roll;
}

bool Model::RenderBuffer()
{
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];


	// Set buffer strides and offsets.
	strides[0] = sizeof(VertexTextureType);
	strides[1] = sizeof(InstanceType);

	offsets[0] = 0;
	offsets[1] = 0;

	bufferPointers[0] = m_vertexBuffer.Get();
	bufferPointers[1] = m_instanceBuffer.Get();

	auto deviceContext = m_Graphics->getRenderer()->getContext();

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 2,bufferPointers, strides, offsets);

	// Set the index buffer to active in the input assembler so it can be rendered.
	//deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}

bool Model::LoadModel(char* fileName)
{
	std::ifstream fin;
	char input;
	int i;

	// Open the model file
	fin.open(fileName);

	// If it could not open the file then exit
	if (fin.fail()) {
		return false;
	}

	// Read up the value of vertex count
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count
	fin >> m_vertexCount;

	// Set the number of inidcies to be the same as the vertex count
	//m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in
	m_model = std::make_unique<ModelType[]>(m_vertexCount);
	if (!m_model) {
		return false;
	}

	// Read up to the beginning of the data
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data
	for (i = 0; i < m_vertexCount; i++) {
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file
	fin.close();

	return true;
}


bool Model::LoadTexture(wchar_t* textureFileName)
{
	m_Texture = std::make_unique<Texture>();

	if (!m_Texture) {
		return false;
	}

	if (!m_Texture->Initialize(m_Graphics, textureFileName)) {
		return false;
	}

	return true;
}

void Model::Reset()
{
	ResetBuffer();
	ResetModel();
	ResetTexture();
}

void Model::ResetBuffer()
{
	m_vertexBuffer.Reset();
	m_instanceBuffer.Reset();
	//m_indexBuffer.Reset();
}

void Model::ResetModel()
{
	m_model.reset();
}

void Model::ResetTexture()
{
	m_Texture.reset();
}