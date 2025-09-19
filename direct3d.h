/*------------------------------
*	DirectX[direct3d.h]
*
* 制作者：山室飛龍		日付：2025/9/19
------------------------------*/
#ifndef DIRECT3D_H
#define DIRECT3D_H

#include <d3d11.h>
#include <windows.h>

class Direct3D
{
	bool Initialize(HWND wnd);	//Direct
	void Finalize();
	
	void Clear();	//バックバッファのクリア
	void Present();	//バックバッファの表示
};

#endif // !DIRECT3D_H
