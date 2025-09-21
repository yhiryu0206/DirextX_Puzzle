/*------------------------------
*	ƒQ[ƒ€‚Ì”wŒiˆ—[back_ground.cpp]
*
* §ìŽÒFŽRŽº”ò—´		“ú•tF2025/9/21
------------------------------*/
#include "back_ground.h"
#include "texture.h"
#include "sprite.h"

void GameBackGround::Initialize()
{
	Texture* g_Texture = Texture::GetTexture();
	m_BackGroundTexId = g_Texture->Load(L"resource/NightSky/BackGroundTexId.png");
}

void GameBackGround::Finalize()
{
}

void GameBackGround::Updata()
{
}

void GameBackGround::Draw()
{
	Sprite* g_Sprite = Sprite::GetSprite();
	Texture* g_Texture = Texture::GetTexture();
	/*g_Sprite->Draw(m_BackGroundTexId, 0, 0,
		g_Texture->GetWidth(m_BackGroundTexId),
		g_Texture->GetHeight(m_BackGroundTexId));*/
	g_Sprite->Draw(m_BackGroundTexId, 0, 0,1600,900);
}
