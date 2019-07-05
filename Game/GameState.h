#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

#include <memory>


#include "../Engine/Abstract/AbstractState.h"

#include "../Engine/Graphics.h"
#include "../Engine/Input.h"
#include "Screens/LoadingScreen.h"
#include "Screens/MainScreen.h"

#pragma once
class GameState: public AbstractState
{
public:
	GameState();
	~GameState();

	bool Initialize(std::shared_ptr<Graphics> graphics, HWND window, std::shared_ptr<Input> input);

	void Start();

	bool Update(DX::StepTimer const& timer);
	bool Render();

	void UpdateScreenSize(int width, int height);

private:

	void LoadScreen(int screenIndex);

private:
	std::shared_ptr<Graphics>	m_Graphics;
	std::shared_ptr<Input>		m_Input;
	HWND						m_window;

	float						counter;
};


#endif // !_GAME_STATE_H_
