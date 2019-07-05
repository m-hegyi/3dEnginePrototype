#ifndef _CHUNK_TERRAIN_H_
#define _CHUNK_TERRAIN_H_

#include <vector>
#include <memory>
#include <d3d11.h>

#include "Chunk.h"
#include "../Engine/Shader.h"
#include "../Engine/Texture.h"
#include "../Engine/Shape.h"
#include "../Engine/Camera.h"

#pragma once
class ChunkTerrain
{
public:
	ChunkTerrain();
	~ChunkTerrain();

	void Initialize(std::shared_ptr<Graphics> graphics);

	bool Render(std::shared_ptr<Shader>Shader, std::shared_ptr<Camera> camera);

private:
	std::vector<std::shared_ptr<Chunk>> m_Chunks;
};

#endif 