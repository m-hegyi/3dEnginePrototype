#ifndef _ABSTRACT_SCREEN_H_
#define _ABSTRACT_SCREEN_H_

#include "../Graphics.h"
#include "../Input.h"
#include "../../StepTimer.h"

#pragma once
class AbstractScreen
{
public:
	
	void Initialize(std::shared_ptr<Graphics> graphics, std::shared_ptr<Input> input) { m_Graphics = graphics; m_Input = input; };

	virtual void Load(HWND hwnd) = 0;
	virtual void UnLoad() = 0;

	virtual bool Render() = 0;
	virtual bool Update(DX::StepTimer const& timer) = 0;

	virtual void UpdateScreenSize(int width, int height) = 0;

protected:

	std::shared_ptr<Graphics>	m_Graphics;
	std::shared_ptr<Input>		m_Input;

};

#endif // !_ABSTRACT_SCREEN_H_
