/*------------------------------
* main[main.cpp]
*
* 制作者：山室飛龍		日付：2025/9/19
------------------------------*/
#include<SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include<windows.h>
#include "direct3d.h"
#include <algorithm>
#include "shader.h"
#include "texture.h"
#include "sprite.h"
#include "system_timer.h"
#include "keylogger.h"
#include "mouse.h"
#include "debug_text.h"
#include <sstream>

#include "scene.h"

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
	/* インスタンス */
	Direct3D g_Direct3D;
	Shader* g_Shader = Shader::GetShader();
	Texture* g_Texture = Texture::GetTexture();
	Sprite* g_Sprite = Sprite::GetSprite();
	SystemTimer g_SystemTimer{};
	Scene* g_Scene = Scene::GetScene();


	(void)CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	//DPIスケーリング
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	/* ウィンドウクラスの登録 */
	WNDCLASSEX wcex{};	// 構造体を0で初期化

	wcex.cbSize = sizeof(WNDCLASSEX);            // 構造体のサイズを設定（必須）
	wcex.lpfnWndProc = WndProc;                  // ウィンドウプロシージャを設定
	wcex.hInstance = hInstance;                  // このアプリケーションのインスタンスハンドル
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION); // ウィンドウの大きいアイコン
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);     // ウィンドウで使用するカーソル
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);   // 背景ブラシ（ウィンドウ背景色）
	wcex.lpszMenuName = nullptr;                 // メニュー名（今回はなし）
	wcex.lpszClassName = WINDOW_CLASS;           // ウィンドウクラス名（識別用）
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION); // 小さいアイコン
	RegisterClassEx(&wcex);                      // ウィンドウクラスを登録

	// クライアント領域のサイズを設定（表示されるメイン領域の幅と高さ）
	RECT window_rect = { 0, 0, 1600, 900 };

	// ウィンドウスタイルを設定（タイトルバーや閉じるボタンは残すが、リサイズと最大化は無効）
	DWORD window_style = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX;

	// クライアント領域を優先してウィンドウ全体のサイズを計算
	AdjustWindowRect(&window_rect, window_style, FALSE);

	// ウィンドウの幅と高さを算出
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	// プライマリモニターの解像度を取得
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	// ウィンドウを画面中央に配置（画面より大きければ左上に表示）
	int window_x = std::max<int>((desktop_width - window_width) / 2, 0);
	int window_y = std::max<int>((desktop_height - window_height) / 2, 0);

	/* メインウィンドウの作成 */
	HWND hWnd = CreateWindow(
		WINDOW_CLASS, TITLE, window_style,
		window_x, window_y, window_width, window_height,
		nullptr, nullptr, hInstance, nullptr
	);

		/* 各種初期化(なんかあったら嫌だから更新とかした後に) */
	if (!g_Direct3D.Initialize(hWnd))//Direct3Dの初期化
	{
		PostQuitMessage(0);
		g_Direct3D.Finalize();
		return 0;	//初期化失敗で終了
	}
	else
	{
		//エラーが起きた場合終了するよ
		if (!g_Shader->Initialize(g_Direct3D.GetDevice(),
			g_Direct3D.GetDeviceContext()))
		{
			PostQuitMessage(0);
		}
		else
		{

			g_Texture->Initialize(g_Direct3D.GetDevice(),
				g_Direct3D.GetDeviceContext());

			g_Sprite->Initialize(g_Direct3D.GetDevice(),
				g_Direct3D.GetDeviceContext(),
				g_Shader,g_Texture);

		}
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	/* もろもろ初期化処理 */
	g_Scene->Initialize();
	g_SystemTimer.Initialize();
	KeyLoggerInitialize();
	Mouse_Initialize(hWnd);

	///* 時間計測 */
	double execLastTime = 0.0;
	double fpsLastTime = 0.0;
	double currentTime = 0.0;
	ULONG frameCount = 0;
	double fps = 0.0;

	execLastTime = fpsLastTime = g_SystemTimer.GetTime();

	hal::DebugText dt(g_Direct3D.GetDevice(),
		g_Direct3D.GetDeviceContext(),
		L"consolab_ascii_512.png",
		g_Direct3D.GetBackBufferWidth(), g_Direct3D.GetBackBufferHeight(),
		0.0f, 0.0f,
		0, 0,
		0.0f, 0.0f);

	/* メッセージループ */
	MSG msg;


	int a = g_Texture->Load(L"名称未設定1.png");

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
		else
		{
			currentTime = g_SystemTimer.GetTime();
			double elapsedTime = currentTime - fpsLastTime;

			if (elapsedTime >= 1.0)
			{
				fps = frameCount / elapsedTime;	//一秒間どれくらいなのかがでる
				fpsLastTime = currentTime;	//FPSを測定した時刻をほぞｎ
				frameCount = 0;	//カウントのクリア
			}

			//60フレーム付近固定にする処理
			elapsedTime = currentTime - execLastTime;
			if ((execLastTime) >= (1.0 / 60.0))

			{
				execLastTime = currentTime;
				g_Direct3D.Clear();

				/* Updata */
				g_Scene->Updata(elapsedTime);

				/* Draw */
				g_Sprite->Draw(a, 0, 0, 100, 100);
				g_Scene->Draw();

#if defined(DEBUG)|| defined(_DEBUG)
				std::stringstream ss;
				ss << "fps" << fps << std::endl;
				dt.SetText(ss.str().c_str());

				dt.Draw();
				dt.Clear();
#endif

				g_Direct3D.Present();
				frameCount++;
			}
		}

	} while (msg.message != WM_QUIT);

	/* 各種終了処理 */
	g_Direct3D.Finalize();
	g_Sprite->Finalize();
	g_Texture->Finalize();
	g_Shader->Finalize();
	CoUninitialize();

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