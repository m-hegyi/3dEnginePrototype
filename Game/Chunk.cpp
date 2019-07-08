#include "Chunk.h"
#include <math.h>
#include <stdlib.h>

using namespace DirectX::SimpleMath;

#define PI 3.14159265

Chunk::Chunk()
{

}

Chunk::~Chunk()
{

}

void Chunk::Initialize(std::shared_ptr<Graphics> graphics, Vector3 position)
{
	m_position = position;
	m_Graphics = graphics;

	m_Model = std::make_unique<Model>();

	std::vector<Vector3> positions;

	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < 1; j++) {
			for (int z = 0; z < 1; z++) {

				positions.push_back(Vector3(position.x + (j * 2), 0.0f, position.z + z * 2));
			}
		}
	}

	m_Model->Initialize(graphics, "Data/cube.txt", L"Assets/texture1.png", positions);
}

bool Chunk::Update() 
{
	return true;
}

bool Chunk::Render(std::shared_ptr<Shader> Shader, std::shared_ptr<Camera> camera)
{
	/*for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			for (int z = 0; z < 8; z++) {
				m_Model->SetPosition((j * 2), i*2, z * 2);

				m_Graphics->getFrustum()->ConstructFrustum(camera->GetScreenDepth(), camera->GetProjectionMatrix(), camera->GetViewMatrix());

				auto position = m_Model->GetPosition();

				if (!m_Graphics->getFrustum()->CheckCube(position.x, position.y, position.z, 1.0f)) 
				{
					continue;
				}

				m_Model->Render();

				Shader->Render(m_Model->getIndexCount(), m_Model->GetWorldMatrix(), camera->GetViewMatrix(), camera->GetProjectionMatrix(), m_Model->getTexture(),
					DirectX::SimpleMath::Vector3(1.0f, -1.0f, 1.0f), DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::SimpleMath::Vector4(0.25f, 0.25f, 0.25f, 1.0f));
			}
		}
	}*/

	m_Model->Render();

	Shader->Render(m_Model->GetVertexCount(), m_Model->GetInstanceCount(), m_Model->GetWorldMatrix(), 
		camera->GetViewMatrix(), camera->GetProjectionMatrix(), m_Model->getTexture(),
		Vector3(1.0f, -1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4(0.25f, 0.25f, 0.25f, 1.0f));

	return true;
}