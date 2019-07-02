#ifndef _CHUNK_H_
#define _CHUNK_H_

#include <vector>
#include <memory>
#include <array>
#include <d3d11.h>
#include <SimpleMath.h>

#include "../Engine/Model.h"
#include "../Engine/Shader.h"
#include "../Engine/Shape.h"


#pragma once
class Chunk
{
public: 
	Chunk();
	~Chunk();

	void Initialize(ID3D11Device* device, DirectX::SimpleMath::Vector3 position);

	void Load();
	void Unload();

	bool Render(ID3D11DeviceContext* deviceContext, std::shared_ptr<Shader>Shader, 
		DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);
	bool Update();

private:
	std::unique_ptr<Model>					m_Model;
	//std::vector<std::unique_ptr<Model>>	m_Models;

	DirectX::SimpleMath::Vector3 m_position;
};

#endif // !_CHUNK_H_
