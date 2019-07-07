#ifndef _MAIN_SCREEN_H_
#define _MAIN_SCREEN_H_

#include "../../Engine/Abstract/AbstractScreen.h"

#include "../../Engine/Camera.h"
#include "../../Engine/Input.h"
#include "../../Engine/Shader.h"
#include "../../Engine/Light.h"
#include "../ChunkTerrain.h"
#include "../../Engine/Bitmap.h"
#include "../../Engine/RenderTexture.h"
#include "../../Engine/DebugWindow.h"

#pragma once
class MainScreen : public AbstractScreen
{
public:
	MainScreen();
	~MainScreen();

	void Load(HWND window);
	void UnLoad();

	bool Update(DX::StepTimer const& timer);
	bool Render();

	void UpdateScreenSize(int width, int height);

private:
	bool RenderToTexture();
	bool RenderScene();

private:
	std::shared_ptr<Shader>							m_Shader;
	std::shared_ptr<Camera>							m_Camera;
	std::unique_ptr<Light>							m_Light;
	std::unique_ptr<ChunkTerrain>					m_Terrain;
	std::unique_ptr<Bitmap>							m_Bitmap;
	std::unique_ptr<RenderTexture>					m_RenderTexture;
	std::unique_ptr<DebugWindow>					m_DebugWindow;
};

#endif // !_MAIN_SCREEN_H_
