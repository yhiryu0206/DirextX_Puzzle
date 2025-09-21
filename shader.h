/*------------------------------
*	シェーダー処理[shader.h]
*
* 制作者：山室飛龍		日付：2025/9/21
------------------------------*/
#ifndef SHADER_H
#define	SHADER_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
class Shader
{
private:
	/* 頂点シェーダー */
	ID3D11VertexShader* m_pVertexShader = nullptr;

	/* 頂点シェーダー用 定数バッファ0（座標変換行列など） */
	ID3D11Buffer* m_pVSConstantBuffer0 = nullptr;

	/* 頂点シェーダー用 定数バッファ1（追加の情報用、例：色や補助データ） */
	ID3D11Buffer* m_pVSConstantBuffer1 = nullptr;

	/* 頂点入力レイアウト（頂点データの構造を GPU に伝えるためのもの） */
	ID3D11InputLayout* m_pInputLayout = nullptr;

	/* ピクセルシェーダー（描画処理で実際に色を決定する部分） */
	ID3D11PixelShader* m_pPixelShader = nullptr;

	/* ピクセルシェーダー用 定数バッファ（描画時の色やライティング情報） */
	ID3D11Buffer* m_pPSConstantBuffer = nullptr;

	/* サンプラーステート（テクスチャをどう補間／フィルタリングするかを定義） */
	ID3D11SamplerState* m_pSamplerState = nullptr;

	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

	/* コンストラクタを private にする（外部から new できないようにする）*/
	Shader() = default;

public:

	// コピー禁止（シングルトンの鉄則）
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	static Shader* GetShader();  // シングルトン取得

	bool Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Finalize();

	/* 行列や色の設定（GPU の定数バッファへ転送） */
	void SetProjectionMatrix(const DirectX::XMMATRIX& matrix);
	void SetWorldMatrix(const DirectX::XMMATRIX& matrix);
	void SetColor(const DirectX::XMFLOAT4& color);

	/* シェーダーを描画パイプラインにセット */
	void Begin();

};

#endif // SHADER_H
