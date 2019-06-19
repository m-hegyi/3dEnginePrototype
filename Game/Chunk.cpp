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

	// 8 x 8 x 8
	for (int i = 0; i < 8; i++) {

		for (int j = 0; j < 8; j++) {

			for (int z = 0; z < 8; z++) {
				m_Models[i * 8 * 8 + j * 8 + z] = std::make_unique<Model>();

				m_Models[i * 8 * 8 + j * 8 + z]->Initialize(device, "Data/cube.txt", L"Assets/texture1.png");

				m_Models[i * 8 * 8 + j * 8 + z]->SetPosition((float)j * 2 + m_position.x, (float)z * 2 + m_position.y, (float)i * 2 + m_position.z);

			}
		}

	}
}

bool Chunk::Update() 
{
	return true;
}

bool Chunk::Render(ID3D11DeviceContext* deviceContext, std::shared_ptr<Shader> Shader,
	DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			for (int z = 0; z < 8; z++) {
				m_Models[i * 8 * 8 + j * 8 + z]->Render(deviceContext);

				Shader->Render(deviceContext, m_Models[i * 8 * 8 + j * 8 + z]->getIndexCount(), 
					m_Models[i * 8 * 8 + j * 8 + z]->GetWorldMatrix(),
					view, projection, m_Models[i * 8 * 8 + j * 8 + z]->getTexture());
			}
		}
	}
	return true;
}