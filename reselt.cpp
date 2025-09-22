/*------------------------------
*	リザルト[reselt.cpp]
*
* 制作者：山室飛龍		日付：2025/9/22
------------------------------*/
#include "reselt.h"
#include "texture.h"
#include "sprite.h"
#include "keylogger.h" 
#include "scene.h"
#include "Audio.h"
#include "fade.h"

void Reselt::Initialize()
{
	Fade* g_Fade = Fade::GetFade();
	Texture* g_Texture = Texture::GetTexture();
	m_ReseltTexId = g_Texture->Load(L"resource/gameover.png");
	m_ReseltSoundId = LoadAudio("resource/Sound/古代文明の跡地.wav");
	PlayAudio(m_ReseltSoundId);
	g_Fade->Start(3.0f, true);
	m_State = RESELT_STATE_FADEIN;
}

void Reselt::Finalize()
{
}

void Reselt::Update(double elapsedTime)
{
	Fade* g_Fade = Fade::GetFade();
	g_Fade->Update(elapsedTime);   // ← これを必ず呼ぶ
	Scene* g_Scene = Scene::GetScene();
	m_AccumulatedTime += elapsedTime;


	switch (m_State)
	{
	case Reselt::RESELT_STATE_FADEIN:
		if (g_Fade->GetState() == g_Fade->FADE_STATE_FADEIN_FINISHED)
		{
			m_State = RESELT_STATE_KEYINPUT_WAIT;
		}
		break;
	case Reselt::RESELT_STATE_KEYINPUT_WAIT:
		if (IsKeyDown(KK_ENTER))
		{
			m_State = RESELT_STATE_KEYINPUT_ACTION;
			m_KeyInputTime = m_AccumulatedTime;
		}
		break;
	case Reselt::RESELT_STATE_KEYINPUT_ACTION:
		if (m_AccumulatedTime - m_KeyInputTime > 0.0)
		{
			m_State = RESELT_STATE_FADEOUT;
			g_Fade->Start(3.0f, false);;
		}
		break;
	case Reselt::RESELT_STATE_FADEOUT:
		if (g_Fade->GetState() == g_Fade->FADE_STATE_FADEOUT_FINISHED)
		{
			//ゲームシーンへ移行
			g_Scene->SetNextScene(g_Scene->SCENE_GAME);
			g_Scene->ChangeScene();
		}
		break;
	default:
		break;
	}
}

void Reselt::Draw()
{
	Sprite* sprite = Sprite::GetSprite();
	sprite->Draw(m_ReseltTexId, 0, 0, 1600, 900);

	Fade* g_Fade = Fade::GetFade();
	g_Fade->Draw();
}

