#include "Shape.h"

#include <fstream>

using namespace DirectX::SimpleMath;

Shape::Shape()
{
}

Shape::~Shape()
{
}

bool Shape::Initialize(ID3D11Device* device, char * fileName)
{
	if (!LoadModel(fileName)) {
		return false;
	}

	InitializeBuffer(device);

	return true;
}

bool Shape::Render(ID3D11DeviceContext * deviceContext)
{
	if (!RenderBuffer(deviceContext)) {
		return false;
	}

	return true;
}

bool Shape::LoadModel(char * fileName)
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

bool Shape::InitializeBuffer(ID3D11Device * device)
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
		vertices[i].position = Vector3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = Vector2(m_model[i].tu, m_model[i].tv);

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

bool Shape::RenderBuffer(ID3D11DeviceContext * deviceContext)
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
