/*------------------------------
*	リザルト[reselt.h]
*
* 制作者：山室飛龍		日付：2025/9/22
------------------------------*/

#ifndef RESELT_H
#define RESELT_H

class Reselt
{
public:
	void Initialize();
	void Finalize();
	void Update(double elapsedTime);
	void Draw();

private:
	int m_ReseltTexId = -1;
	int m_ReseltSoundId = 0;
	double m_AccumulatedTime = 0.0;
	double m_KeyInputTime = 0.0;

	enum ReseltState
	{
		RESELT_STATE_FADEIN,
		RESELT_STATE_KEYINPUT_WAIT,
		RESELT_STATE_KEYINPUT_ACTION,
		RESELT_STATE_FADEOUT,
	};

	ReseltState m_State = RESELT_STATE_FADEIN;

};


#endif // !RESELT_
