/*------------------------------
*	DirectX[direct3d.cpp]
*
* 制作者：山室飛龍		日付：2025/9/19
------------------------------*/
#include "direct3d.h"
#include "debug_ostream.h"
#include "sprite.h"
#pragma comment(lib,"d3d11.lib")

#if defined(DEBUG)|| defined(_DEBUG)
#pragma comment(lib, "DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DirectXTex_Release.lib")
#endif

/*------------------------------
* グローバル変数
------------------------------*/

bool Direct3D::Initialize(HWND hWnd)
{
	/* デバイス、スワップチェーン、コンテキスト生成 */
	DXGI_SWAP_CHAIN_DESC swap_chain_desc{};
	swap_chain_desc.Windowed = TRUE;	//
	swap_chain_desc.BufferCount = 2;	//バッファの数
	//swap_chain_desc.BufferDesc.Width = 0;
	//swap_chain_desc.BufferDesc.Height = 0;
	//→ウィンドウサイズに合わせて自動的に設定される
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	//swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;		//ベンチマーク用のやつ
	swap_chain_desc.OutputWindow = hWnd;

	UINT device_flags = 0;
#if defined(DEBUG)||defined(_DEBUG)
	device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_11_1,	//バージョン関係
		D3D_FEATURE_LEVEL_11_0
	};

	D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,	//ビデオアダプター(GPU)
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		device_flags,
		levels,
		ARRAYSIZE(levels),
		D3D11_SDK_VERSION,
		&swap_chain_desc,
		&m_pSwapChain,
		&m_pDevice,
		&feature_level,	//どっちのバージョンになったか確認
		&m_pDeviceContext);

	if (FAILED(hr))
	{
		MessageBox(hWnd, "Direct3Dの初期化に失敗しました", "エラー", MB_OK);
		return false;
	}

	if (!ConfigureBackBuffer())
	{
		MessageBox(hWnd, "バックバッファの設定に失敗しました", "エラー", MB_OK);
		return false;
	}


	/* ブレンドステート */
	D3D11_BLEND_DESC bd = {};
	bd.AlphaToCoverageEnable = FALSE;
	bd.IndependentBlendEnable = FALSE;
	bd.RenderTarget[0].BlendEnable = TRUE;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	m_pDevice->CreateBlendState(&bd, &m_BlendStateMultiply);	//作成

	bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

	m_pDevice->CreateBlendState(&bd, &m_BlendStateAdd);	//作成

	//Direct3D_SetAlphaBlend(BLEND_TRANSPARENT);

	//深度ステンシルステート設定（奥行きの設定をしない/重なる）
	D3D11_DEPTH_STENCIL_DESC dsd = {};
	dsd.DepthFunc = D3D11_COMPARISON_LESS;
	dsd.StencilEnable = FALSE;
	dsd.DepthEnable = FALSE;	//向こうにする
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	m_pDevice->CreateDepthStencilState(&dsd, &m_DepthStencilStateDepthDisable);
	m_pDeviceContext->OMSetDepthStencilState(m_DepthStencilStateDepthDisable, NULL);


	return true;
}

void Direct3D::Finalize()
{
	/* ブレンドステートのリリース */
	SafeRelease(m_DepthStencilStateDepthDisable);
	SafeRelease(m_BlendStateMultiply);
	SafeRelease(m_BlendStateAdd);

	/* バックバッファのリリース */
	ReleaseBackBuffer();

	/* 基盤のリリース */
	SafeRelease(m_pSwapChain);
	SafeRelease(m_pDeviceContext);
	SafeRelease(m_pDevice);
}

void Direct3D::Clear()
{
	float clear_color[4] = { 0.0f,0.4f,0.8f,1.0f };
	//描画命令の発行(特定の色でクリアしてね～)
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView,
		clear_color);
	//こっちもしないといけないよ(1.0fでクリアしてね～)～
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView,
		D3D11_CLEAR_DEPTH, 1.0f, 0);

	//レンダーターゲットビューとデプスステンシルビューの設定
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView,
		m_pDepthStencilView);
}

