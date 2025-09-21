/*------------------------------
*	ブロック処理[block.cpp]
*
* 制作者：山室飛龍		日付：2025/9/21
------------------------------*/
#include "block.h"
#include "Texture.h"
#include "Sprite.h"

void GameBlock::Initialize()
{
	Texture* g_Texture = Texture::GetTexture();
	m_BlockTexId = g_Texture->Load(L"resource/Jewel.png");

}

void GameBlock::Finalize()
{
}

void GameBlock::Updata()
{
}

void GameBlock::Draw(int kindIndex, float x, float y)
{
	GameBlock Block{};
	Sprite* g_Sprite = Sprite::GetSprite();
	g_Sprite->Draw(m_BlockTexId,x,y,Block.GetWidth(),
		Block.GetHeight(),kindIndex * 24, kindIndex * 24,
		24, 24);
}

float GameBlock::GetWidth()
{
	return m_BlockWidth;
}

float GameBlock::GetHeight()
{
	return m_BlockHeight;
}

GameBlock* GameBlock::GetBlock()
{
	static GameBlock instance;   // Meyers Singleton
	return &instance;
}
