#ifndef _CHUNK_TERRAIN_H_
#define _CHUNK_TERRAIN_H_

#include <vector>
#include <memory>
#include <d3d11.h>

#include "Chunk.h"
#include "../Engine/Shader.h"
#include "../Engine/Texture.h"
#include "../Engine/Shape.h"

#pragma once
class ChunkTerrain
{
public:
	ChunkTerrain();
	~ChunkTerrain();

	void Initialize(ID3D11Device* device);

	bool Render(ID3D11DeviceContext* deviceContext, std::shared_ptr<Shader>Shader,
		DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);

private:
	std::vector<std::shared_ptr<Chunk>> m_Chunks;
};

#endif 