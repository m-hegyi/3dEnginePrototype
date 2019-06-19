#ifndef _INPUT_H_
#define _INPUT_H_

#include <Mouse.h>
#include <Keyboard.h>
#include <memory>

#include "Camera.h"

#pragma once
class Input
{
public:
	Input();
	~Input();

	void Initialize(std::shared_ptr<Camera> camera, HWND hwnd);

	void Update(float elapsedTime);

	DirectX::Keyboard*	GetKeyboard() const { return m_keyboard.get(); }
	DirectX::Mouse*		GetMouse() const { return m_mouse.get(); }

private:
	std::unique_ptr<DirectX::Keyboard>	m_keyboard;
	std::unique_ptr<DirectX::Mouse>		m_mouse;

	std::shared_ptr<Camera>				m_Camera;
};

#endif
