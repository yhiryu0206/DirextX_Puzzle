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
	m_BackGroundGridTexId = g_Texture->Load(L"resource//grid_12x8.png");

}

void GameBackGround::Finalize()
{
}

void GameBackGround::Update()
{
}

void GameBackGround::Draw()
{
	Sprite* g_Sprite = Sprite::GetSprite();

	g_Sprite->Draw(m_BackGroundTexId, 0, 0,1600,900);
	g_Sprite->Draw(m_BackGroundGridTexId, 522, 72, 512, 768);
}
