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

	//m_Terrain = std::make_unique<ChunkTerrain>();

	//m_Terrain->Initialize(m_Graphics);

	//m_Shader = std::make_shared<Shader>();

	//if (!m_Shader->Initialize(window, m_Graphics, "LightVertex.cso", "LightPixel.cso")) {
		//OnDeviceLost();
	//}

	m_Light = std::make_unique<Light>();

	if (!m_Light) {
		//OnDeviceLost();
	}

	m_Light->SetAmbientColor(0.3f, 0.3f, 0.3f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, -1.0f, 1.0f);

	//m_Bitmap = std::make_unique<Bitmap>();

	//m_Bitmap->Initialize(m_Graphics, L"Assets/texture1.png", 240, 240);

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

	m_FogShader = std::make_shared<FogShader>(m_Graphics);

	m_FogShader->Initialize(window);

	m_Model2 = std::make_unique<Model>();

	std::vector<DirectX::SimpleMath::Vector3> position2;

	position2.push_back(DirectX::SimpleMath::Vector3(4.0f, 0.0f, 0.0f));

	m_Model2->Initialize(m_Graphics, "Data/cube.txt", L"Assets/texture1.png", position2);

	m_Model3 = std::make_unique<Model>();

	std::vector<DirectX::SimpleMath::Vector3> position3;

	position3.push_back(DirectX::SimpleMath::Vector3(6.0f, 0.0f, 0.0f));

	m_Model3->Initialize(m_Graphics, "Data/cube.txt", L"Assets/texture1.png", position3);

	m_Floor = std::make_unique<Model>();

	std::vector<DirectX::SimpleMath::Vector3> position4;

	position4.push_back(DirectX::SimpleMath::Vector3(0.0f, -3.0f, 0.0f));

	m_Floor->Initialize(m_Graphics, "Data/floor.txt", L"Assets/texture1.png", position4);

	m_ReflectionShader = std::make_shared<ReflectionShader>(m_Graphics);

	m_ReflectionShader->Initialize(window);

	//m_DebugWindow = std::make_unique<DebugWindow>();

	//m_DebugWindow->Initialize(m_Graphics, 300, 300);
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

	/*m_DebugWindow->Render();

	m_Shader->Render(m_DebugWindow->GetIndexCount(), m_DebugWindow->GetWorldMatrix(), m_Camera->Get2DViewMatrix(), m_Camera->GetOrthoMatrix(),
		m_RenderTexture->GetShaderResourceView(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
*/
	//m_Bitmap->Render();

	//m_Shader->Render(m_Bitmap->GetIndexCount(), m_Bitmap->GetWorldMatrix(), m_Camera->Get2DViewMatrix(), m_Camera->GetOrthoMatrix(),
		//m_Bitmap->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));

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

	m_RenderTexture->ClearRenderTarget(0.0f, 0.0f, 1.0f, 1.0f);

	m_Camera->CalculateReflectionMatrix();

	auto reflectionMatrix = m_Camera->GetReflectionMatrix();

	m_Floor->Render();

	m_LightShader->Render(m_Model->GetVertexCount(), m_Model->GetInstanceCount(), m_Model->GetWorldMatrix(), reflectionMatrix,
		m_Camera->GetProjectionMatrix(), m_Model->getTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	RenderScene();

	m_Graphics->getRenderer()->SetBackBufferRenderTarget();
	return true;
}

bool MainScreen::RenderScene()
{
	
	m_Model->Render();

	m_BasicShader->Render(m_Model->GetVertexCount(), m_Model->GetInstanceCount(),
		m_Model->GetWorldMatrix(), m_Camera->GetViewMatrix(), m_Camera->GetProjectionMatrix(), m_Model->getTexture());

	m_Model2->Render();

	m_LightShader->Render(m_Model2->GetVertexCount(), m_Model2->GetInstanceCount(), m_Model2->GetWorldMatrix(),
		m_Camera->GetViewMatrix(), m_Camera->GetProjectionMatrix(), m_Model2->getTexture(),
		m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Light->GetAmbientColor());

	m_Model3->Render();

	m_FogShader->Render(m_Model3->GetVertexCount(), m_Model3->GetInstanceCount(), m_Model3->GetWorldMatrix(),
		m_Camera->GetViewMatrix(), m_Camera->GetProjectionMatrix(), m_Model3->getTexture(),
		m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Light->GetAmbientColor(),
		0.0f, 25.f);
	
	m_Floor->Render();

	m_ReflectionShader->Render(m_Floor->GetVertexCount(), m_Floor->GetInstanceCount(), m_Floor->GetWorldMatrix(),
		m_Camera->GetViewMatrix(), m_Camera->GetProjectionMatrix(), m_Floor->getTexture(),
		m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Light->GetAmbientColor(), 
		m_RenderTexture->GetShaderResourceView(), m_Camera->GetReflectionMatrix());

	//m_Terrain->Render(m_Shader, m_Camera);

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

	//m_Bitmap->Update(400, 100);

	//m_DebugWindow->Update(100, 100);


	return true;
}