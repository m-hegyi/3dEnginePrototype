#include "../pch.h"
#include "Graphics.h"

Graphics::Graphics()
{
	m_D3D = std::make_unique<D3D>();
	m_D2D = std::make_unique<D2D>();
	m_Frustum = std::make_unique<Frustum>();
}

Graphics::~Graphics()
{
}

bool Graphics::Initialize(HWND hwnd, int outputWidth, int outputHeight)
{
	if (!m_D3D->Initialize(hwnd, outputWidth, outputHeight)) {
		return false;
	}

	if (!m_D2D->Initialize(m_D3D.get(), outputWidth, outputHeight)) {
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

void Graphics::updateScreenSize(HWND hwnd, int outputWidth, int outputHeight)
{
	m_D2D->beforeUpdateScreenSize();
	m_D3D->updateScreenSize(hwnd, outputWidth, outputHeight);
	m_D2D->updateScreenSize(outputWidth, outputHeight, m_D3D.get());
}

void Graphics::reset()
{
	m_D3D.reset();
}