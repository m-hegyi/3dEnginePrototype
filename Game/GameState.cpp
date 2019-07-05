#include "GameState.h"

GameState::GameState()
{
}

GameState::~GameState()
{
}

bool GameState::Initialize(std::shared_ptr<Graphics> graphics, HWND window, std::shared_ptr<Input> input)
{
	m_Graphics = graphics;
	m_window = window;
	m_Input = input;
	m_activeScreenIndex = 0;

	// init screens
	std::shared_ptr<LoadingScreen> loadingScreen = std::make_shared<LoadingScreen>();
	std::shared_ptr<MainScreen> mainScreen = std::make_shared<MainScreen>();

	loadingScreen->Initialize(graphics, input);
	mainScreen->Initialize(graphics, input);

	m_screenList.push_back(loadingScreen);
	m_screenList.push_back(mainScreen);

	return true;
}

void GameState::Start()
{
	m_screenList[m_activeScreenIndex]->Load(m_window);
	// init first screen
}

bool GameState::Update(DX::StepTimer const& timer)
{
	float elapsedTime = timer.GetTotalSeconds();

	if (elapsedTime > 2.f) {
		LoadScreen(1);
	}


	if (!m_screenList[m_activeScreenIndex]->Update(timer)) {
		return false;
	}

	return true;
}

bool GameState::Render()
{
	if (!m_screenList[m_activeScreenIndex]->Render()) {
		return false;
	}

	return true;
}

void GameState::UpdateScreenSize(int width, int height)
{
	m_screenList[m_activeScreenIndex]->UpdateScreenSize(width, height);
}

void GameState::LoadScreen(int screenIndex)
{
	if (m_activeScreenIndex == screenIndex) {
		return;
	}

	m_screenList[screenIndex]->Load(m_window);

	m_activeScreenIndex = screenIndex;
}
