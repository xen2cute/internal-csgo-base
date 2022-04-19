#include <iostream>
#include <Windows.h>
#include <thread>
#include "globals.h"
#include "hooks.h"
#include "csgo.h"

void Imperious(const HMODULE module_) noexcept /* imperious WAS the name of the cheat */
{
	try
	{
		Gui::Core(); 
		hooks::Setup(); 
		std::thread(csgo::GlowThread).detach();
		std::thread(csgo::BHopThread).detach();
		//std::thread(csgo::AimbotThread).detach();
	}
	catch (const std::exception& err)
	{
		MessageBoxA(0, err.what(), "error", MB_OK | MB_ICONEXCLAMATION);
	}

	while (!GetAsyncKeyState(VK_END) & 2) /* you need to press END twice to uninject the cheat */
		std::this_thread::sleep_for(std::chrono::milliseconds(200));

	Beep(250, 400);
	hooks::Destroy();
	Gui::Destroy();
	FreeLibraryAndExitThread(module_, 0); /* uninject */
}

int __stdcall DllMain(const HMODULE module_, const std::uintptr_t c_reason, const void* lpReserved)
{
	if (c_reason == 1)
	{
		DisableThreadLibraryCalls(module_);
		const auto thread = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Imperious), module_, 0, nullptr);
		//Beep(400, 250);
		if (thread)
			CloseHandle(thread);
	}
	return 1;
}