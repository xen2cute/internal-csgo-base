#pragma once
#include <d3d9.h>

namespace Gui 
{
	inline bool open = true;
	inline bool setup = false;
	inline HWND window = nullptr;
	inline WNDCLASSEX windowClass = { };
	inline WNDPROC orginalWindowProcess = nullptr;
	inline LPDIRECT3DDEVICE9 device = nullptr;
	inline LPDIRECT3D9 d3d9 = nullptr;

	bool setupWindowClass(const char* windowClassName) noexcept;
	void destroyWindowClass() noexcept;

	bool setupWindow(const char* windowName) noexcept;
	void destroyWindow() noexcept;

	bool setupDirectX() noexcept;
	void DestroyDX() noexcept;

	void Core();

	void SetupMenu(LPDIRECT3DDEVICE9 device) noexcept;
	void Destroy() noexcept;

	void Render() noexcept;
}