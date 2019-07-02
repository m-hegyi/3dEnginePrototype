#include "Input.h"
#include <string>

using namespace DirectX;

Input::Input()
{

}

Input::~Input()
{

}

void Input::Initialize(std::shared_ptr<Camera> camera, HWND hwnd)
{
	m_keyboard = std::make_unique<Keyboard>();
	m_mouse = std::make_unique<Mouse>();

	m_mouse->SetWindow(hwnd);

	m_Camera = camera;
}

void Input::Update(float elapsedTime)
{
	auto kb = m_keyboard->GetState();

	float pitch = m_Camera->getPitch();
	float yaw = m_Camera->getYaw();

	SimpleMath::Vector3 move = SimpleMath::Vector3::Zero;

	if (kb.Up || kb.W) {
		move.z -= 1.f * elapsedTime;
	}
	
	if (kb.Down || kb.S) {
		move.z += 1.f * elapsedTime;
	}

	if (kb.Left || kb.A) {
		move.x += 1.f * elapsedTime;
	}

	if (kb.Right || kb.D) {
		move.x -= 1.f * elapsedTime;
	}

	if (kb.LeftShift) {
		move.y -= 1.f * elapsedTime;
	}

	if (kb.Space) {
		move.y += 1.f * elapsedTime;
	}

	SimpleMath::Quaternion q = SimpleMath::Quaternion::CreateFromYawPitchRoll(yaw, .0f, .0f);

	move = SimpleMath::Vector3::Transform(move, q);

	move *= 5.f;

	m_Camera->SetPosition(m_Camera->getPosition() + move);

	auto ms = m_mouse->GetState();

	if (ms.leftButton) {
		m_mouse->SetMode(Mouse::MODE_RELATIVE);
	}

	if (ms.rightButton) {
		m_mouse->SetMode(Mouse::MODE_ABSOLUTE);
	}

	if (ms.positionMode == Mouse::MODE_RELATIVE) {

		SimpleMath::Vector3 delta = SimpleMath::Vector3(float(ms.x), float(ms.y), .0f) * 0.9f * elapsedTime;

		m_Camera->SetRotation(m_Camera->getYaw() + delta.x, m_Camera->getPitch() + delta.y, .0f);

		/*wchar_t buffer[256];

		wsprintfW(buffer, L"%d", float(ms.x));

		OutputDebugString(buffer);
		OutputDebugString(L"\n");*/

		//m_Camera->SetRotation(0.0f, rotation.y + delta.x, .0f);
	}

}