#include "ChunkTerrain.h"

ChunkTerrain::ChunkTerrain()
{
}

ChunkTerrain::~ChunkTerrain()
{
}

void ChunkTerrain::Initialize(ID3D11Device * device)
{
	for (int i = 0; i < 1; i++) {
		auto chunk = std::make_shared<Chunk>();

		auto pos = DirectX::SimpleMath::Vector3(16 * i, 1.0f, 1.0f);

		chunk->Initialize(device, pos);

		m_Chunks.push_back(chunk);
	}

}

bool ChunkTerrain::Render(ID3D11DeviceContext* deviceContext, std::shared_ptr<Shader>Shader,
	DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	for (auto chunk : m_Chunks) {
		chunk->Render(deviceContext, Shader, view, projection);
	}

	return true;
}
