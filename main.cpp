/*------------------------------
* main[main.cpp]
*
* 制作者：山室飛龍		日付：2025/9/19
------------------------------*/
#include<SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include<windows.h>
#include "direct3d.h"


#ifndef NOMINMAX
#define NOMINMAX

#endif // !NOMINMAX
#include <algorithm>


/*------------------------------
*	ウィンドウ情報
------------------------------*/

static constexpr char WINDOW_CLASS[] = "GameWindow";	//メインウィンドウクラス名
static constexpr char TITLE[] = "ウィンドウ表示";	//タイトルバーのテキスト

/*------------------------------
*	ウィンドウプロシージャ プロトタイプ宣言
------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wPARAM, LPARAM lParam);

/*------------------------------
*	メイン
------------------------------*/
int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	(void)CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	//DPIスケーリング
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	/* ウィンドウクラスの登録 */
	WNDCLASSEX wcex{};	//初期化して、ゴミを消してるらしい

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = WndProc;		//ウィンドウプロシージャを登録(関数ポインタ)
	wcex.hInstance = hInstance;		//認識系の何かを突っ込んでる
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);	//アイコン変更
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);		//カーソル変更
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	//背景の色
	wcex.lpszMenuName = nullptr;	//メニューは作らない
	wcex.lpszClassName = WINDOW_CLASS;	//なんか名前つっこんでる
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);	//ちっちゃいアイコン

	RegisterClassEx(&wcex);

	//クライアント領域(メインの場所)のさいずをもった短形(左からleft,top,right,bottom)
	RECT window_rect = { 0,0,1600,900 };

	//ウィンドウのスタイル
	DWORD window_style = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX;

	//指定したクライアント領域を確保するために新たな短形座標を計算
	AdjustWindowRect(&window_rect, window_style, FALSE);

	//ウィンドウの幅と高さを算出
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	//プライマリモニターの画像解像度取得
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	//デスクトップの真ん中にウィンドウが生成されるように座標を計算
	// ただし万が一、デスクトップよりウィンドウが大きい場合は左上に表示
	int window_x = std::max<int>((desktop_width - window_width) / 2, 0);
	int window_y = std::max<int>((desktop_height - window_height) / 2, 0);

	/* メインウィンドウの作成 */
	HWND hWnd = CreateWindow(WINDOW_CLASS, TITLE, window_style,
		window_x, window_y, window_width, window_height, nullptr, nullptr, hInstance, nullptr);
	//USERDEFAULTはいい感じの位置にいい感じのサイズで置いてくれる



	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	/* メッセージループ */
	MSG msg;

	//マウスの位置や画面サイズなどのメッセージが飛んでくるのを待っている
	//Getmessageは同期処理,PeekMessageを非同期処理という
	do
	{

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			//ウィンドウメッセージが来ていたら
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	} while (msg.message != WM_QUIT);

	return (int)msg.wParam;

}
/*------------------------------
*	ウィンドウプロシージャ
------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}
		break;

		//こいつを消すとウィンドウが消えなくなる
	case WM_CLOSE:
		if (MessageBox(hWnd, "本当に終了してよろしいですか？",
			"確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
		{
			DestroyWindow(hWnd);	//指定のウィンドウにWM_DESTROYメッセージを送る
		}
		break;

	case WM_DESTROY:	//ウィンドウの破棄メッセージ
		PostQuitMessage(0);	//WM_QUITメッセージの送信
		break;

	default:
		//通常のメッセージ処理はこの関数に任せる
		return DefWindowProc(hWnd, message, wParam, lParam);
		//break;
	}
	return 0;
}