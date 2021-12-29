#include "winapi.h"
#include "pongerr.h"

int WINAPI wWinMain(
	HINSTANCE hInst,
	HINSTANCE hPrevInst,
	PWSTR lpCmdArgs,
	int nCmdShow
)
{
	consciousUnused(hPrevInst);

	PongWnd_t window;
	if (PongWnd_create(&window, hInst, lpCmdArgs, nCmdShow) == false)
	{
		pongLastErr(&window);
		return 1;
	}

	int ret = PongWnd_msgLoop(&window);

	PongWnd_free(&window);
	return ret;
}
