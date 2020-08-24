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

IDXGISwapChain* swapchain;			//pointer untuk swap chain interface
ID3D11Device* dev;					//pointer ke arah Direct3D device interface
ID3D11DeviceContext* devcon;		//pointer ke arah Direct3D device context
ID3D11RenderTargetView* backbuffer; //pointer ke arah backbuffer
ID3D11InputLayout* inputLayout;		
ID3D11VertexShader* VS;
ID3D11PixelShader* PS;
ID3D11Buffer* pVBuffer;

struct VERTEX { FLOAT x, y, z; D3DXCOLOR Color; };

//function prototype
void InitD3D(HWND hWnd);																//setting initialisasi dari direct3D
void CleanD3D(void);																	//tutup Direct3D dan bersihin memory
void RenderFrame(void);																	//render single frame
void InitGraphic(void);
void InitPipeline(void);

//WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

FLOAT SCREEN_WIDTH = 800;
FLOAT SCREEN_HEIGHT = 600;

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
	wc.cbSize = sizeof(WNDCLASSEX);				//Size dari window class.
	wc.style = CS_HREDRAW | CS_VREDRAW;			//Style dari window. berfungsi untuk menggambar ulang.
	wc.lpfnWndProc = WindowProc;				//Mendapatkan informasi dari process window.
	wc.hInstance = hInstance;					//Dapatkan instance dari window.
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	//Cursor yang akan di render di dalam window.
//	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;	//Gambar yang akan di 'brush' bila window digerakkan.
	wc.lpszClassName = TEXT("BelajarDirectX11");		//Class name dari window class.

	//Register window class.
	RegisterClassEx(&wc);

	RECT wr = { 0,0,500,400 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	//Buat windows Extended.
	hWnd = CreateWindowEx(NULL,
		TEXT("BelajarDirectX11"),
		TEXT("Direct X 11 Learn"),
		WS_OVERLAPPEDWINDOW,
		300,
		300,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
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

	//Window loop
	while (TRUE)
	{
		//idk why but this linker doesn't work even i already check that user32.lib already exist.
		//so for now, i'll just commenting this without blocking progress on the project.

//		DWORD old = timeGetTime();
//		DWORD delta, now = old;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

//			DWORD old = timeGetTime();
//			delta = now - old;

//			if (delta >= 1)
//				break;

			Sleep(1);

			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{				
				//Game code disini.
				RenderFrame();
			}
		}
	}

	//bersihin D3D dan COM
	CleanD3D();

	return msg.wParam;
}

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

//fungsi untuk render single frame.
void RenderFrame(void)
{
	//bersihin render buffer jadi warna
	devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.5f, 1.0f));

	//pilih vertex buffer yang ingin di display
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

	//set cara primitive topology
	devcon->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	//gambar segitiga
	devcon->Draw(3, 0);

	//switch back buffer dengan front buffer
	swapchain->Present(0, 0);
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
	scd.BufferDesc.Width = SCREEN_WIDTH;				//set back buffer width.
	scd.BufferDesc.Height = SCREEN_HEIGHT;				//set back buffer height.
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  //bagaimana pemakaian swap chain.
	scd.OutputWindow = hWnd;							//window yang akan dipakai.
	scd.SampleDesc.Count = 4;							//berapa banyak multisample.
	scd.Windowed = TRUE;								//windowed / fullscreen.
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //membolehkan full-screen switching.

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
		&swapchain,
		&dev,
		NULL,
		&devcon);

	//dapetin address dari backbuffer
	ID3D11Texture2D* pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	//pakai backbuffer address untuk membuat render target
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();

	//simpan render target di backbuffer
	devcon->OMSetRenderTargets(1, &backbuffer, NULL);

	//set viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Height = SCREEN_HEIGHT;
	viewport.Width = SCREEN_WIDTH;

	devcon->RSSetViewports(1, &viewport);

	InitPipeline();
	InitGraphic();
}

void CleanD3D()
{
	swapchain->SetFullscreenState(FALSE, NULL);

	//tutup dan buang semua COM object yang ada.
	inputLayout->Release();
	VS->Release();
	PS->Release();
	pVBuffer->Release();
	swapchain->Release();
	backbuffer->Release();
	dev->Release();
	devcon->Release();
}

void InitPipeline()
{
	//load dan compile dua shaders (vertex dan pixel)
	ID3D10Blob* vs_buffer, * ps_buffer;
	ID3D10Blob* vs_buffer_error, * ps_buffer_error;
	D3DX11CompileFromFile(TEXT("shaders.shader"), 0, 0, "VShader", "vs_4_0", 0, 0, 0, &vs_buffer, &vs_buffer_error, 0);
	D3DX11CompileFromFile(TEXT("shaders.shader"), 0, 0, "PShader", "ps_4_0", 0, 0, 0, &ps_buffer, &ps_buffer_error, 0);

	//check jika ada error menggunakan ppErrorMsgs (dalam CompileFromFile)
	if (vs_buffer_error != 0)
	{
		OutputDebugStringA((char*)vs_buffer_error->GetBufferPointer());
	}

	if (ps_buffer_error != 0)
	{
		OutputDebugStringA((char*)ps_buffer_error->GetBufferPointer());
	}


	//enkapsulasi kedua shader ke dalam shader objects
	dev->CreateVertexShader(vs_buffer->GetBufferPointer(), vs_buffer->GetBufferSize(), NULL, &VS);
	dev->CreatePixelShader(ps_buffer->GetBufferPointer(), ps_buffer->GetBufferSize(), NULL, &PS);

	devcon->VSSetShader(VS, 0, 0);
	devcon->PSSetShader(PS, 0, 0);

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	dev->CreateInputLayout(ied, 2, vs_buffer->GetBufferPointer(), vs_buffer->GetBufferSize(), &inputLayout);
	devcon->IASetInputLayout(inputLayout);

}

void InitGraphic()
{
	VERTEX ourVertices[] =
	{
		{0.0f,0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) },
		{0.45f,-0.5f, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) },
		{-0.45f,-0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f) }
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;				//tulis access untuk CPU dan GPU
	bd.ByteWidth = sizeof(ourVertices);			//ukuran dari ourVertices
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//digunakan untuk binding vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //memberikan akses ke CPU untuk di write buffer

	dev->CreateBuffer(&bd, NULL, &pVBuffer);	//buat buffer

	//copy vertices ke buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, ourVertices, sizeof(ourVertices));
	devcon->Unmap(pVBuffer, NULL);
}

