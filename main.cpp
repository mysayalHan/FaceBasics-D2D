#include"stdafx.h"
#include"ImageThread.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
#ifdef _DEBUG
	AllocConsole();
	_cwprintf(L"Battle Control  Online! \n");
#endif

	if (SUCCEEDED(CoInitialize(NULL)))
	{
		{
			ImageThread main_thread;
			main_thread.run();
		}
		CoUninitialize();
	}
#ifdef _DEBUG
	_cwprintf(L"Battle Control Terminated! \n");
	FreeConsole();
#endif
	return 0;

}
