#include "D2D.h"

D2D::D2D()
{
}

D2D::~D2D()
{
}

bool D2D::Initialize(D3D* direct3D)
{
	if (!CreateDevice(direct3D)) {
		return false;
	}

	if (!CreateContext(direct3D)) {
		return false;
	}

	if (!InitializeTextFormats()) {
		return false;
	}

	return true;
}

void D2D::test(WCHAR* string, UINT32 length)
{
	m_writeFactory->CreateTextLayout(string, length, m_textFormatFPS.Get(),
		(float)1280, (float)720, m_textLayoutFPS.ReleaseAndGetAddressOf());
}

void D2D::PrintFPS()
{
	if (m_textLayoutFPS) {

		m_d2dContext->BeginDraw();

		m_d2dContext->DrawTextLayout(D2D1::Point2F(2.0f, 5.0f), m_textLayoutFPS.Get(), yellowBrush.Get());
		if (FAILED(m_d2dContext->EndDraw())) {
		}
	}
}

bool D2D::CreateDevice(D3D* direct3D)
{
	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &m_writeFactory))) {
		return false;
	}

	D2D1_FACTORY_OPTIONS options;
#ifndef NDEBUG
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#else
	options.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#endif

	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, __uuidof(ID2D1Factory2), &options, &m_factory))) {
		return false;
	}

	Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
	
	// create the Direct2D device
	if (FAILED(direct3D->getDevice()->QueryInterface(__uuidof(IDXGIDevice), &dxgiDevice))) {
		return false;
	}

	HRESULT hr;

	hr = m_factory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice);

	if (FAILED(hr)) {
		return false;
	}

	// create its context
	if (FAILED(m_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &m_d2dContext))) {
		return false;
	}

	return true;
}

bool D2D::CreateContext(D3D* direct3D)
{
	// specify the desired bitmap properties
	D2D1_BITMAP_PROPERTIES1 bp;
	bp.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	bp.dpiX = 96.0f;
	bp.dpiY = 96.0f;
	bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bp.colorContext = nullptr;

	// Direct2D needs the DXGI version of the back buffer
	Microsoft::WRL::ComPtr<IDXGISurface> dxgiBuffer;
	if (FAILED(direct3D->getSwapChain()->GetBuffer(0, __uuidof(IDXGISurface), &dxgiBuffer))) {
		return false;
	}

	// create the bitmap
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> targetBitmap;
	if (FAILED(m_d2dContext->CreateBitmapFromDxgiSurface(dxgiBuffer.Get(), &bp, &targetBitmap))) {
		return false;
	}
	// set the newly created bitmap as render target
	m_d2dContext->SetTarget(targetBitmap.Get());

	return true;
}

bool D2D::InitializeTextFormats()
{
	// create standard brushes
	if (FAILED(m_d2dContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &yellowBrush))) {
		return false;
	}
	if (FAILED(m_d2dContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &blackBrush))) {
		return false;
	}
	if (FAILED(m_d2dContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &whiteBrush))) {
		return false;
	}

	// set up text formats

	// FPS text
	if (FAILED(m_writeFactory.Get()->CreateTextFormat(L"Lucida Console", nullptr, 
		DWRITE_FONT_WEIGHT_LIGHT, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 18.0f, L"hu-HU", &m_textFormatFPS))) {
		return false;
	}
	if (FAILED(m_textFormatFPS->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING))) {
		return false;
	}
	if (FAILED(m_textFormatFPS->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR))) {
		return false;
	}

	return true;
}
