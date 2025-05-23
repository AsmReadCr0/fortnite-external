#include <core/thread/entity/a_entity.hpp>
#include <core/thread/world.hpp>
#include <core/thread/entity.hpp>

#include <dependencies/driver/driver.hpp>

inline std::string get_player_username( uintptr_t state, bool is_lobby )
{
	int pNameLength;
	WORD* pNameBufferPointer;
	int i;
	char v25;
	int v26;
	int v29;

	char16_t* pNameBuffer;

	uintptr_t pNameStructure = io->read<uintptr_t>(state + 0xb08);

	pNameLength = io->read<int>(pNameStructure + 0x10);
	if (pNameLength <= 0)
		return hash_str("null");

	pNameBuffer = new char16_t[pNameLength];

	uintptr_t pNameEncryptedBuffer = io->read<uintptr_t>(pNameStructure + 0x8);
	io->read_physical(reinterpret_cast<PVOID>(pNameEncryptedBuffer), pNameBuffer, pNameLength * sizeof(wchar_t));
	v25 = pNameLength - 1;
	v26 = 0;
	pNameBufferPointer = reinterpret_cast<WORD*>(pNameBuffer);
	for (i = (v25) & 3;; *pNameBufferPointer++ += i & 7)
	{
		v29 = pNameLength - 1;
		if (!(DWORD)pNameLength)
			v29 = 0;

		if (v26 >= v29)
			break;

		i += 3;
		++v26;
	}

	std::u16string temp_wstring(pNameBuffer);
	delete[] pNameBuffer;

	return std::string(temp_wstring.begin(), temp_wstring.end());
}

bool is_same_team( uintptr_t temp_player_state )
{
	if ( !world_data->local_pawn ) {
		return false;
	}

	auto local_team_index = io->read<int>( io->read<uintptr_t>( world_data->local_pawn + offsets->playerstate ) + offsets->teamindex );
	auto player_team_index = io->read<int>( temp_player_state + offsets->teamindex );

	return local_team_index == player_team_index;
}

void a_entity::run()
{
	std::vector<a_entity_data> entity_data_temp;

	std::unordered_map<uintptr_t, std::string> username_cache;
	std::unordered_map<uintptr_t, int32_t> rank_cache;
	std::unordered_map<uintptr_t, char> team_id_cache;

	std::mutex entity_data_mutex;

	bool in_lobby = false;
	bool previous_in_lobby = false;

	while (true) {
		for (auto& temp_player_state : world_data->player_array) {
			a_entity_data cached;

			if (!temp_player_state) continue;

			try {
				auto temp_pawn_private = io->read<uintptr_t>(temp_player_state + offsets->pawnprivate);
				if (!temp_pawn_private || temp_pawn_private == world_data->local_pawn) continue;

				auto temp_mesh = io->read<uintptr_t>(temp_pawn_private + offsets->mesh);
				if (!temp_mesh) continue;

				if (is_same_team(temp_player_state)) continue;

				if (g_vars->visuals.rank) {
					int32_t temp_rank_progress;

					auto it = rank_cache.find(temp_player_state);
					if (it != rank_cache.end()) {
						temp_rank_progress = it->second;
					}
					else {
						auto temp_habanero_component = io->read<uintptr_t>(temp_player_state + 0x9f0);
						temp_rank_progress = io->read<int32_t>(temp_habanero_component + 0xb8 + 0x10);
						rank_cache[temp_player_state] = temp_rank_progress;
					}

					cached.rank_progress = temp_rank_progress;
					rank_cache_size = rank_cache.size();
				}

				if (g_vars->visuals.team_id) {
					char temp_team_id;

					auto it = team_id_cache.find(temp_player_state);
					if (it != team_id_cache.end()) {
						temp_team_id = it->second;
					}
					else {
						temp_team_id = io->read<char>(temp_player_state + offsets->teamindex);
						team_id_cache[temp_player_state] = temp_team_id;
					}

					cached.team_id = temp_team_id;
				}

				if (g_vars->visuals.name)
				{
					std::string temp_username;

					auto it = username_cache.find(temp_player_state);
					if (it != username_cache.end()) {
						temp_username = it->second;
					}
					else {
						temp_username = get_player_username(temp_player_state, in_lobby);
						if (!temp_username.empty()) {
							username_cache[temp_player_state] = temp_username;
						}
						else {
							log_error();
							continue;
						}
					}

					cached.username = temp_username;
					username_cache_size = username_cache.size();
				}

				if (g_vars->visuals.name || g_vars->visuals.team_id || g_vars->visuals.rank)
				{
					auto server_time = io->read<float>(world_data->game_state + 0x2c8);
					in_lobby = (server_time == 0.0f);

					if (in_lobby != previous_in_lobby) {
						username_cache.clear();
						rank_cache.clear();
						team_id_cache.clear();
						previous_in_lobby = in_lobby;
					}
				}

				cached.player_state = temp_player_state;
				cached.pawn_private = temp_pawn_private;
				cached.mesh = temp_mesh;

				entity_data_temp.push_back(cached);
			}
			catch (const std::exception& ex) {
				log_error();
				continue;
			}
		}

		{
			std::lock_guard<std::mutex> lock(entity_data_mutex);
			entity_data.swap(entity_data_temp);
		}

		entity_data_temp.clear();
		entity_data_temp.reserve(world_data->player_array.size());

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}