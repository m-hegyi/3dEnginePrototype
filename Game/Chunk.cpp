#include "Chunk.h"

Chunk::Chunk()
{

}

Chunk::~Chunk()
{

}

void Chunk::Initialize(ID3D11Device* device, DirectX::SimpleMath::Vector3 position)
{
	m_position = position;

	m_Model = std::make_unique<Model>();

	m_Model->Initialize(device, "Data/sphere.txt", L"Assets/texture1.png");
}

bool Chunk::Update() 
{
	return true;
}

bool Chunk::Render(ID3D11DeviceContext* deviceContext, std::shared_ptr<Shader> Shader,
	DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	m_Model->Render(deviceContext);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			for (int z = 0; z < 8; z++) {

				m_Model->SetPosition((j * 2), i*2, z * 2);

				Shader->Render(deviceContext, m_Model->getIndexCount(), m_Model->GetWorldMatrix(), view, projection, m_Model->getTexture(),
					DirectX::SimpleMath::Vector3(1.0f, -1.0f, 1.0f), DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::SimpleMath::Vector4(0.25f, 0.25f, 0.25f, 1.0f));
			}
		}
	}
	return true;
}