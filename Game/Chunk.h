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
#include "../Engine/Camera.h"


#pragma once
class Chunk
{
public: 
	Chunk();
	~Chunk();

	void Initialize(std::shared_ptr<Graphics> graphics, DirectX::SimpleMath::Vector3 position);

	void Load();
	void Unload();

	bool Render(std::shared_ptr<Shader>Shader, std::shared_ptr<Camera> camera);
	bool Update();

private:
	std::unique_ptr<Model>					m_Model;
	std::shared_ptr<Graphics>				m_Graphics;
	//std::vector<std::unique_ptr<Model>>	m_Models;

	DirectX::SimpleMath::Vector3 m_position;
};

#endif // !_CHUNK_H_
