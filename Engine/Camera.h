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
	DirectX::SimpleMath::Matrix GetViewMatrix() const { return m_viewMatrix; };
	DirectX::SimpleMath::Matrix GetProjectionMatrix() const { return m_projectionMatrix; };
	DirectX::SimpleMath::Matrix Get2DViewMatrix() const { return m_2dViewMatrix; };
	DirectX::SimpleMath::Matrix GetOrthoMatrix() const { return m_orthoMatrix; };

	DirectX::SimpleMath::Matrix GetReflectionMatrix() const { return m_reflectionMatrix; };

	// Exprimentings
	void SetRotation(float yaw, float pitch, float roll);

	DirectX::SimpleMath::Vector3 getPosition() const { return m_position; };
	DirectX::SimpleMath::Vector3 getRotation() const { return DirectX::SimpleMath::Vector3(m_yaw, m_pitch, m_roll); };
	float getPitch() const { return m_pitch; }
	float getYaw() const { return m_yaw; }
	void GetPosition(DirectX::SimpleMath::Vector3 &position) const { position = m_position; }
	void GetRotiation(DirectX::SimpleMath::Vector3 &rotation) const { rotation = m_rotation; }

	float GetScreenDepth() const { return m_farZ; };

	int GetScreenWidth() const { return m_screenWidth; };
	int GetScreenHeight() const { return m_screenHeight; };

	void CalculateReflectionMatrix();
private:
	void CalculateViewMatrix();
	void Calculate2DViewMatrix();
	void CalculateProjectionMatrix();
	void CalculateOrthoMatrix();

private:
	int m_screenWidth, m_screenHeight;
	float m_fov, m_nearZ, m_farZ;

	float m_pitch, m_yaw, m_roll;

	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_rotation;
	DirectX::SimpleMath::Matrix m_viewMatrix;
	DirectX::SimpleMath::Matrix m_2dViewMatrix;
	DirectX::SimpleMath::Matrix m_projectionMatrix;
	DirectX::SimpleMath::Matrix m_orthoMatrix;
	DirectX::SimpleMath::Matrix m_reflectionMatrix;
};

#endif