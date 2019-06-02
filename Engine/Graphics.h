#ifndef _GRAPHICS_H_
#define _GRAHPICS_H_

#include <Windows.h>
#include <memory>
#include "D3D.h"

#pragma once
class Graphics
{
public:
	Graphics();
	~Graphics();

	bool Initialize(HWND hwnd, int outputWidt, int outputHeight);

	bool BeginScreen();
	bool EndScreen();

	void reset();

	D3D* getRenderer() const { return m_D3D.get(); };

private:
	std::unique_ptr<D3D> m_D3D;
};

#endif
