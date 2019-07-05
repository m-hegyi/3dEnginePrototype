#ifndef _GRAPHICS_H_
#define _GRAHPICS_H_

#include <Windows.h>
#include <memory>
#include "D3D.h"
#include "D2D.h"
#include "Frustum.h"

#pragma once
class Graphics
{
public:
	Graphics();
	~Graphics();

	bool Initialize(HWND hwnd, int outputWidth, int outputHeight);

	bool BeginScreen();
	bool EndScreen();

	void updateScreenSize(HWND hwnd, int outputWidth, int heioutputHeightght);

	void reset();

	D3D* getRenderer() const { return m_D3D.get(); };

	D2D* get2DRenderer() const { return m_D2D.get(); };

	Frustum* getFrustum() const { return m_Frustum.get(); };

private:
	std::unique_ptr<D3D>		m_D3D;
	std::unique_ptr<D2D>		m_D2D;

	std::unique_ptr<Frustum>	m_Frustum;
};

#endif
