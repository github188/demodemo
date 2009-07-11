#include <windows.h>

int WinMain(
			HINSTANCE hInstance,
			HINSTANCE hPrevInstance,
			LPSTR lpCmdLine,
			int nCmdShow
			)
{
	MessageBox(NULL,
		TEXT("Hi, hello world."),	
		TEXT("Hello world."),
		MB_OK);
	return 0;
}