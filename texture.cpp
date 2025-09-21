/*------------------------------
* テクスチャの管理[texture.cpp]
*
* 制作者：山室飛龍		日付：2025/9/20
------------------------------*/
#include "texture.h"
#include "direct3d.h"
#include "DirectXTex.h"
using namespace DirectX;

static constexpr int TEXTURE_MAX = 100;

struct TextureData
{
	std::wstring fileName;	//2重読み込み防止
	unsigned int width = 0;
	unsigned int height = 0;
	ID3D11ShaderResourceView* pTexture = nullptr;	//テクスチャ
};

static TextureData g_Texture[TEXTURE_MAX];

void Texture::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
}

void Texture::Finalize()
{
	AllRelease();
}

int Texture::Load(const wchar_t* pFilename, bool bMipMap)
{
	// 既に読み込んでいるファイルかどうか確認
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (!g_Texture[i].pTexture) continue;

		if (g_Texture[i].fileName == pFilename)
		{
			return i;	// すでに存在する → そのIDを返す
		}
	}
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		//空いている場所を探す
		if (g_Texture[i].pTexture)
		{
			continue;
		}

		//テクスチャのファイルからの読み込み
		TexMetadata metadata;
		ScratchImage image;
		//
		LoadFromWICFile(pFilename, WIC_FLAGS_NONE, &metadata, image);

		//画像ファイルのサイズ取得
		g_Texture[i].width = (unsigned int)metadata.width;
		g_Texture[i].height = (unsigned int)metadata.height;

		if (bMipMap)
		{
			// ミップマップ生成
			ScratchImage mipChain;
			GenerateMipMaps(image.GetImages(), image.GetImageCount(),
				image.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);
			image = std::move(mipChain);
			metadata = image.GetMetadata();
		}

		//シェーダーリソースビューの生成
		HRESULT hr = CreateShaderResourceView(m_pDevice, image.GetImages(),
			image.GetImageCount(), metadata, &g_Texture[i].pTexture);

		if (FAILED(hr))
		{
			MessageBox(nullptr, "テクスチャの読み込みにしっぱしました", "エラー", MB_OK);
			break;
		}

		g_Texture[i].fileName = pFilename;	// ファイル名を保存（重複読み込み防止用）

		return i;	// スロット番号を返す
	}

	return -1;	// 空きがない or 失敗
}

/* 全テクスチャ解放 */
void Texture::AllRelease()
{
	for (TextureData& t : g_Texture)
	{
		SAFE_RELEASE(t.pTexture);
	}
}

/* 指定したテクスチャをシェーダにセット */
void Texture::SetTexture(int textureID)
{
	if (textureID < 0)
	{
		return;
	}
	// Pixel Shader にテクスチャをセット
	m_pContext->PSSetShaderResources(0, 1, &g_Texture[textureID].pTexture);
}

DirectX::XMUINT2 Texture::GetSize(int textureID) const
{
    return { g_Texture[textureID].width, g_Texture[textureID].height };
}

unsigned int Texture::GetWidth(int textureID) const
{
    return g_Texture[textureID].width;
}

unsigned int Texture::GetHeight(int textureID) const
{
    return g_Texture[textureID].height;
}

Texture* Texture::GetTexture()
{
	static Texture instance;   // Meyers Singleton
	return &instance;
}
