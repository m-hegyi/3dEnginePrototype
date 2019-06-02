#include "../pch.h"
#include "Graphics.h"

Graphics::Graphics()
{
	m_D3D = std::make_unique<D3D>();
}

Graphics::~Graphics()
{
}

bool Graphics::Initialize(HWND hwnd, int outputWidth, int outputHeight)
{
	if (!m_D3D->Initialize(hwnd, outputWidth, outputHeight)) {
		return false;
	}
	
	return true;
}

bool Graphics::BeginScreen()
{
	if (!m_D3D->Clear()) {
		return false;
	}

	return true;
}

bool Graphics::EndScreen()
{
	if (!m_D3D->Present()) {
		return false;
	}

	return true;
}

void Graphics::reset()
{
	m_D3D.reset();
}