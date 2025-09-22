/*------------------------------
*	フェード処理[fade.h]
*
* 制作者：山室飛龍		日付：2025/9/22
------------------------------*/
#ifndef FADE_H
#define FADE_H

#include <DirectXMath.h>

class Fade
{
public:
	void Initialize();
	void Finalize();

	void Update(double elapsedTime);
	void Draw();

	void Start(double fadeTime,bool isFadeIn,
		DirectX::XMFLOAT4 fadeColor = { 0.0f,0.0f,0.0f,1.0f });

	enum FadeState
	{
		FADE_STATE_NONE,
		FADE_STATE_FADEOUT,
		FADE_STATE_FADEOUT_FINISHED,
		FADE_STATE_FADEIN,
		FADE_STATE_FADEIN_FINISHED,
		FADE_STATE_MAX,
	};

	//上のenumの内部情報を取得
	FadeState GetState();

	static Fade* GetFade();


private:
	FadeState m_FadeState = FADE_STATE_NONE;
	double m_FadeTime = 0.0;
	double m_AccumulatedTime = 0.0f;
	double m_FadeStartTime = 0.0;
	DirectX::XMFLOAT4 m_FadeColor = { 0.0f,0.0f,0.0f,0.0f };
	int m_WhiteTexId = -1;

};

#endif // !FADE_H
