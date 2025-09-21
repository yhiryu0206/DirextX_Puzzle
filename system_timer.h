/*------------------------------
*	システムタイマー[system_timer.h]
*
* 制作者：山室飛龍		日付：2025/9/22
------------------------------*/

#ifndef SYSTEM_TIMER_H
#define SYSTEM_TIMER_H

#include <Windows.h>

class SystemTimer
{
private:
	bool m_bTimerStopped = true;	//ストップフラグ
	LONGLONG m_TicksPerSec = 0;  // １秒間の計測時間
	LONGLONG m_StopTime;         // ストップした時間
	LONGLONG m_LastElapsedTime;  // 最後に記録した更新時間
	LONGLONG m_BaseTime;         // 基本時間
public:
	void Initialize();
	void Reset();

	void Start();
	void Stop();
	void Advance();
	double GetTime();
	double GetAbsoluteTime();
	float GetElapsedTime();

	bool IsStoped();
	void LimitThreadAffinityToCurrentProc();

	// 停止していれば停止時間、そうでなければ現在の時間の取得
	LARGE_INTEGER GetAdjustedCurrentTime(void);



};



#endif // !SYSTEM_TIMER_H
