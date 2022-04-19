#include "hooks.h"
#include <stdexcept>
#include <intrin.h>
#include <thread>

#include "minhook/MinHook.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"

void hooks::Setup() /* minhook initialization */
{
	if (MH_Initialize()) /* if it successfully initializes it returns 0 */
		throw std::runtime_error("failed to init minhook!");

	if (MH_CreateHook(VirtualFunction(Gui::device, 42), &EndScene,
		reinterpret_cast<void**>(&EndSceneOrg))) throw std::runtime_error("unable to hook EndScene");

	if (MH_CreateHook(VirtualFunction(Gui::device, 16), &Reset,
		reinterpret_cast<void**>(&ResetOrg))) throw std::runtime_error("unable to hook Reset");

	if (MH_EnableHook(MH_ALL_HOOKS))
		throw std::runtime_error("hookfunction error");

	Gui::DestroyDX();
}

void hooks::Destroy() noexcept
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

long __stdcall hooks::EndScene(IDirect3DDevice9* device) noexcept
{
	static const auto returnAddr = _ReturnAddress();
	const auto result = EndSceneOrg(device, device);

	if (_ReturnAddress() == returnAddr)
		return result;

	if (!Gui::setup)
		Gui::SetupMenu(device);

	if (Gui::open)
		Gui::Render();

	return result;
}

HRESULT __stdcall hooks::Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto result = ResetOrg(device, device, params);
	ImGui_ImplDX9_CreateDeviceObjects();

	return result;
}