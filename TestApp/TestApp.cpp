// TestApp.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "TestApp.h"
#include <iostream>
#include <gdiplus.h>
#include <string>

#define MAX_LOADSTRING 100

void create_console(void) {
	FILE* fp;
	AllocConsole();
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);
}

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名
const int screenWidth = 1280;
const int screenHeight = 720;
//const int screenWidth = 640;
//const int screenHeight = 360;

// Definitions of DLL functions
void(*init)(int, int) = NULL;
void(*enable_direct_frame_buffer)(unsigned char *) = NULL;
void(*exchange_frame_buffer)(unsigned char *) = NULL;
void(*clear_frame_buffer)() = NULL;
void(*free_frame_buffer)() = NULL;
void(*run_renderer)() = NULL;
void(*renderer_finish)() = NULL;

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
HWND                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL				SetClientSize(HWND, int, int);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。
	create_console();

	HINSTANCE hDLL = LoadLibrary(L".././Release/SimpleRayTracer.dll");
	if (hDLL == NULL)
	{
		std::cout << "'SimpleRayTracer.dll' is not found." << std::endl;
		return -1;
	}

	// Load DLL functions
	init = (void(*)(int, int))GetProcAddress(hDLL, "init");
	enable_direct_frame_buffer = (void(*)(unsigned char *))GetProcAddress(hDLL, "enable_direct_frame_buffer");
	exchange_frame_buffer = (void(*)(unsigned char *))GetProcAddress(hDLL, "exchange_frame_buffer");
	clear_frame_buffer = (void(*)())GetProcAddress(hDLL, "clear_frame_buffer");
	free_frame_buffer = (void(*)())GetProcAddress(hDLL, "free_frame_buffer");
	run_renderer = (void(*)())GetProcAddress(hDLL, "run_renderer");
	renderer_finish = (void(*)())GetProcAddress(hDLL, "renderer_finish");

	// Check DLL functions
	if (
		init == NULL || 
		enable_direct_frame_buffer == NULL || 
		exchange_frame_buffer == NULL ||
		clear_frame_buffer == NULL || 
		free_frame_buffer == NULL ||
		run_renderer == NULL ||
		renderer_finish == NULL
		)
	{
		std::cout << "Failed to load DLL functions." << std::endl;
		return -1;
	}

	init(screenHeight, screenWidth);
	run_renderer();

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TESTAPP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
	HWND hWnd;
	if ((hWnd = InitInstance(hInstance, nCmdShow)) != NULL)
	{
		SetClientSize(hWnd, screenWidth, screenHeight);
		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);
	}
	else
		return -1;

	MSG msg;
	HDC hdc = GetDC(hWnd);
	HDC hBuf = CreateCompatibleDC(hdc);

	unsigned char *ppvBits;
	BITMAPINFO bmpInfo;
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = screenWidth;
	bmpInfo.bmiHeader.biHeight = -screenHeight;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 24;
	bmpInfo.bmiHeader.biCompression = BI_RGB;

	HBITMAP hDIBSection = CreateDIBSection(hdc, &bmpInfo, DIB_RGB_COLORS, (VOID **)&ppvBits, nullptr, 0);
	HBITMAP hbmpOld = (HBITMAP)SelectObject(hBuf, hDIBSection);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTAPP));

    // メイン メッセージ ループ:
    while (1)
    {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			if (msg.message == WM_QUIT) break;
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else 
		{
			exchange_frame_buffer(ppvBits);
			BitBlt(hdc,0, 0, screenWidth, screenHeight, hBuf, 0, 0, SRCCOPY);
		}
    }

	SelectObject(hBuf, hbmpOld);
	DeleteObject(hDIBSection);

	FreeLibrary(hDLL);

    return (int) msg.wParam;
}

BOOL SetClientSize(HWND hWnd, int width, int height)
{
	RECT rw, rc;
	::GetWindowRect(hWnd, &rw);
	::GetClientRect(hWnd, &rc);

	int new_width = (rw.right - rw.left) - (rc.right - rc.left) + width;
	int new_height = (rw.bottom - rw.top) - (rc.bottom - rc.top) + height;

	return ::SetWindowPos(hWnd, NULL, 0, 0, new_width, new_height, SWP_NOMOVE | SWP_NOZORDER);
}

//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTAPP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TESTAPP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存します。
//
HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
      CW_USEDEFAULT, CW_USEDEFAULT, screenWidth, screenHeight, nullptr, nullptr, hInstance, nullptr);

   return hWnd;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 選択されたメニューの解析:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: HDC を使用する描画コードをここに追加してください...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
		renderer_finish();
		free_frame_buffer();
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
