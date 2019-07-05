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

	m_Graphics = std::make_shared<Graphics>();

	if (!m_Graphics->Initialize(window, width, height)) {
		OnDeviceLost();
	}

	m_Input = std::make_shared<Input>();

	m_Input->Initialize(window);

	m_GameState = std::make_unique<GameState>();

	if (!m_GameState->Initialize(m_Graphics, window, m_Input)) {
		OnDeviceLost();
	}

	m_GameState->Start();

	// minden kuka

    //CreateDevice();

    //CreateResources();

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
	m_GameState->Update(timer);
}

// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

	m_GameState->Render();

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

	m_GameState->UpdateScreenSize(width, height);

	//m_Camera->UpdateScreenSize(width, height);

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
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
    // TODO: Initialize windows-size dependent objects here.
	//m_view = DirectX::SimpleMath::Matrix::CreateLookAt(DirectX::SimpleMath::Vector3(0.f, 0.f, 4.f),
		//DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::UnitY);
	//m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
		//float(1280) / float(720), 0.1f, 1000.f);

	//m_view = m_Camera->GetViewMatrix();

	//m_projection = XMMatrixPerspectiveFovLH(XM_PI / 4.f, float(1280) / float(720), 0.1f, 1000.f);
	//m_projection = m_Camera->GetProjectionMatrix();

}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
	m_Graphics.reset();
	m_GameState.reset();
	m_Input.reset();

}