//include windows header file.
#include<Windows.h>
#include<windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

//include Direct3D Library file.
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

IDXGISwapChain* swapChain;			//pointer untuk swap chain interface
ID3D11Device* dev;					//pointer ke arah Direct3D device interface
ID3D11DeviceContext* devcon;		//pointer ke arah Direct3D device context

//function prototype
void InitD3D(HWND hWnd);				//setting initialisasi dari direct3D
void CleanD3D(void);				//tutup Direct3D dan bersihin memory

//Ini adalah main handler untuk programnya.
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

	RECT wr = { 0,0,500,400 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	//Buat windows Extended.
	hWnd = CreateWindowEx(NULL,
		L"BelajarDirectX11",
		L"Direct X 11 Learn",
		WS_OVERLAPPEDWINDOW,
		100,
		100,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	//Tampilkan window.
	ShowWindow(hWnd, cmdShow);

	//initialisasi D3D
	InitD3D(hWnd);

	//Object pesan / MSG
	MSG msg;
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				//Game code disini.
			}
		}
	}

	//bersihin D3D dan COM
	CleanD3D();

	return msg.wParam;
}

//function untuk initialisasi Direct3D
void InitD3D(HWND hWnd)
{
	//membuat struct untuk menyimpan informasi swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	//bersihin memory.
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	//isi swap chain description struct.
	scd.BufferCount = 1;								//satu back buffer.
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //pakai 32-bit color.
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  //bagaimana pemakaian swap chain.
	scd.OutputWindow = hWnd;							//window yang akan dipakai.
	scd.SampleDesc.Count = 4;							//berapa banyak multisample.
	scd.Windowed = TRUE;								//windowed / fullscreen

	//buat device , device context dan swap chain dengan informasi yang ada di scd struct.
	D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapChain,
		&dev,
		NULL,
		&devcon);
}

void CleanD3D()
{
	//tutup dan buang semua COM object yang ada.
	swapChain->Release();
	dev->Release();
	devcon->Release();
}

