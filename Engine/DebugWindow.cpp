#include "DebugWindow.h"

DebugWindow::DebugWindow()
{
}

DebugWindow::~DebugWindow()
{
}

bool DebugWindow::Initialize(std::shared_ptr<Graphics> graphics, int bitmapWidth, int bitmapHeight)
{
	m_Graphics = graphics;

	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	m_previousPosX = -1;
	m_previousPosY = -1;
	m_previousScreenWidth = -1;
	m_previousScreenHeight = -1;

	m_worldMatrix = DirectX::SimpleMath::Matrix::Identity;

	if (!InitializeBuffers()) {
		return false;
	}

	return true;
}

bool DebugWindow::Render()
{
	if (!RenderBuffers())
	{
		return false;
	}

	return true;
}

bool DebugWindow::Update(int positionX, int positionY)
{
	if (!UpdateBuffers(positionX, positionY)) {
		return false;
	}

	return true;
}

bool DebugWindow::InitializeBuffers()
{
	std::unique_ptr<VertexType[]> vertices;
	std::unique_ptr<unsigned long[]> indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	m_vertexCount = 6;

	m_indexCount = m_vertexCount;

	// Create the vertex array
	vertices = std::make_unique<VertexType[]>(m_vertexCount);
	if (!vertices)
	{
		return false;
	}

	// Create the index array
	indices = std::make_unique<unsigned long[]>(m_indexCount);
	if (!indices)
	{
		return false;
	}

	for (i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the static vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices.get();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = m_Graphics->getRenderer()->getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, m_vertexBuffer.ReleaseAndGetAddressOf());
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

	result = m_Graphics->getRenderer()->getDevice()->CreateBuffer(&indexBufferDesc, &indexData, m_indexBuffer.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool DebugWindow::RenderBuffers()
{
	unsigned int stride;
	unsigned int offset;

	auto deviceContext = m_Graphics->getRenderer()->getContext();

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}

bool DebugWindow::UpdateBuffers(int positionX, int positionY)
{
	auto screenWidth = m_Graphics->getRenderer()->GetOutputWidth();
	auto screenHeight = m_Graphics->getRenderer()->GetOutputHeight();

	if ((positionX == m_previousPosX) && (positionY == m_previousPosY)
		&& (screenWidth == m_previousScreenWidth) && (screenHeight == m_previousScreenHeight))
	{
		return true;
	}

	float left, right, top, bottom;
	std::unique_ptr<VertexType[]> vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;

	m_previousPosX = positionX;
	m_previousPosY = positionY;

	m_previousScreenWidth = screenWidth;
	m_previousScreenHeight = screenHeight;


	// Calculate the screen coordinates of the left and right side of the bitmap
	left = (float)((screenWidth / 2) * -1) + (float)positionX;
	right = left + (float)m_bitmapWidth;

	// Calculate the screen coordinates of the top and bottom if the bitmap
	top = (float)(screenHeight / 2) - (float)positionY;
	bottom = top - (float)m_bitmapHeight;

	vertices = std::make_unique<VertexType[]>(m_vertexCount);
	if (!vertices)
	{
		return false;
	}

	vertices[0].position = DirectX::SimpleMath::Vector3(left, top, 0.0f);  // Top left.
	vertices[0].texture = DirectX::SimpleMath::Vector2(0.0f, 0.0f);

	vertices[1].position = DirectX::SimpleMath::Vector3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].texture = DirectX::SimpleMath::Vector2(1.0f, 1.0f);

	vertices[2].position = DirectX::SimpleMath::Vector3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].texture = DirectX::SimpleMath::Vector2(0.0f, 1.0f);

	// Second triangle.
	vertices[3].position = DirectX::SimpleMath::Vector3(left, top, 0.0f);  // Top left.
	vertices[3].texture = DirectX::SimpleMath::Vector2(0.0f, 0.0f);

	vertices[4].position = DirectX::SimpleMath::Vector3(right, top, 0.0f);  // Top right.
	vertices[4].texture = DirectX::SimpleMath::Vector2(1.0f, 0.0f);

	vertices[5].position = DirectX::SimpleMath::Vector3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].texture = DirectX::SimpleMath::Vector2(1.0f, 1.0f);

	result = m_Graphics->getRenderer()->getContext()->Map(m_vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices.get(), (sizeof(VertexType) * m_vertexCount));

	m_Graphics->getRenderer()->getContext()->Unmap(m_vertexBuffer.Get(), 0);


	return true;
}
