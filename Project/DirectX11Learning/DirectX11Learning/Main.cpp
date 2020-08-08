#include <Windows.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);

	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = DefWindowProc;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = L"Belajar Direct X 11";

	if (!RegisterClassEx(&wndClass))
	{
		OutputDebugStringW(L"Error RegisterClassEx NULL");
		return -1;
	}
	
	RECT rc = { 0, 0, 640, 480 };	
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);	
	HWND hwnd = CreateWindowA("Belajar Direct X 11", "Belajar Direct X 11",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.
		left,
		rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
	
	if (!hwnd)
	{
		OutputDebugStringW(L"Error hwnd NULL");
		return -1;
	}

	ShowWindow(hwnd, cmdShow);
	// Demo Initialize
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// Update
		// Draw
	}
	// Demo Shutdown
	return static_cast<int>(msg.wParam);
}