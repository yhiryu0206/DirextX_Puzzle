/*------------------------------
* テクスチャの管理[texture.h]
*
* 制作者：山室飛龍		日付：2025/9/20
------------------------------*/

#ifndef TEXTURE_H
#define TEXTURE_H

#include <d3d11.h>
#include <string>
#include <DirectXMath.h>

class Texture
{
private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

public:

	void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Finalize();

	int Load(const wchar_t* pFilename, bool bMipMap = true);

	void AllRelease();
	void SetTexture(int textureID);

	/* 読み取り専用 */
	DirectX::XMUINT2 GetSize(int textureID) const;
	unsigned int GetWidth(int textureID) const ;
	unsigned int GetHeight(int textureID) const;

	static Texture* GetTexture();
};

#endif // !TEXTURE_H