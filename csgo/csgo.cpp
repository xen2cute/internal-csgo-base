#include "csgo.h"
#include "gui.h"
#include "globals.h"
#include <thread>

void csgo::GlowThread()
{
	while (Gui::setup)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(1)); /* sleeps every 1ms to increase performance */

		for (int i = 1; i < 64; ++i) /* i is equal to the amount of players */
		{
			const auto Entity = *reinterpret_cast<std::uintptr_t*>(Game::Client + offset::dwEntityList + i * 0x10);
			const auto GlowObjectManager = *reinterpret_cast<std::uintptr_t*>(Game::Client + offset::dwGlowObjectManager);

			if (!Entity) /* if entity is invalid, it will break out of the loop */
				continue;

			if (globals::glow)
			{
				int glowIndex = *(int*)(Entity + offset::m_iGlowIndex);

				if (!globals::functions::isTeammate(Entity, Game::LocalPlayer, offset::m_iTeamNum)) /* team check */
					*(Color*)((GlowObjectManager + glowIndex * 0x38 + 0x8)) = Color{ 1.f, 0.f, 0.f }; /* will glow red for enemeys. see color struct in globals.h */

				*(bool*)((GlowObjectManager + glowIndex * 0x38 + 0x28)) = true;
				*(bool*)((GlowObjectManager + glowIndex * 0x38 + 0x29)) = false;
			}

			if (globals::radar)
			{
				*(bool*)((Entity + offset::m_bSpotted)) = true; /* this line speaks for itself */
			}
		}
	}
}

void csgo::BHopThread()
{
	while (Gui::setup)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(1)); /* another sleep */

		if (!Game::LocalPlayer) /* checks n shit */
			continue;

		if (!Game::Health) /* checks n shit */
			continue;

		if (globals::bhop)
		{
			if ((GetAsyncKeyState(VK_SPACE) & 1) && (Game::Flags & 1))
			{
				/* idk why this "method" for bhop doesn't feel as good as some other cheats */
				*(DWORD*)(Game::Client + offset::dwForceJump) = 6;
			}
		}
	}
}

void csgo::AimbotThread()
{
	while (Gui::setup)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(1));

		/* insert code for aimbot here */
	}
}