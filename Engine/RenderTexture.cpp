#include "RenderTexture.h"

RenderTexture::RenderTexture(std::shared_ptr<Graphics> graphics) : m_Graphics(graphics)
{

}

RenderTexture::~RenderTexture()
{
	if (m_shaderResourceView) {
		m_shaderResourceView->Release();
		m_shaderResourceView = 0;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}

	if (m_renderTargetTexture)
	{
		m_renderTargetTexture->Release();
		m_renderTargetTexture = 0;
	}
}

bool RenderTexture::Initialize(int textureWidth, int textureHeight)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target view desc
	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	auto device = m_Graphics->getRenderer()->getDevice();

	result = device->CreateTexture2D(&textureDesc, NULL, &m_renderTargetTexture);
	if (FAILED(result)) 
	{
		return false;
	}

	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	result = device->CreateRenderTargetView(m_renderTargetTexture, &renderTargetViewDesc, &m_renderTargetView);
	if (FAILED(result)) 
	{
		return false;
	}

	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	result = device->CreateShaderResourceView(m_renderTargetTexture, &shaderResourceViewDesc, &m_shaderResourceView);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void RenderTexture::SetRenderTarget()
{
	m_Graphics->getRenderer()->getContext()->OMSetRenderTargets(1, &m_renderTargetView, m_Graphics->getRenderer()->GetDepthStencilView());
}

void RenderTexture::ClearRenderTarget(float red, float green, float blue, float alpha)
{
	auto deviceContext = m_Graphics->getRenderer()->getContext();

	float color[4];


	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	// Clear the depth buffer.
	deviceContext->ClearDepthStencilView(m_Graphics->getRenderer()->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

}
