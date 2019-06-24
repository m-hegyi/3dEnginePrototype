#ifndef _BASE_FONT_H_
#define _BASE_FONT_H_

#include <d3d11.h>
#include <SimpleMath.h>

#pragma once
class BaseFont
{
public: 
	BaseFont();
	~BaseFont();

	void Initialize(DirectX::SimpleMath::Vector2 position);

	void Update(DirectX::SimpleMath::Vector2 position);

	bool Render(wchar_t* text);

private:
	// color, position
	DirectX::SimpleMath::Vector2 m_position;
};

#endif