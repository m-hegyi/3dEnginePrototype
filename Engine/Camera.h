#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <d3d11.h>
#include "SimpleMath.h"

#pragma once
class Camera
{
public:
	Camera();
	~Camera();

	void SetPosition(DirectX::SimpleMath::Vector3 position);
	void SetRotation(DirectX::SimpleMath::Vector3 rotation);

	void Render();
	void GetViewMatrix(DirectX::SimpleMath::Matrix &viewMatrix) const;

	DirectX::SimpleMath::Vector3 getPosition() const { return m_position; };
	DirectX::SimpleMath::Vector3 getRotation() const { return m_rotation; };

private:
	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_rotation;
	DirectX::SimpleMath::Matrix m_viewMatrix;
};

#endif