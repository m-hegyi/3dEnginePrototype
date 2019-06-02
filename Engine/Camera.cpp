#include "Camera.h"

using namespace DirectX::SimpleMath;

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::SetPosition(DirectX::SimpleMath::Vector3 position)
{
}

void Camera::SetRotation(DirectX::SimpleMath::Vector3 rotation)
{
}

void Camera::Render()
{
	Vector3 up, position, lookAt;
	float yaw, pitch, roll;
	Matrix rotationMatrix;

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = m_position.x;
	position.y = m_position.y;
	position.z = m_position.z;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch	= m_rotation.x * 0.0174532925f;
	yaw		= m_rotation.y * 0.0174532925f;
	roll	= m_rotation.z * 0.0174532925f;

	rotationMatrix = rotationMatrix.CreateFromYawPitchRoll(yaw, pitch, roll);
}
