#include "Model.h"
#include <fstream>

using namespace DirectX::SimpleMath;

Model::Model() 
{
}

Model::~Model()
{
}

bool Model::Initialize(ID3D11Device* device, char* fileName, wchar_t* textureFileName)
{
	// TODO
	// Load model from file properly
	if (!LoadModel(fileName)) {
		return false;
	}

	if (!InitializeBuffer(device)) {
		return false;
	}

	if (!LoadTexture(device, textureFileName)) {
		return false;
	}

	m_position = Vector3(0.0f, 0.0f, 0.0f);
	m_rotation = Vector3(0.0, 0.0f, 0.0f);

	return true;
}

bool Model::InitializeBuffer(ID3D11Device* device)
{
	//
	// TODO: DX::ThrowIfFailed -> errorhandling
	//

	//
	// TODO: multiple vertexType loading
	//


	std::unique_ptr<VertexTextureType[]> vertices;
	std::unique_ptr<unsigned long[]> indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	vertices = std::make_unique<VertexTextureType[]>(m_vertexCount);
	if (!vertices) {
		return false;
	}

	indices = std::make_unique<unsigned long[]>(m_indexCount);
	if (!indices) {
		return false;
	}

	for (int i = 0; i < m_vertexCount; i++) {
		vertices[i].position	= Vector3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture		= Vector2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal		= Vector3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
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

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, m_vertexBuffer.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

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
	}

	return true;
}

bool Model::Render(ID3D11DeviceContext* deviceContext)
{
	if (!RenderBuffer(deviceContext)) {
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

bool Model::RenderBuffer(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexTextureType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

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
	m_indexCount = m_vertexCount;

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


bool Model::LoadTexture(ID3D11Device* device, wchar_t* textureFileName)
{
	m_Texture = std::make_unique<Texture>();

	if (!m_Texture) {
		return false;
	}

	if (!m_Texture->Initialize(device, textureFileName)) {
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
	m_indexBuffer.Reset();
}

void Model::ResetModel()
{
	m_model.reset();
}

void Model::ResetTexture()
{
	m_Texture.reset();
}