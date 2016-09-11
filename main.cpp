#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#include <windows.h>

#include "Graphics\Primatives.h"

#include "Graphics\GraphicsEngine.h"

#include "Game Objects\GameController.h"

// define the screen resolution and keyboard macros
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
HWND CreateWindowAux( HINSTANCE hinstance, unsigned int screen_width, unsigned int screen_height );
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND console_handle = NULL;

void OpenDebugConsole( void )
{
	AllocConsole();
	SetConsoleTitle( "Debuf Information" );

	int hConHandle;
	long lStdHandle;
	FILE *fp;

	// redirect unbuffered STDOUT to the console
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stdout = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );

	// redirect unbuffered STDIN to the console
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "r" );
	*stdin = *fp;
	setvbuf( stdin, NULL, _IONBF, 0 );

	// redirect unbuffered STDERR to the console
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stderr = *fp;
	setvbuf( stderr, NULL, _IONBF, 0 );

}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
	OpenDebugConsole();

	//create the main program window and display it
	HWND hWnd = CreateWindowAux( hInstance, 800, 600 ); 

	GameController gc;
	gc.Initilize( hWnd, hInstance );

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	unsigned int prev_time = GetTickCount();
	unsigned int fcount = 0;

	while(TRUE)
	{
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if(msg.message == WM_QUIT)
			break;
		// check the 'escape' key to end the program
		if(KEY_DOWN(VK_ESCAPE))
		{
			PostMessage(hWnd, WM_QUIT, 0, 0);
		}

		//Graphics.RenderScene();
		//gc.GraphicsEngine.RenderScene();
		gc.Update( 0 );

		++fcount;
		if( fcount == 10 )
		{
			fcount = 0;
			unsigned int cur_time = GetTickCount();
			float diff = float(cur_time - prev_time);
			diff /= 10.f;
			if( diff < 1.f ) continue;
			float fps = diff / 1000.f;
			fps = 1.f / fps;
			char fpsstr[16];
			sprintf_s(fpsstr, sizeof("fps: xxxx.xx"), "fps: %4.2f", fps);
			SetWindowText( hWnd, fpsstr );
			prev_time = cur_time;
		}
	}

	//UnregisterClass( "GameWindow", hInstance );
	return 0;
}


HWND CreateWindowAux( HINSTANCE hinstance, unsigned int screen_width, unsigned int screen_height )
{
  WNDCLASSEX wc;

  ZeroMemory(&wc, sizeof(WNDCLASSEX));

  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = hinstance;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.lpszClassName = "GameWindow";

  RegisterClassEx(&wc);

  return CreateWindowEx( NULL,
                        "GameWindow",
                        "Game",
                        WS_EX_TOPMOST | WS_OVERLAPPEDWINDOW, //| WS_POPUP,    // fullscreen values
                        0, 0,    // the starting x and y positions should be 0
                        screen_width, screen_height,    // set the window to 640 x 480
                        NULL,
                        NULL,
                        hinstance,
                        NULL);
}


LRESULT CALLBACK WindowProc( HWND   hWnd, 
							 UINT   msg, 
							 WPARAM wParam, 
							 LPARAM lParam )
{
    switch( msg )
	{       
        case WM_KEYDOWN:
		{
			//switch( wParam )
			//{
			//}
			break;
		}       
		case WM_KEYUP:
		{
			switch( wParam )
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
			}
			break;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);	
			break;
		}
        case WM_DESTROY:
		{
            PostQuitMessage(0);
			break;
		}
		default:
		{
			return DefWindowProc( hWnd, msg, wParam, lParam );
		}
	}
	return 0;
}