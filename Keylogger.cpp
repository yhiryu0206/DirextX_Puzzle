/*------------------------------
* キーボード入力の記録[keylogger.cpp]
*
* 制作者：山室飛龍		日付：2025/6/27
------------------------------*/
#include "Keylogger.h"

static Keyboard_State g_Prevstate = {};
static Keyboard_State g_TriggerState = {};
static Keyboard_State g_ReleaseState = {};


void KeyLoggerInitialize()
{
	Keyboard_Initialize();
}

void KeyLoggerUpdata()
{
	const Keyboard_State* state = Keyboard_GetState();
	LPBYTE pt = (LPBYTE)&g_TriggerState;
	LPBYTE pn = (LPBYTE)state;
	LPBYTE po = (LPBYTE)&g_Prevstate;
	LPBYTE pr = (LPBYTE)&g_ReleaseState;

	for (int i = 0; i < sizeof(Keyboard_State); i++)
	{
		pt[i] = (po[i] ^ pn[i]) & pn[i];
		//pr[i] = (po[i] ^ pn[i]) & ~pn[i];

		pr[i] = (po[i] & ~pn[i]);
	}

	g_Prevstate = *state;

}
//キーリピートなるものを作ってみよう

bool KeyLoggerIsPressed(Keyboard_Keys key)
{
	return Keyboard_IsKeyDown(key);
}

bool KeyLoggerIsTrigger(Keyboard_Keys key)
{
	return Keyboard_IsKeyDown(key, &g_TriggerState);
}

bool KeyLoggerIsRelease(Keyboard_Keys key)
{
	return Keyboard_IsKeyDown(key, &g_ReleaseState);
}
