/*------------------------------
* キーボード入力の記録[keylogger.h]
*
* 制作者：山室飛龍		日付：2025/6/27
------------------------------*/
#include "keyboard.h"

#ifndef KEYLOGGER_H
#define KEYLOGGER_H

void KeyLoggerInitialize();
void KeyLoggerUpdata();

bool KeyLoggerIsPressed(Keyboard_Keys key);
bool KeyLoggerIsTrigger(Keyboard_Keys key);
bool KeyLoggerIsRelease(Keyboard_Keys key);



#endif // KEYLOGGER_H
