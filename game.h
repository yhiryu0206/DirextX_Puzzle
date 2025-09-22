/*------------------------------
*	ゲームの処理[game.h]
*
* 制作者：山室飛龍		日付：2025/9/21
------------------------------*/
#ifndef GAME_H
#define GAME_H

#include "back_ground.h"
#include "block.h"
#include "stage.h"
#include "carrier.h"

class Game
{

public:
	void Initialize();
	void Finalize();
	void Update(double elapsedTime);
	void Draw();

	static Game* GetGame();

private:
	int m_GameSoundId;

};
#endif // !GAME_H
