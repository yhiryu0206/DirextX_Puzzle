/*------------------------------
*	システムタイマー[system_timer.h]
*
* 制作者：山室飛龍		日付：2025/9/22
------------------------------*/
#include "system_timer.h"

/* システムタイマーの初期化 */
void SystemTimer::Initialize()
{
    m_bTimerStopped = true;
    m_TicksPerSec = 0;
    m_StopTime = 0;
    m_LastElapsedTime = 0;
    m_BaseTime = 0;

    // 高分解能パフォーマンスカウンタ周波数の取得
    LARGE_INTEGER ticksPerSec = { 0 };
    QueryPerformanceFrequency(&ticksPerSec);
    m_TicksPerSec = ticksPerSec.QuadPart;
}

/* システムタイマーのリセット */
void SystemTimer::Reset()
{
    LARGE_INTEGER time = GetAdjustedCurrentTime();

    m_BaseTime = m_LastElapsedTime = time.QuadPart;
    m_StopTime = 0;
    m_bTimerStopped = false;
}

/* システムタイマーのスタート */
void SystemTimer::Start()
{
    // 現在の時間を取得
    LARGE_INTEGER time = { 0 };
    QueryPerformanceCounter(&time);

    // 今まで計測がストップしていたら
    if (m_bTimerStopped) {
        // 止まっていた時間を差し引いて基本時間を更新
        m_BaseTime += time.QuadPart - m_StopTime;
    }

    m_StopTime = 0;
    m_LastElapsedTime = time.QuadPart;
    m_bTimerStopped = false;
}

/* システムタイマーのストップ */
void SystemTimer::Stop()
{
    if (m_bTimerStopped) return;

    LARGE_INTEGER time = { 0 };
    QueryPerformanceCounter(&time);

    m_LastElapsedTime = m_StopTime = time.QuadPart; // 停止時間を記録
    m_bTimerStopped = true;
}

/* システムタイマーを0.1秒進める */
void SystemTimer::Advance()
{
    m_StopTime += m_TicksPerSec / 10;
}

/* 計測時間を取得 */
double SystemTimer::GetTime()
{
    LARGE_INTEGER time = GetAdjustedCurrentTime();

    return (double)(time.QuadPart - m_BaseTime) / (double)m_TicksPerSec;
}

/* 現在の時間を取得 */
double SystemTimer::GetAbsoluteTime()
{
    LARGE_INTEGER time = { 0 };
    QueryPerformanceCounter(&time);

    return time.QuadPart / (double)m_TicksPerSec;
}

/* 経過時間の取得 */
float SystemTimer::GetElapsedTime()
{
    LARGE_INTEGER time = GetAdjustedCurrentTime();

    double elapsed_time = (float)((double)(time.QuadPart - m_LastElapsedTime) / (double)m_TicksPerSec);
    m_LastElapsedTime = time.QuadPart;

    // タイマーが正確であることを保証するために、更新時間を０にクランプする。
    // elapsed_timeは、プロセッサが節電モードに入るか、何らかの形で別のプロセッサにシャッフルされると、この範囲外になる可能性がある。
    // よって、メインスレッドはSetThreadAffinityMaskを呼び出して、別のプロセッサにシャッフルされないようにする必要がある。
    // 他のワーカースレッドはSetThreadAffinityMaskを呼び出すべきではなく、メインスレッドから収集されたタイマーデータの共有コピーを使用すること。
    if (elapsed_time < 0.0f) {
        elapsed_time = 0.0f;
    }

    return (float)elapsed_time;
}

/* システムタイマーが止まっているか？ */
bool SystemTimer::IsStoped()
{
    return m_bTimerStopped;
}

/* 現在のスレッドを1つのプロセッサ（現在のスレッド）に制限 */
void SystemTimer::LimitThreadAffinityToCurrentProc()
{
    HANDLE hCurrentProcess = GetCurrentProcess();

    // Get the processor affinity mask for this process
    DWORD_PTR dwProcessAffinityMask = 0;
    DWORD_PTR dwSystemAffinityMask = 0;

    if (GetProcessAffinityMask(hCurrentProcess, &dwProcessAffinityMask, &dwSystemAffinityMask) != 0 && dwProcessAffinityMask) {
        // Find the lowest processor that our process is allows to run against
        DWORD_PTR dwAffinityMask = (dwProcessAffinityMask & ((~dwProcessAffinityMask) + 1));

        // Set this as the processor that our thread must always run against
        // This must be a subset of the process affinity mask
        HANDLE hCurrentThread = GetCurrentThread();
        if (INVALID_HANDLE_VALUE != hCurrentThread) {
            SetThreadAffinityMask(hCurrentThread, dwAffinityMask);
            CloseHandle(hCurrentThread);
        }
    }

    CloseHandle(hCurrentProcess);
}

/* 停止していれば停止時間、そうでなければ現在の時間の取得 */
LARGE_INTEGER SystemTimer::GetAdjustedCurrentTime(void)
{
    LARGE_INTEGER time;
    if (m_StopTime != 0) {
        time.QuadPart = m_StopTime;
    }
    else {
        QueryPerformanceCounter(&time);
    }

    return time;
}
