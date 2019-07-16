#include "MainScreen.h"

#include <Mouse.h>
#include <vector>

MainScreen::MainScreen()
{
};

MainScreen::~MainScreen()
{
};

void MainScreen::Load(HWND window)
{
	m_Camera = std::make_shared<Camera>();

	m_Bitmap = std::make_unique<Bitmap>();

	m_Bitmap->Initialize(m_Graphics, L"Assets/texture1.png", 240, 240);

	m_RenderTexture = std::make_unique<RenderTexture>(m_Graphics);

	m_RenderTexture->Initialize(m_Graphics->getRenderer()->GetOutputWidth(), m_Graphics->getRenderer()->GetOutputHeight());

	m_Model = std::make_unique<Model>();

	std::vector<DirectX::SimpleMath::Vector3> position1;

	position1.push_back(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f));

	m_Model->Initialize(m_Graphics, "Data/cube.txt", L"Assets/texture1.png", position1);

	m_BasicShader = std::make_shared<BasicShader>(m_Graphics);

	m_BasicShader->Initialize(window);

	m_LightShader = std::make_shared<LightShader>(m_Graphics);

	m_LightShader->Initialize(window);

	m_Light = std::make_unique<Light>();

	m_Light->SetDirection(1.0f, 1.0f, 1.0f);
	m_Light->SetAmbientColor(1.0f, 1.0f, 1.0f, .2f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);

	m_DebugWindow = std::make_unique<DebugWindow>();

	m_DebugWindow->Initialize(m_Graphics, 300, 300);
}

void MainScreen::UnLoad()
{
}

bool MainScreen::Render()
{

	if (!RenderToTexture()) 
	{
		return false;
	}

	if (!m_Graphics->BeginScreen()) {
		return false;
	}

	RenderScene();

	m_Graphics->getRenderer()->TurnZBufferOff();

	m_Bitmap->Render();

	m_BasicShader->Render(m_Bitmap->GetIndexCount(), 1, m_Bitmap->GetWorldMatrix(),
		m_Camera->Get2DViewMatrix(), m_Camera->GetOrthoMatrix(), m_Bitmap->GetTexture());

	m_DebugWindow->Render();
	
	m_BasicShader->Render(m_DebugWindow->GetIndexCount(), 1, m_DebugWindow->GetWorldMatrix(),
		m_Camera->Get2DViewMatrix(), m_Camera->GetOrthoMatrix(), m_RenderTexture->GetShaderResourceView());

	m_Graphics->getRenderer()->TurnZBufferOn();

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

bool MainScreen::RenderToTexture()
{

	m_RenderTexture->SetRenderTarget();

	m_RenderTexture->ClearRenderTarget(0.0f, 1.0f, 1.0f, 1.0f);

	RenderScene();

	m_Graphics->getRenderer()->SetBackBufferRenderTarget();
	return true;
}

bool MainScreen::RenderScene()
{
	
	m_Model->Render();

	m_LightShader->Render(m_Model->GetVertexCount(), m_Model->GetInstanceCount(),
		m_Model->GetWorldMatrix(), m_Camera->GetViewMatrix(), m_Camera->GetProjectionMatrix(), m_Model->getTexture(), 
		m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Light->GetAmbientColor());


	return true;
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
	stringStream << "Camera rot:" << "yaw: " << rot.x << " pitch: " << rot.y << " roll: " << rot.z << std::endl;
	stringStream << "Rendered object count: " << m_Graphics->GetObjCount();

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

	m_Bitmap->Update(400, 100);

	m_DebugWindow->Update(100, 100);


	return true;
}