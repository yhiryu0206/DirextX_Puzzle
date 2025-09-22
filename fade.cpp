/*------------------------------
*	フェード処理[fade.cpp]
*
* 制作者：山室飛龍		日付：2025/9/22
------------------------------*/
#include "fade.h"
#include "sprite.h"
#include "texture.h"
using namespace DirectX;
#include "direct3d.h"

void Fade::Initialize()
{
	Texture* m_Texture = Texture::GetTexture();
	m_FadeState = FADE_STATE_NONE;
	m_AccumulatedTime = 0.0;

	m_WhiteTexId= m_Texture->Load(L"resource/white.png");
}

void Fade::Finalize()
{
}

void Fade::Update(double elapsedTime)
{
	//時間計測とステートの管理
	if (m_FadeState == FADE_STATE_NONE ||
		m_FadeState == FADE_STATE_FADEIN_FINISHED ||
		m_FadeState == FADE_STATE_FADEOUT_FINISHED)
	{
		return;
	}

	double lifeTime = m_AccumulatedTime - m_FadeStartTime;

	//透明度の計算
	float alpha = static_cast<float>(lifeTime / m_FadeTime);

	m_FadeColor.w = m_FadeState == FADE_STATE_FADEIN ? 1.0f - alpha : alpha;
	if (m_FadeTime <= lifeTime)
	{
		m_FadeState = m_FadeState == FADE_STATE_FADEIN ?
			FADE_STATE_FADEIN_FINISHED : FADE_STATE_FADEOUT_FINISHED;
	}
	m_AccumulatedTime += elapsedTime;
}

void Fade::Draw()
{
	Sprite* g_Sprite = Sprite::GetSprite();

	if (m_FadeState == FADE_STATE_NONE || m_FadeState == FADE_STATE_FADEIN_FINISHED)
	{
		return;
	}
	g_Sprite->Draw(m_WhiteTexId, 0, 0, 1600, 900, m_FadeColor);
}

void Fade::Start(double fadeTime, bool isFadeIn, DirectX::XMFLOAT4 fadeColor)
{
	m_FadeTime = fadeTime;
	m_FadeState = isFadeIn ? FADE_STATE_FADEIN : FADE_STATE_FADEOUT;
	m_FadeStartTime = m_AccumulatedTime;
	m_FadeColor = fadeColor;
}

Fade::FadeState Fade::GetState()
{
	return m_FadeState;
}

Fade* Fade::GetFade()
{
	static Fade instance;   // Meyers Singleton
	return &instance;
}