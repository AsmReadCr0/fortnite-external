#pragma once
#include <core/includes.hpp>

struct a_world_data
{
	uintptr_t gworld = null;
	uintptr_t game_instance = null;
	uintptr_t game_state = null;
	uintptr_t local_player = null;
	uintptr_t local_pawn = null;
	uintptr_t player_controller = null;
	uintptr_t player_state = null;
	uintptr_t root_component = null;
	std::vector<std::uintptr_t> player_array;
}; inline std::unique_ptr<a_world_data> world_data = std::make_unique<a_world_data>();