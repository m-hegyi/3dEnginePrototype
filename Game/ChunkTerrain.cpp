#include "ChunkTerrain.h"

ChunkTerrain::ChunkTerrain()
{
}

ChunkTerrain::~ChunkTerrain()
{
}

void ChunkTerrain::Initialize(std::shared_ptr<Graphics> graphics)
{
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < 1; j++) {
			auto chunk = std::make_shared<Chunk>();

			auto pos = DirectX::SimpleMath::Vector3(64 * i, 1.0f, 64 * j);

			chunk->Initialize(graphics, pos);

			m_Chunks.push_back(chunk);
		}
	}

}

bool ChunkTerrain::Render(std::shared_ptr<Shader>Shader,
	std::shared_ptr<Camera> camera)
{
	for (auto chunk : m_Chunks) {
		chunk->Render(Shader, camera);
	}

	return true;
}
