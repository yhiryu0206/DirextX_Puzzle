/*------------------------------
*	タイトル[title.cpp]
*
* 制作者：山室飛龍		日付：2025/9/22
------------------------------*/
#include "title.h"
#include "texture.h"
#include "sprite.h"
#include "keylogger.h" 
#include "scene.h"
#include "Audio.h"
#include "fade.h"

void Title::Initialize()
{
	Fade* g_Fade = Fade::GetFade();
	Texture* g_Texture = Texture::GetTexture();
	m_TitleTexId = g_Texture->Load(L"resource/title.png");
	m_TitleSoundId = LoadAudio("resource/Sound/古の世界地図.wav");
	m_PushSoundId = LoadAudio("resource/Sound/maou_se_system46.wav");

	g_Fade->Start(1.0f, true);
	
	PlayAudio(m_TitleSoundId, true);
	m_State = TITLE_STATE_FADEIN;
}

void Title::Finalize()
{
}

void Title::Update(double elapsedTime)
{
	Fade* g_Fade = Fade::GetFade();
	g_Fade->Update(elapsedTime);   // ← これを必ず呼ぶ
	Scene* g_Scene = Scene::GetScene();
	m_AccumulatedTime += elapsedTime;

	switch (m_State)
	{
	case Title::TITLE_STATE_FADEIN:
		if (g_Fade->GetState() == g_Fade->FADE_STATE_FADEIN_FINISHED)
		{
			m_State = TITLE_STATE_KEYINPUT_WAIT;
		}
		break;
	case Title::TITLE_STATE_KEYINPUT_WAIT:
		if (IsKeyDown(KK_ENTER))
		{
			m_State = TITLE_STATE_KEYINPUT_ACTION;
			m_KeyInputTime = m_AccumulatedTime;
			PlayAudio(m_PushSoundId);
		}
		break;
	case Title::TITLE_STATE_KEYINPUT_ACTION:
		if (m_AccumulatedTime - m_KeyInputTime > 0.0)
		{
			m_State = TITLE_STATE_FADEOUT;
			g_Fade->Start(3.0f, false);;
		}
		break;
	case Title::TITLE_STATE_FADEOUT:
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

void Title::Draw()
{
	Sprite* sprite = Sprite::GetSprite();	
	sprite->Draw(m_TitleTexId, 0, 0, 1600, 900);

	Fade* g_Fade = Fade::GetFade();
	g_Fade->Draw();
}

