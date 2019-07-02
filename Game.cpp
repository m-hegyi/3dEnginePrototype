//
// Game.cpp
//
#include "pch.h"

#include <DirectXMath.h>
#include <sstream>
#include "Game.h"

extern void ExitGame();

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept :
    m_window(nullptr),
    m_outputWidth(1280),
    m_outputHeight(720)
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_window = window;
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

	m_Graphics = std::make_unique<Graphics>();

	if (!m_Graphics->Initialize(window, width, height)) {
		OnDeviceLost();
	}

	m_Camera = std::make_shared<Camera>();

	m_Terrain = std::make_unique<ChunkTerrain>();

	m_Terrain->Initialize(m_Graphics->getRenderer()->getDevice());

	/*m_Model = std::make_unique<Model>();

	if (!m_Model->Initialize(m_Graphics->getRenderer()->getDevice(), "Data/cube.txt", L"Assets/texture1.png")) {
		OnDeviceLost();
	}
	
	m_Model2 = std::make_unique<Model>();

	if (!m_Model2->Initialize(m_Graphics->getRenderer()->getDevice(), "Data/cube.txt", L"Assets/texture1.png")) {
		OnDeviceLost();
	}*/


	m_Shader = std::make_shared<Shader>();

	if (!m_Shader->Initialize(m_Graphics->getRenderer()->getDevice(), window)) {
		OnDeviceLost();
	}

	m_Light = std::make_unique<Light>();

	if (!m_Light) {
		OnDeviceLost();
	}

	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 0.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);

	m_Input = std::make_unique<Input>();

	m_Input->Initialize(m_Camera, m_window);

    CreateDevice();

    CreateResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    //*/
}

// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());
	float time = float(timer.GetTotalSeconds());

	m_Input->Update(elapsedTime);

	uint32_t frames = timer.GetFramesPerSecond();

	std::wostringstream stringStream;

	stringStream.precision(4);

	stringStream << "FPS: " << frames << std::endl;
	stringStream << "Felbontás: " << m_outputWidth << "x" << m_outputHeight << std::endl;
	auto pos = m_Camera->getPosition();
	auto rot = m_Camera->getRotation();
	stringStream << "Camera pos:" << "x: " << pos.x << " y: " << pos.y << " z: " << pos.z << std::endl;
	stringStream << "Camera rot:" << "yaw: " << rot.x << " pitch: " << rot.y << " roll: " << rot.z;

	m_Graphics->get2DRenderer()->test((WCHAR*)stringStream.str().c_str(), (UINT32)stringStream.str().size()) ;

    // TODO: Add your game logic here.

	//m_Model->SetRotation(time * .4f, 0.0f, 0.0f);
	//m_Model->SetPosition(time * 0.1f, 0.0f, 0.0f);

	//m_Model2->SetRotation(time * -0.4f, 0.0f, 0.0f);
	//m_Model2->SetPosition(time * -0.1, 0.0f, 0.0f);

	//m_Camera->SetPosition(SimpleMath::Vector3(0.0f, time * 0.5f, 4.0f));
	//m_Camera->SetRotation(0.0f, 0.0f, time * 0.1);
    elapsedTime;
}

// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

	// 
	// TODO: something simple
	//

	//
	// TODO: something else maybe?
	//
	m_view = m_Camera->GetViewMatrix();
	m_projection = m_Camera->GetProjectionMatrix();


	if (!m_Graphics->BeginScreen()) {
		OnDeviceLost();
	}


	//if (!m_Model->Render(m_Graphics->getRenderer()->getContext())) {
		//OnDeviceLost();
	//}

	//if (!m_Shader->Render(m_Graphics->getRenderer()->getContext(), m_Model->getIndexCount(), 
		//m_Model->GetWorldMatrix(), m_view, m_projection, m_Model->getTexture())) {
		//OnDeviceLost();
	//}

	m_Terrain->Render(m_Graphics->getRenderer()->getContext(), m_Shader, m_view, m_projection);

	//if (!m_Model->Render(m_Graphics->getRenderer()->getContext())) {
		//OnDeviceLost();
	//}

	/*m_world = m_world.CreateFromYawPitchRoll(m_Model2->GetRotation().x, m_Model2->GetRotation().y, m_Model2->GetRotation().z)
		* SimpleMath::Matrix::CreateTranslation(m_Model2->GetPosition());

	if (!m_Shader->Render(m_Graphics->getRenderer()->getContext(), m_Model2->getIndexCount(), 
		m_Model2->GetWorldMatrix(), m_view, m_projection, m_Model2->getTexture())) {
		OnDeviceLost();
	}*/

	m_Graphics->get2DRenderer()->PrintFPS();


	if (!m_Graphics->EndScreen()) {
		OnDeviceLost();
	}

	//if (!m_Graphics->Render()) {
		//OnDeviceLost();
	//}
	/*
    Clear();

    // TODO: Add your rendering code here.
	m_shape1->Draw(m_world, m_view, m_projection, Colors::White);

	m_shape2->Draw(m_world2, m_view, m_projection, Colors::Black);


    Present();*/
}

// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int width, int height)
{
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

	m_Graphics->updateScreenSize(m_window, m_outputWidth, m_outputHeight);

	m_Camera->UpdateScreenSize(width, height);

    //CreateResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 1280;
    height = 720;
}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
    
	//m_shape1 = GeometricPrimitive::CreateSphere(m_d3dContext.Get());
	//m_shape2 = GeometricPrimitive::CreateSphere(m_d3dContext.Get());

	m_world = DirectX::SimpleMath::Matrix::Identity;
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
    // TODO: Initialize windows-size dependent objects here.
	//m_view = DirectX::SimpleMath::Matrix::CreateLookAt(DirectX::SimpleMath::Vector3(0.f, 0.f, 4.f),
		//DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::UnitY);
	//m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
		//float(1280) / float(720), 0.1f, 1000.f);

	m_view = XMMatrixLookAtLH(DirectX::SimpleMath::Vector3(0.f, 0.f, 4.f),
		DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::UnitY);

	m_view = m_Camera->GetViewMatrix();

	//m_projection = XMMatrixPerspectiveFovLH(XM_PI / 4.f, float(1280) / float(720), 0.1f, 1000.f);
	m_projection = m_Camera->GetProjectionMatrix();

}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
	m_Graphics.reset();

	m_Model->Reset();
	m_Model.reset();
	m_Shader->Reset();
	m_Shader.reset();

	m_Input.reset();
	m_Camera.reset();

    CreateDevice();

    CreateResources();
}