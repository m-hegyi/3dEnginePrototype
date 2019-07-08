#include "Camera.h"
#include <DirectXMath.h>

using namespace DirectX;

Camera::Camera() : m_pitch(0), m_yaw(0), m_roll(0)
{
	m_screenWidth = 1280;
	m_screenHeight = 720;
	m_fov = XM_PI / 4.f;
	m_nearZ = 1.0f;
	m_farZ = 10000.f;
	m_position = SimpleMath::Vector3(-7.f, 3.f, 8.f);
	m_rotation = SimpleMath::Vector3(0.f, 0.f, 0.f);
	m_pitch = 0.1f;
	m_yaw = -1.6f;

	CalculateViewMatrix();
	Calculate2DViewMatrix();
	CalculateProjectionMatrix();
	CalculateOrthoMatrix();
}

Camera::Camera(int screenWidth, int screenHeight, float fov, float nearZ, float farZ,
	SimpleMath::Vector3 position, SimpleMath::Vector3 rotation)
	: m_screenWidth(screenWidth), m_screenHeight(screenHeight), m_fov(fov), m_nearZ(nearZ), m_farZ(farZ),
	m_position(position), m_rotation(rotation)
{
}

Camera::~Camera()
{
}

void Camera::SetPosition(DirectX::SimpleMath::Vector3 position)
{
	m_position = position;

	CalculateViewMatrix();
	Calculate2DViewMatrix();
	CalculateProjectionMatrix();
}

void Camera::UpdateScreenSize(int screenWidth, int screenHeight)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	CalculateViewMatrix();
	Calculate2DViewMatrix();
	CalculateProjectionMatrix();
	CalculateOrthoMatrix();
}

void Camera::Render()
{
}

void Camera::SetRotation(float yaw, float pitch, float roll)
{

	float limit = XM_PI / 2.0f - 0.01f;
	m_pitch = max(-limit, pitch);
	m_pitch = min(limit, pitch);

	m_yaw = yaw;

	// keep longitude in sane range by wrapping
	if (m_yaw > XM_PI)
	{
		m_yaw -= XM_PI * 2.0f;
	}
	else if (m_yaw < -XM_PI)
	{
		m_yaw += XM_PI * 2.0f;
	}

	m_roll = roll;

	CalculateViewMatrix();
}

void Camera::CalculateViewMatrix()
{
	float y = sinf(m_pitch);
	float r = cosf(m_pitch);
	float z = r * cosf(m_yaw);
	float x = r * sinf(m_yaw);

	auto lookAt = m_position - SimpleMath::Vector3(x, y, z);

	auto upDirection = SimpleMath::Vector3(sinf(m_roll), cosf(m_roll), 0.0f);

	//lookAt = SimpleMath::Vector3(0, 0, 5);

	m_viewMatrix = XMMatrixLookAtLH(m_position, lookAt, upDirection);

	//m_viewMatrix = XMMatrixLookAtLH(m_position, SimpleMath::Vector3::Zero, SimpleMath::Vector3::UnitY);

	//XMMatrixLookToLH(m_position, m_rotation, SimpleMath::Vector3::UnitY);
	//m_viewMatrix = XMMatrixLookToLH(m_position, m_rotation, SimpleMath::Vector3::/UnitY);
}

void Camera::Calculate2DViewMatrix()
{
	m_2dViewMatrix = XMMatrixLookAtLH(SimpleMath::Vector3(0.f, 0.f, -10.f),
		SimpleMath::Vector3(0.f, 0.f, 1.0f), SimpleMath::Vector3(0.f, 1.f, 0.f));
}

void Camera::CalculateProjectionMatrix()
{
	m_projectionMatrix = XMMatrixPerspectiveFovLH(m_fov, float(m_screenWidth) / float(m_screenHeight), m_nearZ, m_farZ);
}

void Camera::CalculateOrthoMatrix()
{
	m_orthoMatrix = XMMatrixOrthographicLH(m_screenWidth, m_screenHeight, m_nearZ, m_farZ);
}
