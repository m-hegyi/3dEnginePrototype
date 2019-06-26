#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <d3d11.h>
#include "SimpleMath.h"

#pragma once
class Camera
{
public:
	Camera();
	Camera(int screenWidth, int screenHeight, float fov, float nearZ, float farZ,
		DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector3 rotation);
	~Camera();

	void SetPosition(DirectX::SimpleMath::Vector3 position);

	void UpdateScreenSize(int screenWidth, int screenHeight);

	void Render();
	void GetViewMatrix(DirectX::SimpleMath::Matrix &viewMatrix) { viewMatrix = m_viewMatrix; };
	DirectX::SimpleMath::Matrix GetViewMatrix() const { return m_viewMatrix; }
	void GetProjectionMatrix(DirectX::SimpleMath::Matrix &projectionMatrix) { projectionMatrix = m_projectionMatrix; }
	DirectX::SimpleMath::Matrix GetProjectionMatrix() const { return m_projectionMatrix; }

	// Exprimentings
	void SetRotation(float yaw, float pitch, float roll);

	DirectX::SimpleMath::Vector3 getPosition() const { return m_position; };
	DirectX::SimpleMath::Vector3 getRotation() const { return DirectX::SimpleMath::Vector3(m_yaw, m_pitch, m_roll); };
	float getPitch() const { return m_pitch; }
	float getYaw() const { return m_yaw; }
	void GetPosition(DirectX::SimpleMath::Vector3 &position) const { position = m_position; }
	void GetRotiation(DirectX::SimpleMath::Vector3 &rotation) const { rotation = m_rotation; }

private:
	void CalculateViewMatrix();
	void CalculateProjectionMatrix();

private:
	int m_screenWidth, m_screenHeight;
	float m_fov, m_nearZ, m_farZ;

	float m_pitch, m_yaw, m_roll;

	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_rotation;
	DirectX::SimpleMath::Matrix m_viewMatrix;
	DirectX::SimpleMath::Matrix m_projectionMatrix;
};

#endif