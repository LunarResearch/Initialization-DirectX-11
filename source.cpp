#include <d3d11.h>
#include <wrl\client.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dxgi.lib")

using namespace Microsoft::WRL;

ComPtr<ID3D11Device> device;
ComPtr<ID3D11DeviceContext> context;
ComPtr<ID3D11RenderTargetView> renderTarget;

ComPtr<IDXGIFactory> factory;
ComPtr<IDXGISwapChain> swapChain;

void KeyDown(UINT8 key) {
	if (GetAsyncKeyState(VK_ESCAPE))
		exit(0);
}

void KeyUp(UINT8 key) {

}

void Clear() {
	device.ReleaseAndGetAddressOf();
	context.ReleaseAndGetAddressOf();
	renderTarget.ReleaseAndGetAddressOf();
	factory.ReleaseAndGetAddressOf();
	swapChain.ReleaseAndGetAddressOf();
}

void Update() {

}

void Render() {
	float Color[4] = { 0.4f, 0.6f, 0.9f, 1.0f };
	context->ClearRenderTargetView(renderTarget.Get(), Color);
	swapChain->Present(1, 0);
}

void Init(_In_ HWND hWnd) {
#if defined(DEBUG) | defined(_DEBUG)
	UINT flag = D3D11_CREATE_DEVICE_DEBUG;
#else
	UINT flag = D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#endif

	D3D_FEATURE_LEVEL feat_11_0 = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL feat_10_1 = D3D_FEATURE_LEVEL_10_1;
	D3D_FEATURE_LEVEL feat_10_0 = D3D_FEATURE_LEVEL_10_0;
	D3D_FEATURE_LEVEL feat_9_3 = D3D_FEATURE_LEVEL_9_3;
	D3D_FEATURE_LEVEL feat_9_2 = D3D_FEATURE_LEVEL_9_2;
	D3D_FEATURE_LEVEL feat_9_1 = D3D_FEATURE_LEVEL_9_1;

	if (SUCCEEDED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flag,
		&feat_11_0, 1, D3D11_SDK_VERSION, &device, nullptr, &context)))
		goto Continue;
	else if (SUCCEEDED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flag,
		&feat_10_1, 1, D3D11_SDK_VERSION, &device, nullptr, &context)))
		goto Continue;
	else if (SUCCEEDED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flag,
		&feat_10_0, 1, D3D11_SDK_VERSION, &device, nullptr, &context)))
		goto Continue;
	else if (SUCCEEDED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flag,
		&feat_9_3, 1, D3D11_SDK_VERSION, &device, nullptr, &context)))
		goto Continue;
	else if (SUCCEEDED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flag,
		&feat_9_2, 1, D3D11_SDK_VERSION, &device, nullptr, &context)))
		goto Continue;
	else if (SUCCEEDED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flag,
		&feat_9_1, 1, D3D11_SDK_VERSION, &device, nullptr, &context)))
		goto Continue;
	else {
		MessageBox(NULL, "Your GPU doesn't support D3D_FEATURE_LEVEL_9_1 or higher.",
			"Error", MB_ICONERROR);
		exit(-1);
	}
	
Continue:
	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferCount = 2;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.OutputWindow = hWnd;
	desc.SampleDesc.Count = 1;
	desc.Windowed = TRUE;

	CreateDXGIFactory1(IID_PPV_ARGS(&factory));
	factory->CreateSwapChain(device.Get(), &desc, &swapChain);

	ComPtr<ID3D11Resource> resource;
	swapChain->GetBuffer(0, IID_PPV_ARGS(&resource));
	device->CreateRenderTargetView(resource.Get(), nullptr, &renderTarget);
	resource.ReleaseAndGetAddressOf();
}

LRESULT CALLBACK WindowProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) {
	switch (uMsg) {
	case WM_CREATE:
		Init(hWnd);
		return 0;

	case WM_KEYDOWN:
		KeyDown(static_cast<UINT8>(wParam));
		return 0;

	case WM_KEYUP:
		KeyUp(static_cast<UINT8>(wParam));
		return 0;

	case WM_PAINT:
		Update();
		Render();
		return 0;

	case WM_DESTROY:
		Clear();
		PostQuitMessage(0);
		return 0;
			
	default: 
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "WindowClass";
	RegisterClassEx(&wc);
	HWND hWnd = CreateWindow(wc.lpszClassName, "Test DirectX 11 for D3D11CreateDevice",
		WS_SYSMENU | WS_MINIMIZEBOX,
		(GetSystemMetrics(SM_CXSCREEN) - 1024) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - 576) / 2,
		1024, 576, nullptr, nullptr, hInstance, 0);
	ShowWindow(hWnd, nCmdShow);	
	MSG msg = {};
	while (msg.message != WM_QUIT)
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	return static_cast<char>(msg.wParam);
}
