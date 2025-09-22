/*------------------------------
*	Sprite処理[sprite.cpp]
*
* 制作者：山室飛龍		日付：2025/9/20
------------------------------*/
#include "direct3d.h"
#include "shader.h"
#include "debug_ostream.h"
#include "sprite.h"
#include "texture.h"
using namespace DirectX;

static constexpr int NUM_VERTEX = 4; // 頂点数

/* 頂点構造体 */
struct Vertex
{
	XMFLOAT3 position; // 頂点座標
	XMFLOAT4 color;    // 頂点カラー
	XMFLOAT2 texCoord; // UV座標
};

void Sprite::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	Shader* pShader, Texture* pTexture)
{
	Direct3D pDirect3D;
	// デバイスとデバイスコンテキストのチェック
	if (!pDevice || !pContext) {
		hal::dout << "Polygon_Initialize() : 与えられたデバイスかコンテキストが不正です" << std::endl;
		return;
	}

	/* メンバ変数へ保持 */
	m_pDevice = pDevice;
	m_pContext = pContext;
	m_pShader = pShader;
	m_pTexture = pTexture;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Vertex) * NUM_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	m_pDevice->CreateBuffer(&bd, NULL, &m_pVertexBuffer);
}

void Sprite::Finalize()
{
	/* 頂点バッファ解放 */
	SafeRelease(m_pVertexBuffer);
}

void Sprite::BeginDraw()
{
	/* シェーダー開始 */
	m_pShader->Begin();
	/* ワールド行列を単位行列に */
	m_pShader->SetWorldMatrix(XMMatrixIdentity());
	/* 正射影行列を設定（左上原点の2D座標系） */
	m_pShader->SetProjectionMatrix(
		XMMatrixOrthographicOffCenterLH(
			0.0f, m_ScreenWidth,
			m_ScreenHeight, 0.0f,
			0.0f, 1.0f
		)
	);
	/* 三角形ストリップで描画 */
	m_pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Sprite::Draw(int texId, float x, float y, float width, float height, DirectX::XMFLOAT4 color)
{
	/* 描画準備 */
	BeginDraw();

	/* 頂点バッファをCPU側から更新できるようにする */
	D3D11_MAPPED_SUBRESOURCE msr;
	m_pContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	Vertex* v = (Vertex*)msr.pData;

	float left = x;
	float right = x + width;
	float top = y;
	float bottom = y + height;

	v[0] = { {left,  bottom, 0.0f}, {color}, {0.0f, 1.0f} };
	v[2] = { {right, bottom, 0.0f}, {color}, {1.0f, 1.0f} };
	v[3] = { {right, top,    0.0f}, {color}, {1.0f, 0.0f} };
	v[1] = { {left,  top,    0.0f}, {color}, {0.0f, 0.0f} };

	/* アンマップ */
	m_pContext->Unmap(m_pVertexBuffer, 0);

	/* 頂点バッファを設定 */
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	/* ワールド・射影行列再設定（2D座標系） */
	m_pShader->SetWorldMatrix(XMMatrixIdentity());
	m_pShader->SetProjectionMatrix(
		XMMatrixOrthographicOffCenterLH(
			0.0f, m_ScreenWidth,
			m_ScreenHeight, 0.0f,
			0.0f, 1.0f
		)
	);

	/* 色とテクスチャ設定 */
	m_pShader->SetColor(color);
	m_pTexture->SetTexture(texId);

	/* 描画実行 */
	m_pContext->Draw(NUM_VERTEX, 0);
}

void Sprite::Draw(int texId, float x, float y, float width, float height, float tx, float ty, float tw, float th, DirectX::XMFLOAT4 color)
{
	/* 描画準備 */
	BeginDraw();

	/* 頂点バッファをCPUから更新可能にする */
	D3D11_MAPPED_SUBRESOURCE msr;
	m_pContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	Vertex* v = (Vertex*)msr.pData;

	/* テクスチャ情報取得 */
	float tsw = static_cast<float>(m_pTexture->GetWidth(texId));
	float tsh = static_cast<float>(m_pTexture->GetHeight(texId));

	/* 頂点座標 */
	float left = x;
	float right = x + width;
	float top = y;
	float bottom = y + height;

	/* UV座標 */
	float u0 = tx / tsw;
	float v0 = ty / tsh;
	float u1 = (tx + tw) / tsw;
	float v1 = (ty + th) / tsh;

	v[0] = { {left,  bottom, 0.0f}, color, {u0, v1} };
	v[1] = { {left,  top,    0.0f}, color, {u0, v0} };
	v[2] = { {right, bottom, 0.0f}, color, {u1, v1} };
	v[3] = { {right, top,    0.0f}, color, {u1, v0} };

	/* アンマップ */
	m_pContext->Unmap(m_pVertexBuffer, 0);

	/* 頂点バッファを設定 */
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	/* ワールド・射影行列再設定（2D座標系）*/
	m_pShader->SetWorldMatrix(XMMatrixIdentity());
	m_pShader->SetProjectionMatrix(
		XMMatrixOrthographicOffCenterLH(
			0.0f, m_ScreenWidth,
			m_ScreenHeight, 0.0f,
			0.0f, 1.0f
		)
	);

	/* 色とテクスチャ設定 */
	m_pShader->SetColor(color);
	m_pTexture->SetTexture(texId);

	/* 描画実行 */
	m_pContext->Draw(NUM_VERTEX, 0);
}

Sprite* Sprite::GetSprite()
{
	static Sprite instance;   // Meyers Singleton
	return &instance;
}
