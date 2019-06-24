#include "BaseFont.h"

using namespace DirectX::SimpleMath;

BaseFont::BaseFont()
{
}

BaseFont::~BaseFont()
{
}

void BaseFont::Initialize(Vector2 position)
{
	m_position = position;
}

void BaseFont::Update(Vector2 position )
{
	//m_position = position;
}

bool BaseFont::Render(wchar_t * text)
{
	return false;
}
