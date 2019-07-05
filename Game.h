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
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Engine/Light.h"

#include "Game/GameState.h"

#include "Game/ChunkTerrain.h"

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
	Input* GetInput() const { return m_Input.get(); }

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


	std::unique_ptr<GameState>						m_GameState;
	std::shared_ptr<Graphics>						m_Graphics;
	std::shared_ptr<Input>							m_Input;
};