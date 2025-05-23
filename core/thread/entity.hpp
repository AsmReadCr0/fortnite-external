#pragma once
#include <core/includes.hpp>

#include <core/sdk/engine/engine.hpp>

inline size_t username_cache_size = null;
inline size_t rank_cache_size = null;
inline size_t team_id_cache_size = null;

struct a_entity_data
{
	uintptr_t player_state = null;
	uintptr_t pawn_private = null;
	uintptr_t mesh = null;
	uengine::tarray< uengine::ftransform> bone_array;

	uengine::ftransform component_to_world;

	std::vector<uengine::ftransform> transform;

	int rank_progress = null;

	int team_id = null;

	std::string username;

	char is_bot = null;

	char dying = null;
	char downed = null;
	bool visible = null;
}; inline std::vector<a_entity_data> entity_data;

inline std::vector<std::pair<int, int>> bone_pairs = {
    {110, 66},
    {66, 9},
    {66, 38},
    {9, 10},
    {38, 39},
    {10, 11},
    {39, 40},
    {66, 2},
    {2, 78},
    {2, 71},
    {78, 79},
    {79, 82},
    {71, 72},
    {72, 75}
};