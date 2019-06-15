#ifndef _D3D_H_
#define _D3D_H_

#include <wrl\client.h>
#include <memory>
#include <Windows.h>
#include <stdio.h>
#include <d3d11.h>
#include <CommonStates.h>


#pragma once
class D3D
{
public:
	D3D();
	~D3D();

	bool Initialize(HWND hwnd, int outputWidth, int outputHeight);

	bool Present();
	bool Clear();

	void Reset();

	ID3D11Device* getDevice() const { return m_d3dDevice.Get(); };

	ID3D11DeviceContext* getContext() const { return m_d3dContext.Get(); };

private:

	void CreateDevice();
	bool CreateContext(HWND hwnd, int outputWidth, int outputHeight);

	void CreateDepthStencilBuffer(ID3D11Device* device);
	void CreateDepthStencilState(ID3D11Device* device);
	void CreateDepthStencilView(ID3D11Device* device);
	void CreateRasterState(ID3D11Device* device);
	void SetViewport();
	
private:
	int m_outputWidth;
	int m_outputHeight;

	D3D_FEATURE_LEVEL								m_featureLevel;
	Microsoft::WRL::ComPtr<ID3D11Device>			m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_d3dContext;


	Microsoft::WRL::ComPtr<IDXGISwapChain1>			m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	m_rasterState;
};


#endif
