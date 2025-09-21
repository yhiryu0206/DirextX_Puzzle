/*------------------------------
*	ゲームの製作[carrier.h]
*
* 制作者：山室飛龍		日付：2025/9/21
------------------------------*/
#ifndef CARRIER_H
#define CARRIER_H

#include <algorithm>
#include <random>

class Carrier
{
public:
	void Initialize();
	void Finalize();
	void Updata(double elapsedTime);
	void Draw();

	/* リセット先座標を変える */
	void Reset(float x, float y);
	void MoveLeft();
	void MoveRight();
	void SwapUp();
	void SwapDown();
	void Fall();

	const int* GetBlock();


	int m_BlockList[3]{ -1,-1,-1 };

	float m_CarrierX = 0.0f;
	float m_CarrierY = 0.0f;

	float m_MoveStartX = 0.0f;
	float FALL_HEIGHT = 0;
	float MOVE_WIDTH = 0;
	double m_FallWaitTime = 0.3;
	double m_AccumulatedTime = 0.0;
	double m_MoveStartTime = 0.0;
	double m_FallWaitStartTime = 0.0;
	double m_DestroyWaitStartTime = 0.0;
	double MOVE_TIME = 1.012 * 2;

	static Carrier* GetCarrier();


	enum CarrierStateMode
	{
		FALL_WAIT,
		FALL,
		FALL_FORCE,
		MOVE_LEFT,
		MOVE_RIGHT,
		LANDING,
		DESTROY,
		DESTROY_WAIT,
		ADJUST,
		ADJUST_WAIT,

		GAMEOVER,

		MAX,
	};

	CarrierStateMode m_CarrierState = FALL_WAIT;

};


#endif // !CARRIER_H
