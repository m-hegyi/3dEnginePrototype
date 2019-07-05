#ifndef _LOADING_SCREEN_H_
#define _LOADING_SCREEN_H_

#include "../../Engine/Abstract/AbstractScreen.h"

#pragma once
class LoadingScreen : public AbstractScreen
{
public:
	LoadingScreen();
	~LoadingScreen();

	void Load(HWND window);
	void UnLoad();

	bool Render();
	bool Update(DX::StepTimer const& timer);

	void UpdateScreenSize(int width, int height) {};

private:
	float countdown;
};

#endif // !_LOADING_SCREEN_H_
