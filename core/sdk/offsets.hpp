#pragma once

class offset
{
public:
	static constexpr std::uintptr_t gworld = 0x1771c988;

	static constexpr std::uintptr_t gameinstance = 0x208;

	static constexpr std::uintptr_t localplayers = 0x38;

	static constexpr std::uintptr_t playercontroller = 0x30;

	static constexpr std::uintptr_t acknowledgedpawn = 0x350;

	static constexpr std::uintptr_t playerstate = 0x2c8;

	static constexpr std::uintptr_t root_component = 0x1b0;

	static constexpr std::uintptr_t character_movement = 0x318;

	static constexpr std::uintptr_t velocity = 0x180;

	static constexpr std::uintptr_t gamestate = 0x190;

	static constexpr std::uintptr_t mesh = 0x328;

	static constexpr std::uintptr_t c2w = 0x1E0;

	static constexpr std::uintptr_t pawnprivate = 0x320;

	static constexpr std::uintptr_t playerarray = 0x2c0;

	static constexpr std::uintptr_t teamindex = 0x1259;

	static constexpr std::uintptr_t habanero_component = 0xa48;

	static constexpr std::uintptr_t killscore = 0x1224;

	static constexpr std::uintptr_t b_is_dying = 0x720;

	static constexpr std::uintptr_t b_is_dbno = 0x962;

	static constexpr std::uintptr_t b_is_bot = 0x29a;

	static constexpr std::uintptr_t b_is_reloading = 0x3b9;

	static constexpr std::uintptr_t current_weapon = 0x9d8;

	static constexpr std::uintptr_t weapon_data = 0x510;

	static constexpr std::uintptr_t item_name = 0x40;

	static constexpr std::uintptr_t bone_array = 0x5C8;

	static constexpr std::uintptr_t bone_cache = 0x590;

	static constexpr std::uintptr_t targetedfortpawn = 0x18A8;
};

inline std::unique_ptr<offset> offsets = std::make_unique<offset>( );