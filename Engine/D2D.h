#ifndef _D2D_H_
#define _D2D_H_

#include <wrl/client.h>

#include <shtypes.h>
#include <d2d1_2.h>
#include <dwrite_2.h>
#include <sstream>

#include "D3D.h"

#pragma comment (lib, "d2d1.lib")
#pragma comment (lib, "dwrite.lib")

#pragma once
class D2D
{
public:
	D2D();
	~D2D();

	bool Initialize(D3D* direct3D);

	void test(WCHAR* string, UINT32 length);

	void PrintFPS();

private:
	bool CreateDevice(D3D* direct3D);
	bool CreateContext(D3D* direct3D);
	bool InitializeTextFormats();

private:

	// brushes
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> yellowBrush;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> whiteBrush;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> blackBrush;


	// text formats
	Microsoft::WRL::ComPtr<IDWriteTextFormat>	m_textFormatFPS;

	// text layouts
	Microsoft::WRL::ComPtr<IDWriteTextLayout>	m_textLayoutFPS;

	Microsoft::WRL::ComPtr<IDWriteFactory2>		m_writeFactory;
	Microsoft::WRL::ComPtr<ID2D1Factory2>		m_factory;
	Microsoft::WRL::ComPtr<ID2D1Device1>		m_d2dDevice;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext1> m_d2dContext;
};


#endif
