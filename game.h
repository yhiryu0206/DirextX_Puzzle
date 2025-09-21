/*------------------------------
*	ゲームの処理[game.h]
*
* 制作者：山室飛龍		日付：2025/9/21
------------------------------*/
#ifndef GAME_H
#define GAME_H

class Game
{

public:
	void Initialize();
	void Finalize();
	void Updata(double elapsedTime);
	void Draw();

	static Game* GetGame();

};
#endif // !GAME_H
