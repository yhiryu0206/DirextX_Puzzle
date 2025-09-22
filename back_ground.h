/*------------------------------
*	ゲームの背景処理[back_ground.h]
*
* 制作者：山室飛龍		日付：2025/9/21
------------------------------*/
#ifndef BACKGROUND_H
#define BACKGROUND_H

class GameBackGround
{
private:
	int m_BackGroundTexId;
	int m_BackGroundTexId2;
	int m_BackGroundGridTexId;

public:

	void Initialize();
	void Finalize();

	void Update();
	void Draw();


};
#endif // !GAME_BACKGROUND_H
