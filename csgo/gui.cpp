#include "gui.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include <stdexcept>
#include <thread>

#include "globals.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam
);

LRESULT CALLBACK WindowProcess(
	HWND window, UINT message, WPARAM wide, LPARAM longg
);

bool Gui::setupWindowClass(const char* windowClassName) noexcept
{
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = DefWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = windowClassName;
	windowClass.hIconSm = 0;

	if (!RegisterClassEx(&windowClass))
		return false;

	return true;
}
void Gui::destroyWindowClass() noexcept
{
	UnregisterClass(
		windowClass.lpszClassName,
		windowClass.hInstance
	);
}

bool Gui::setupWindow(const char* windowName) noexcept
{
	window = CreateWindow(
		windowClass.lpszClassName,
		windowName,
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		10,
		10,
		0,
		0,
		windowClass.hInstance,
		0
	);

	if (!window)
		return false;

	return true;
}
void Gui::destroyWindow() noexcept
{
	if (window)
		DestroyWindow(window);
}

bool Gui::setupDirectX() noexcept
{
	const auto handle = GetModuleHandle("d3d9.dll");

	if (!handle)
		return false;

	using CreateFn = LPDIRECT3D9(__stdcall*)(UINT);

	const auto create = reinterpret_cast<CreateFn>(GetProcAddress(handle, "Direct3DCreate9"));
	if (!create)
		return false;

	d3d9 = create(D3D_SDK_VERSION);

	if (!d3d9)
		return false;

	D3DPRESENT_PARAMETERS params = {  };
	params.BackBufferWidth = 0;
	params.BackBufferHeight = 0;
	params.BackBufferFormat = D3DFMT_UNKNOWN;
	params.BackBufferCount = 0;
	params.MultiSampleType = D3DMULTISAMPLE_NONE;
	params.MultiSampleQuality = 0;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.hDeviceWindow = window;
	params.Windowed = 1;
	params.EnableAutoDepthStencil = 0;
	params.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	params.Flags = 0;
	params.FullScreen_RefreshRateInHz = 0;
	params.PresentationInterval = 0;

	if (d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, window, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT, &params, &device) < 0) return false;

	return true;
}
void Gui::DestroyDX() noexcept
{
	if (device)
	{
		device->Release();
		device = NULL;
	}

	if (d3d9)
	{
		d3d9->Release();
		d3d9 = NULL;
	}
}

void Gui::Core() 
{
	if (!setupWindowClass("class1"))
		throw std::runtime_error("Failed to create window class!");

	if (!setupWindow("Imperious"))
		throw std::exception("failed to create window / SF");

	if (!setupDirectX())
		throw std::runtime_error("failed to create device");

	destroyWindow();
	destroyWindowClass();
}

void Gui::SetupMenu(LPDIRECT3DDEVICE9 device) noexcept
{
	auto params = D3DDEVICE_CREATION_PARAMETERS{ };
	device->GetCreationParameters(&params);

	window = params.hFocusWindow;
	orginalWindowProcess = reinterpret_cast<WNDPROC>(
		SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcess))
	);

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);

	setup = true;
}
void Gui::Destroy() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	SetWindowLongPtr(
		window,
		GWLP_WNDPROC,
		reinterpret_cast<LONG_PTR>(orginalWindowProcess)
	);

	DestroyDX();
}
void Gui::Render() noexcept
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Imperious", &open);

	if (ImGui::BeginTabBar("Imperious"))
	{
		if (ImGui::BeginTabItem("Aimbot"))
		{
			if (ImGui::Checkbox("Aimbot", &globals::aimbot));

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Visuals"))
		{
			if (ImGui::Checkbox("Glow ESP", &globals::glow));
			if (ImGui::Checkbox("Radar", &globals::radar));

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Misc"))
		{
			if (ImGui::Checkbox("BHop", &globals::bhop));

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	
	ImGui::End();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

LRESULT CALLBACK WindowProcess(
	HWND window, UINT message, WPARAM wide, LPARAM longg
) {
	if (GetAsyncKeyState(VK_INSERT) & 1)
		Gui::open = !Gui::open;

	if (Gui::open && ImGui_ImplWin32_WndProcHandler(
		window,
		message,
		wide,
		longg
	)) return 1L;


	return CallWindowProc(
		Gui::orginalWindowProcess,
		window,
		message,
		wide,
		longg
	);
}