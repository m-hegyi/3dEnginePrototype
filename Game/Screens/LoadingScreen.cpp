#include "LoadingScreen.h"

LoadingScreen::LoadingScreen()
{
}

LoadingScreen::~LoadingScreen()
{
}

void LoadingScreen::Load(HWND window)
{
}

void LoadingScreen::UnLoad()
{
}

bool LoadingScreen::Render()
{

	m_Graphics->BeginScreen();

	m_Graphics->get2DRenderer()->PrintFPS();

	m_Graphics->EndScreen();

	return true;
}

bool LoadingScreen::Update(DX::StepTimer const& timer)
{
	float elapsedTime = timer.GetTotalSeconds();

	std::wostringstream stringStream;

	stringStream.precision(4);

	stringStream << "Loading..." << std::endl;

	stringStream << elapsedTime << std::endl;

	m_Graphics->get2DRenderer()->test((WCHAR*)stringStream.str().c_str(), (UINT32)stringStream.str().size());

	return true;
}