void Direct3D::Present()
{
	//スワップチェーンの表示
	m_pSwapChain->Present(1, 0);
}

void Direct3D::SetAlphaBlend(AlphaBlendMode blend)
{
	float blend_factor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11BlendState* arg = nullptr;
	switch (blend)
	{
	case BLEND_TRANSPARENT:
		arg = m_BlendStateMultiply;
		break;
	case BLEND_ADD:
		arg = m_BlendStateAdd;
		break;
	default:
		break;
	}
	m_pDeviceContext->OMSetBlendState(arg, blend_factor, 0xffffffff);//設定
}

unsigned int Direct3D::GetBackBufferWidth()
{
	return m_BackBufferDesc.Width;
}

unsigned int Direct3D::GetBackBufferHeight()
{
	return m_BackBufferDesc.Height;
}

ID3D11Device* Direct3D::GetDevice()
{
	return m_pDevice;
}

ID3D11DeviceContext* Direct3D::GetDeviceContext()
{
	return m_pDeviceContext;
}


bool Direct3D::ConfigureBackBuffer()
{
	HRESULT hr;

	ID3D11Texture2D* back_buffer_pointer = nullptr;

	// バックバッファの取得
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer_pointer);
	
	if (FAILED(hr)) {
		hal::dout << "バックバッファの取得に失敗しました" << std::endl;
		return false;
	}

	// バックバッファのレンダーターゲットビューの生成
	hr = m_pDevice->CreateRenderTargetView(back_buffer_pointer, nullptr, &m_pRenderTargetView);

	if (FAILED(hr)) {
		back_buffer_pointer->Release();
		hal::dout << "バックバッファのレンダーターゲットビューの生成に失敗しました" << std::endl;
		return false;
	}

	// バックバッファの状態（情報）を取得
	back_buffer_pointer->GetDesc(&m_BackBufferDesc);

	back_buffer_pointer->Release(); // バックバッファのポインタは不要なので解放

	/* デプスステンシルバッファの生成 */
	D3D11_TEXTURE2D_DESC depth_stencil_desc{};
	depth_stencil_desc.Width = m_BackBufferDesc.Width;
	depth_stencil_desc.Height = m_BackBufferDesc.Height;
	depth_stencil_desc.MipLevels = 1;
	depth_stencil_desc.ArraySize = 1;
	depth_stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_stencil_desc.SampleDesc.Count = 1;
	depth_stencil_desc.SampleDesc.Quality = 0;
	depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_stencil_desc.CPUAccessFlags = 0;
	depth_stencil_desc.MiscFlags = 0;
	hr = m_pDevice->CreateTexture2D(&depth_stencil_desc, nullptr, &m_pDepthStencilBuffer);

	if (FAILED(hr)) {
		hal::dout << "デプスステンシルバッファの生成に失敗しました" << std::endl;
		return false;
	}

	// デプスステンシルビューの生成
	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc{};
	depth_stencil_view_desc.Format = depth_stencil_desc.Format;
	depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_stencil_view_desc.Texture2D.MipSlice = 0;
	depth_stencil_view_desc.Flags = 0;
	hr = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depth_stencil_view_desc, &m_pDepthStencilView);

	if (FAILED(hr)) {
		hal::dout << "デプスステンシルビューの生成に失敗しました" << std::endl;
		return false;
	}

	//ビューポートの設定
	m_Viewport[0].TopLeftX = 0.0f;
	m_Viewport[0].TopLeftY = 0.0f;
	m_Viewport[0].Width = (FLOAT)m_BackBufferDesc.Width;
	m_Viewport[0].Height = (FLOAT)m_BackBufferDesc.Height;
	m_Viewport[0].MinDepth = 0.0f;
	m_Viewport[0].MaxDepth = 1.0f;

	m_pDeviceContext->RSSetViewports(1, &m_Viewport[0]);	//ビューポートの

	return true;
}

void Direct3D::ReleaseBackBuffer()
{
	/* バックバッファのリリース */
	SafeRelease(m_pRenderTargetView);
	SafeRelease(m_pDepthStencilView);
	SafeRelease(m_pDepthStencilBuffer);
}
