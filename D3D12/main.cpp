#include <C:\WinSDK\Include\10.0.15063.0\um\Windows.h>
#include <C:\WinSDK\Include\10.0.15063.0\shared\dxgi1_2.h>
#include <C:\WinSDK\Include\10.0.15063.0\um\d3dcompiler.h>
#include <C:\WinSDK\Include\10.0.15063.0\um\d3dx12.h>
#include <C:\WinSDK\Include\10.0.15063.0\um\d3d12.h>
#pragma comment (lib,"C:\\WinSDK\\Lib\\10.0.15063.0\\um\\x64\\dxgi.lib")
#pragma comment (lib,"d3dcompiler.lib")

#define Assert(x) \
	if(!(x)) {MessageBoxA(0, #x, "Assertion Failed", MB_OK); __debugbreak();}

#define Check(x) \
	if(!(x)) {MessageBoxA(0, #x, "Check Failed", MB_OK); __debugbreak();}

#define CheckSucceeded(hresult) \
	Check(SUCCEEDED(hresult))

bool quit=false;
ID3D12Debug *debug_interface;
ID3D12Device *device;
IDXGISwapChain1 *swap_chain;
IDXGIFactory2 *dxgi_factory;
ID3D12CommandQueue *graphics_command_queue;
ID3D12CommandAllocator *command_allocator;
ID3D12RootSignature *root_signature;

enum {
	FRAMES = 3
};

LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
	switch (message) {
		case WM_DESTROY: 
			PostQuitMessage(0x0012); 
			quit = true;
			return 0;
		default: return DefWindowProc(window, message, wparam, lparam);
	}
	
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmd, int nCmdShow) {
	WNDCLASSA window_class = { 0 };
	window_class.lpfnWndProc = WindowProc;
	window_class.lpszClassName = "d3d12test";
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClassA(&window_class);

	RECT rect = { 0,0,1680,1050 };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);
	HWND windowshandle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "d3d12test", "DirectX", WS_OVERLAPPEDWINDOW, 100, 100,
		rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, 0);
	ShowWindow(windowshandle, SW_SHOW);
	HRESULT hres;

	hres = D3D12GetDebugInterface(IID_PPV_ARGS(&debug_interface));
	CheckSucceeded(hres);
	debug_interface->EnableDebugLayer();

	hres = CreateDXGIFactory(IID_PPV_ARGS(&dxgi_factory));
	CheckSucceeded(hres);

	hres = D3D12CreateDevice(0, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device));
	CheckSucceeded(hres);

	D3D12_COMMAND_QUEUE_DESC graphics_command_queue_desc = {};
	graphics_command_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	hres = device->CreateCommandQueue(&graphics_command_queue_desc, IID_PPV_ARGS(&graphics_command_queue));
	CheckSucceeded(hres);

	DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = { 0 };
	swap_chain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.BufferCount = FRAMES;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	hres = dxgi_factory->CreateSwapChainForHwnd(graphics_command_queue, windowshandle, &swap_chain_desc, 0, 0, &swap_chain);
	CheckSucceeded(hres);

	hres = device->CreateCommandAllocator(graphics_command_queue_desc.Type, IID_PPV_ARGS(&command_allocator));
	CheckSucceeded(hres);

	D3D12CompileFromFile("C:\\pa\\shaders.hlsl")

	CD3DX12_ROOT_SIGNATURE_DESC root_signature_desc;
	root_signature_desc.Init(0, 0, 0, 0, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	ID3D10Blob *root_signature_blob;
	ID3D10Blob *root_signature_error_blob;
	hres = D3D12SerializeRootSignature(&root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1, &root_signature_blob, &root_signature_error_blob);
	CheckSucceeded(hres);

	hres = device->CreateRootSignature(0, root_signature_blob->GetBufferPointer(), root_signature_blob->GetBufferSize(), IID_PPV_ARGS(&root_signature));
	CheckSucceeded(hres);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeline_state_desc = { 0 };
	pipeline_state_desc.pRootSignature = root_signature;
	pipeline_state_desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	pipeline_state_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//pipeline_state_desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	pipeline_state_desc.SampleMask = UINT_MAX;
	pipeline_state_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pipeline_state_desc.NumRenderTargets = 1;
	pipeline_state_desc.RTVFormats[0] = swap_chain_desc.Format;
	pipeline_state_desc.SampleDesc.Count = 1;
	
	ID3D12PipelineState *pipeline_state;
	hres = device->CreateGraphicsPipelineState(&pipeline_state_desc, IID_PPV_ARGS(&pipeline_state));
	CheckSucceeded(hres);

	MSG message;
	message.message = WM_NULL;

	while (message.message != WM_QUIT){
		if (PeekMessage(&message, windowshandle, 0, 0, PM_REMOVE)) {
			//TranslateMessage(&message);
			DispatchMessage(&message);
		}

		ID3D12GraphicsCommandList *command_list;
		hres = device->CreateCommandList(0, graphics_command_queue_desc.Type, command_allocator, 0, IID_PPV_ARGS(&command_list));
		CheckSucceeded(hres);
		
		//swap_chain->GetBuffer(frame %3, IID_PPV_ARGS())

		command_list->Close();
		ID3D12CommandList *command_lists[] = { command_list };
		graphics_command_queue->ExecuteCommandLists(1, command_lists);

		swap_chain->Present(0, 0);
		command_list->Release();
		if (quit == true) {
			break;
		}
	}
	
	
	

	return 0;
}