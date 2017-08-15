#include "resource.h"
#include "Figure.h"
#include "GameParam.h"
#include "Field.h"
WORD RegClass(HINSTANCE hInst);
HWND CreateWnd(HINSTANCE hInst);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
VOID CALLBACK From_Right_To_Left(HWND, UINT, UINT_PTR, DWORD);
VOID CALLBACK From_Left_To_Right(HWND, UINT, UINT_PTR, DWORD);
void Stop();
void Restart();
void GLInit();
void GLShutdown();

HWND  hWnd;
HGLRC hRC;
HMENU hMenu;
Field *GameField = new Field();

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR lpCmdLine, int nShowCmd)
{
	srand(time(0));
	GameField->Game->Inception();
	MessageBoxW(hWnd, L"Добро пожаловать в игру Stack!\nНажмите Enter, чтобы начать игру.\nSpace - опустить блок.\nПравую кнопку мыши - открыть контекстное меню.", L"Stack", MB_OK);
	if (!RegClass(hInst))
	{
		MessageBox(0, L"Не могу зарегистрировать класс окна...", 0, 0);
		return 1;
	}
	hWnd = CreateWnd(hInst);
	if (hWnd == NULL)
	{
		MessageBox(0, L"Не могу создать окно...", 0, 0);
		return 1;
	}
	GLInit();
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

WORD RegClass(HINSTANCE hInst)
{
	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = 0;
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hInstance = hInst;
	wc.hIcon = LoadIconW(hInst, MAKEINTRESOURCE(IDI_ICON1));
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = GameField->TitleGame;
	wc.lpszMenuName = 0;
	wc.style = 0;
	return RegisterClass(&wc);
}

HWND CreateWnd(HINSTANCE hInst)
{
	return CreateWindowW(GameField->TitleGame, GameField->TitleGame,
		WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX,
		(GetSystemMetrics(SM_CXSCREEN) -600) / 2,  
		(GetSystemMetrics(SM_CYSCREEN) - 600) / 2,
		600, 600,
		0, 0, hInst, 0);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	switch (msg)
	{
	case WM_COMMAND:
	{
		int wId = LOWORD(wParam);
		switch (wId)
		{
			case 1: {GameField->Game->Params->Sound = !GameField->Game->Params->Sound; break; }
			case 2: {GameField->Game->Params->_3D = !GameField->Game->Params->_3D; break; }
		}
	}
	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		GameField->GLRenderScene();
		EndPaint(hWnd, &ps);
		break;
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO *MinMax = reinterpret_cast<MINMAXINFO*>(lParam);
		MinMax->ptMaxTrackSize.x = 600;
		MinMax->ptMaxTrackSize.y = 600;
		break;
	}
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
			case VK_SPACE: {if (GameField->Game->Params->Game) Stop(); break; }
			case VK_RETURN:
				if (GameField->Game->Params->Game == false)
				{
					GameField->Game->Params->Game = true;
					GameField->Game->Start();
					SetTimer(hWnd, 1, GameField->Game->Params->Speed(), From_Right_To_Left);
				}
				break;
		}
		break;
	}
	case WM_CONTEXTMENU:
	{
		hMenu = CreatePopupMenu();
		TCHAR *Sound = L"Звук +";
		TCHAR *_3D = L"3D +";
		if (!GameField->Game->Params->Sound) Sound = L"Звук -";
		if (!GameField->Game->Params->_3D) _3D = L"3D -";
		AppendMenuW(hMenu, MFT_STRING, 1, Sound);
		AppendMenu(hMenu, MFT_SEPARATOR, 0, NULL);
		AppendMenu(hMenu, MFT_STRING, 2, _3D);
		TrackPopupMenu(hMenu, TPM_RIGHTBUTTON |TPM_TOPALIGN |TPM_LEFTALIGN,LOWORD(lParam),HIWORD(lParam), 0, hWnd, NULL);
		DestroyMenu(hMenu);
		break;
	}
	case WM_DESTROY:
		GLShutdown();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	}
	return 0;
}

void Stop()
{
	KillTimer(hWnd, 1);
	switch (GameField->Check())
	{
		case 0: SetTimer(hWnd, 1, GameField->Game->Params->Speed(), From_Left_To_Right); break;
		case 1: SetTimer(hWnd, 1, GameField->Game->Params->Speed(), From_Right_To_Left); break;
		case -1: Restart(); break;
	}
}

VOID CALLBACK From_Right_To_Left(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	if (GameField->Game->Params->Up)
	{
		GameField->Game->Params->X = XPARAM; GameField->Game->Params->Y = YPARAM;
		GameField->GLRenderScene();
		if (GameField->Game->Now.East[0] > 1) GameField->Game->Params->Up = !GameField->Game->Params->Up;
	}
	else
	{
		GameField->Game->Params->X = -XPARAM; GameField->Game->Params->Y = -YPARAM;
		GameField->GLRenderScene();
		if (GameField->Game->Now.West[0] <-1) GameField->Game->Params->Up = !GameField->Game->Params->Up;
	}
}

VOID CALLBACK From_Left_To_Right(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	if (GameField->Game->Params->Up)
	{
		GameField->Game->Params->X = -XPARAM; GameField->Game->Params->Y = YPARAM;
		GameField->GLRenderScene();
		if (GameField->Game->Now.West[0] <-1) GameField->Game->Params->Up = !GameField->Game->Params->Up;
	}
	else
	{
		GameField->Game->Params->X = XPARAM; GameField->Game->Params->Y = -YPARAM;
		GameField->GLRenderScene();
		if (GameField->Game->Now.East[0] > 1) GameField->Game->Params->Up = !GameField->Game->Params->Up;
	}
}

void GLShutdown()
{
	if (hRC != NULL)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hRC);
		hRC = NULL;
	}

	if (GameField->hDC != NULL)
	{
		ReleaseDC(hWnd, GameField->hDC);
		GameField->hDC = NULL;
	}
}

void GLInit()
{
	GameField->hDC = GetDC(hWnd);
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 16;
	pfd.cDepthBits = 16;
	int iPixelFormat = ChoosePixelFormat(GameField->hDC, &pfd);
	PIXELFORMATDESCRIPTOR bestMatch_pfd;
	DescribePixelFormat(GameField->hDC, iPixelFormat, sizeof(pfd), &bestMatch_pfd);
	SetPixelFormat(GameField->hDC, iPixelFormat, &pfd);
	hRC = wglCreateContext(GameField->hDC);
	wglMakeCurrent(GameField->hDC, hRC);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

void Restart()
{
	std::wstring msg = L"Увы, вы проиграли.\nФинальный счет: " + std::to_wstring(GameField->Game->Params->Total)+L"\nНажмите Enter, чтобы начать заново.";
	MessageBoxW(hWnd, msg.c_str(), L"Stack", MB_OK);
	GameField->Game->Params->NewGame();
	GameField->Game->List.clear();
	glClearColor(1.0, 1.0, 1.0, 1.0);
	GameField->Game->Inception();
	GameField->Game->Clear();
	GameField->GLRenderScene();
}
