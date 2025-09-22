/*------------------------------
*	タイトル[title.h]
*
* 制作者：山室飛龍		日付：2025/9/22
------------------------------*/

#ifndef TITLE_H
#define TITLE_H

class Title
{
public:
	void Initialize();
	void Finalize();
	void Update(double elapsedTime);
	void Draw();

private:
	int m_TitleTexId = -1;
	int m_TitleSoundId = 0;
	int m_PushSoundId = 0;
	double m_AccumulatedTime = 0.0;
	double m_KeyInputTime = 0.0;

	enum TitleState
	{
		TITLE_STATE_FADEIN,
		TITLE_STATE_KEYINPUT_WAIT,
		TITLE_STATE_KEYINPUT_ACTION,
		TITLE_STATE_FADEOUT,
	};

	TitleState m_State = TITLE_STATE_FADEIN;

};

#endif // !TITLE_H
