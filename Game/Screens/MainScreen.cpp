#include "MainScreen.h"

#include <Mouse.h>

MainScreen::MainScreen()
{
};

MainScreen::~MainScreen()
{
};

void MainScreen::Load(HWND window)
{
	m_Camera = std::make_shared<Camera>();

	m_Terrain = std::make_unique<ChunkTerrain>();

	m_Terrain->Initialize(m_Graphics);

	m_Shader = std::make_shared<Shader>();

	if (!m_Shader->Initialize(window, m_Graphics)) {
		//OnDeviceLost();
	}

	m_Light = std::make_unique<Light>();

	if (!m_Light) {
		//OnDeviceLost();
	}

	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 0.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
}

void MainScreen::UnLoad()
{
}

bool MainScreen::Render()
{

	if (!m_Graphics->BeginScreen()) {
		return false;
	}

	m_Terrain->Render(m_Shader, m_Camera);

	m_Graphics->get2DRenderer()->PrintFPS();

	if (!m_Graphics->EndScreen()) {
		return false;
	}

	return true;
}

void MainScreen::UpdateScreenSize(int width, int height)
{
	m_Camera->UpdateScreenSize(width, height);
}

bool MainScreen::Update(DX::StepTimer const& timer)
{
	float elapsedTime = float(timer.GetElapsedSeconds());
	float time = float(timer.GetTotalSeconds());

	//m_Input->Update(elapsedTime);

	uint32_t frames = timer.GetFramesPerSecond();

	std::wostringstream stringStream;

	stringStream.precision(4);

	stringStream << "FPS: " << frames << std::endl;
	stringStream << "Felbontás: " << m_Camera->GetScreenWidth() << "x" << m_Camera->GetScreenHeight() << std::endl;
	auto pos = m_Camera->getPosition();
	auto rot = m_Camera->getRotation();
	stringStream << "Camera pos:" << "x: " << pos.x << " y: " << pos.y << " z: " << pos.z << std::endl;
	stringStream << "Camera rot:" << "yaw: " << rot.x << " pitch: " << rot.y << " roll: " << rot.z;

	m_Graphics->get2DRenderer()->test((WCHAR*)stringStream.str().c_str(), (UINT32)stringStream.str().size());

	auto kb = m_Input->GetKeyboard()->GetState();

	DirectX::SimpleMath::Vector3 move = DirectX::SimpleMath::Vector3::Zero;

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

	DirectX::SimpleMath::Quaternion q = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_Camera->getYaw(), .0f, .0f);

	move = DirectX::SimpleMath::Vector3::Transform(move, q);

	move *= 10.f;

	m_Camera->SetPosition(m_Camera->getPosition() + move);

	auto ms = m_Input->GetMouse()->GetState();

	if (ms.leftButton) {
		m_Input->GetMouse()->SetMode(DirectX::Mouse::MODE_RELATIVE);
	}

	if (ms.rightButton) {
		m_Input->GetMouse()->SetMode(DirectX::Mouse::MODE_ABSOLUTE);
	}

	if (ms.positionMode == DirectX::Mouse::MODE_RELATIVE) {

		DirectX::SimpleMath::Vector3 delta = DirectX::SimpleMath::Vector3(float(ms.x), float(ms.y), .0f) * 0.9f * elapsedTime;

		m_Camera->SetRotation(m_Camera->getYaw() + delta.x, m_Camera->getPitch() + delta.y, .0f);

		/*wchar_t buffer[256];

		wsprintfW(buffer, L"%d", float(ms.x));

		OutputDebugString(buffer);
		OutputDebugString(L"\n");*/

		//m_Camera->SetRotation(0.0f, rotation.y + delta.x, .0f);
	}


	return true;
}