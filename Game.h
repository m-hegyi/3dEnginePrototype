//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
#include "SpriteBatch.h"
#include "SimpleMath.h"
#include "Effects.h"
#include "VertexTypes.h"
#include "SpriteFont.h"
#include "CommonStates.h"
#include "PrimitiveBatch.h"
#include "GeometricPrimitive.h"


#include "Engine/Graphics.h"
#include "Engine/Model.h"
#include "Engine/Shader.h"

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:

    Game() noexcept;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

private:

    void Update(DX::StepTimer const& timer);
    void Render();


    void CreateDevice();
    void CreateResources();

    void OnDeviceLost();

    // Device resources.
    HWND                                            m_window;
    int                                             m_outputWidth;
    int                                             m_outputHeight;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;

	std::unique_ptr<Graphics>						m_Graphics;
	std::unique_ptr<Model>							m_Model;
	std::unique_ptr<Shader>							m_Shader;

	DirectX::SimpleMath::Matrix						m_world;
	DirectX::SimpleMath::Matrix						m_world2;
	DirectX::SimpleMath::Matrix						m_view;
	DirectX::SimpleMath::Matrix						m_projection;

	std::unique_ptr<DirectX::GeometricPrimitive>	m_shape1;
	std::unique_ptr<DirectX::GeometricPrimitive>	m_shape2;
};