#pragma once
#include <cstdint>

struct Color {
	constexpr Color(float r, float g, float b, float a = 1.f) noexcept :
		r(r), g(g), b(b), a(a) {  }

	float r, g, b, a;
};

namespace offset {
	//you will need to update your offsets everytime CS:GO updates. new offsets can be found:
	//https://raw.githubusercontent.com/frk1/hazedumper/master/csgo.hpp
	constexpr std::uintptr_t dwLocalPlayer = 0xDB65DC;
	constexpr std::uintptr_t dwForceJump = 0x527C38C;
	constexpr std::uintptr_t dwClientState = 0x58CFC4;
	constexpr std::uintptr_t dwGlowObjectManager = 0x531B048;
	constexpr std::uintptr_t dwEntityList = 0x4DD245C;
	constexpr std::uintptr_t dwClientState_State = 0x108;
	constexpr std::uintptr_t dwClientState_ViewAngles = 0x4D90;
	constexpr std::uintptr_t dwClientState_GetLocalPlayer = 0x180;
	constexpr std::uintptr_t m_vecViewOffset = 0x108;
	//some offsets arent used because i deleted some functions before publishing
	constexpr std::uintptr_t m_dwBoneMatrix = 0x26A8;
	constexpr std::uintptr_t m_aimPunchAngle = 0x303C;
	constexpr std::uintptr_t m_iHealth = 0x100;
	constexpr std::uintptr_t m_fFlags = 0x104;
	constexpr std::uintptr_t m_iTeamNum = 0xF4;
	constexpr std::uintptr_t m_iDefaultFOV = 0x333C;
	constexpr std::uintptr_t m_iFOV = 0x31F4;
	constexpr std::uintptr_t m_iGlowIndex = 0x10488;
	constexpr std::uintptr_t m_bIsDefusing = 0x997C;
	constexpr std::uintptr_t m_clrRender = 0x70;
	constexpr std::uintptr_t m_bDormant = 0xED;
	constexpr std::uintptr_t m_bSpotted = 0x93D;
	constexpr std::uintptr_t m_vecOrigin = 0x138;

	constexpr std::uintptr_t model_ambient_min = 0x59003C;

}

namespace globals {
	inline bool glow = false;
	inline bool radar = false;
	inline bool bhop = false;
	inline bool aimbot = false;

	namespace functions {
		struct Vec3 {
			float x, y, z;
		};
		int getEntityTeam(const auto entity)
		{
			return *(int*)(entity + offset::m_iTeamNum);
		}
		bool isTeammate(const auto entity, const auto localplayer, std::uintptr_t offset)
		{
			if (functions::getEntityTeam(entity) != *(uintptr_t*)(localplayer + offset))
				return false;

			return true;
		}
	}
}

namespace Game {
	const auto Client = reinterpret_cast<std::uintptr_t>(GetModuleHandle("client.dll"));
	const auto LocalPlayer = *reinterpret_cast<std::uintptr_t*>(Client + offset::dwLocalPlayer);
	const auto Health = *reinterpret_cast<std::int32_t*>(LocalPlayer + offset::m_iHealth);
	const auto Flags = *reinterpret_cast<std::int32_t*>(LocalPlayer + offset::m_fFlags);
}
