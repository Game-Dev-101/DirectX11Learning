//include windows header file.
#include<Windows.h>
#include<windowsx.h>

//Window process prototype.
LRESULT CALLBACK WindowProc(HWND hWnd,
							UINT message,
							WPARAM wParam,
							LPARAM lParam)
{
	//Check message nya berisi apa. Bila sudah di destroy maka kita return
	//Bahwa program keluar dengan sempurna 0.
	switch (message)
	{
		case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}
		break;
	}

	//Pastikan tidak ada value yang terlewat.
	//Dengan DefWindowProc, kita bisa menetralisir semua value yang
	//terlewat dan diassign dengan 0.
	return DefWindowProc(hWnd, message, wParam, lParam);
}

// Entry point punya semua windows program.
int WINAPI WinMain( HINSTANCE hInstance,
					HINSTANCE prevInstance,
					LPSTR cmdLine,
					int cmdShow)
{
	//Handle milik window, diisi dengan function.
	HWND hWnd;

	//struct yang menyimpan informasi tentang window class.
	WNDCLASSEX wc;

	//clear memory yang dipakai window class.
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	//Isi informasi window class.
	wc.cbSize = sizeof(WNDCLASSEX); //Size dari window class.
	wc.style = CS_HREDRAW | CS_VREDRAW; //Style dari window. berfungsi untuk menggambar ulang.
	wc.lpfnWndProc = WindowProc; //Mendapatkan informasi dari process window.
	wc.hInstance = hInstance; //Dapatkan instance dari window.
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //Cursor yang akan di render di dalam window.
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW; //Gambar yang akan di 'brush' bila window digerakkan.
	wc.lpszClassName = L"BelajarDirectX11"; //Class name dari window class.

	//Register window class.
	RegisterClassEx(&wc);

	//Buat windows Extended.
	hWnd = CreateWindowEx(NULL,
		L"BelajarDirectX11",
		L"Direct X 11 Learn",
		WS_OVERLAPPEDWINDOW,
		300,
		300,
		500,
		300,
		NULL,
		NULL,
		hInstance,
		NULL);

	//Tampilkan window.
	ShowWindow(hWnd, cmdShow);

	//Object pesan / MSG
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

