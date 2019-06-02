//
// Game.cpp
//

#include "pch.h"
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

	m_Model = std::make_unique<Model>();

	if (!m_Model->Initialize(m_Graphics->getRenderer()->getDevice(), "Data/cube.txt", L"Assets/seafloor.dds")) {
		OnDeviceLost();
	}

	m_Shader = std::make_unique<Shader>();

	if (!m_Shader->Initialize(m_Graphics->getRenderer()->getDevice(), window)) {
		OnDeviceLost();
	}

    CreateDevice();

    CreateResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */
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

    // TODO: Add your game logic here.

	//m_Model->SetRotation(time * 1.f, 0.0f, 0.0f);
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

	if (!m_Graphics->BeginScreen()) {
		OnDeviceLost();
	}


	if (!m_Model->Render(m_Graphics->getRenderer()->getContext())) {
		OnDeviceLost();
	}

	m_world = m_world.CreateTranslation(m_Model->GetPosition());
	m_world = m_world.CreateFromYawPitchRoll(m_Model->GetRotation().x, m_Model->GetRotation().y, m_Model->GetRotation().z);

	if (!m_Shader->Render(m_Graphics->getRenderer()->getContext(), m_Model->getIndexCount(), m_world, m_view, m_projection, m_Model->getTexture())) {
		OnDeviceLost();
	}


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
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(DirectX::SimpleMath::Vector3(0.f, 0.f, 4.f),
		DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::UnitY);
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
		float(1280) / float(720), 0.1f, 1000.f);
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
	m_Graphics.reset();

	m_Model->Reset();
	m_Model.reset();
	m_Shader->Reset();
	m_Shader.reset();

    CreateDevice();

    CreateResources();
}