/*------------------------------
*	Sprite処理[sprite.h]
*
* 制作者：山室飛龍		日付：2025/9/20
------------------------------*/

#ifndef SPRITE_H
#define SPRITE_H

#include <d3d11.h>
#include <DirectXMath.h>
#include "DirectXTex.h"
#include <math.h>
#include "shader.h"
#include "direct3d.h"
#include "texture.h"

class Sprite
{
private:

	/* 頂点バッファ */
	ID3D11Buffer* m_pVertexBuffer = nullptr; 

	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

	/* 共有リソースの参照 */
	Shader* m_pShader = nullptr;
	Texture* m_pTexture = nullptr;
	float m_ScreenWidth = 1600;
	float m_ScreenHeight = 900;

public:
	void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	Shader* pShader, Texture* pTexture);
	void Finalize(void);

	void BeginDraw();

	void Draw(int texId, float x, float y, DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f, });
	void Draw(int texId, float x, float y,
		float width, float height,
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f});

	void Draw(int texId, float x, float y,
		float width, float height,
		float tx, float ty, float tw, float th,
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f});

	Sprite* GetSprite();

};
#endif // !SPRITE_H
