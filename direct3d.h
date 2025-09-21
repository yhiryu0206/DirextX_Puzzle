/*------------------------------
*	DirectX[direct3d.h]
*
* 制作者：山室飛龍		日付：2025/9/19
------------------------------*/
#ifndef DIRECT3D_H
#define DIRECT3D_H

#include <d3d11.h>
#include <windows.h>

/* Releaseのtemplate */
template<class T>
inline void SafeRelease(T*& p)
{
	if (p)
	{
		p->Release();
		p = nullptr;
	}
}
enum AlphaBlendMode
{
	BLEND_TRANSPARENT,
	BLEND_ADD,
	BLEND_MAX,
};

class Direct3D
{
public:

	/* 基盤 */
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;

	/* ブレンドステート系 */
	ID3D11BlendState* m_BlendStateMultiply = nullptr;
	ID3D11BlendState* m_BlendStateAdd = nullptr;
	ID3D11DepthStencilState* m_DepthStencilStateDepthDisable = nullptr;

	/* バックバッファ系 */
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
	ID3D11Texture2D* m_pDepthStencilBuffer = nullptr;
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;
	D3D11_TEXTURE2D_DESC m_BackBufferDesc{};	//作ったバックバッファの情報を突っ込んでる
	D3D11_VIEWPORT m_Viewport[1] = { {} };

	bool Initialize(HWND wnd);	//Direct
	void Finalize();
	
	void Clear();	//バックバッファのクリア
	void Present();	//バックバッファの表示

	void SetAlphaBlend(AlphaBlendMode blend);

	unsigned int GetBackBufferWidth();
	unsigned int GetBackBufferHeight();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	bool ConfigureBackBuffer();	//バックバッファの設定・生成
	void ReleaseBackBuffer();
};
#endif // !DIRECT3D_H
