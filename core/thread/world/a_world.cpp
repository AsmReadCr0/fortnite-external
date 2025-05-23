#include <core/thread/world/a_world.hpp>

#include <core/thread/world.hpp>

#include <core/includes.hpp>

#include <functional>
#include <dependencies/driver/driver.hpp>
#include <core/sdk/utilities/utilities.hpp>

class timer {
public:
	timer( int wait_time, std::function<void( )> task )
		: wait_time( wait_time ), task( std::move( task ) ), last_run_time( std::chrono::steady_clock::now( ) ) {}

	void update( ) {
		auto current_time = std::chrono::steady_clock::now( );
		if ( (current_time - last_run_time) >= std::chrono::seconds( wait_time ) ) {
			task( );
			last_run_time = current_time;
		}
	}

private:
	int wait_time;
	std::function<void( )> task;
	std::chrono::steady_clock::time_point last_run_time;
};

inline std::mutex w_data_mutex;
inline std::mutex local_player_mutex;

void a_world::run( )
{
	SetThreadPriority( GetCurrentThread( ), THREAD_PRIORITY_LOWEST );

	timer static_world( 1, [ & ] ( ) {
		std::lock_guard<std::mutex> lock( w_data_mutex );

		world_data->gworld = io->read<uintptr_t>( io->m_image_base + offsets->gworld );
		if ( !world_data->gworld ) return;

		world_data->game_state = io->read<uintptr_t>( world_data->gworld + offsets->gamestate );
		if ( !world_data->game_state ) return;

		world_data->game_instance = io->read<uintptr_t>( world_data->gworld + offsets->gameinstance );
		if ( !world_data->game_instance ) return;

		auto local_players = io->read<uintptr_t>( world_data->game_instance + offsets->localplayers );
		if ( !local_players ) return;

		std::lock_guard<std::mutex> local_lock( local_player_mutex );
		world_data->local_player = io->read<uintptr_t>( local_players );

		} );

	while ( true )
	{
		static_world.update( );

		std::lock_guard<std::mutex> lock( w_data_mutex );

		world_data->player_controller = io->read<uintptr_t>( world_data->local_player + offsets->playercontroller );
		if ( !world_data->player_controller ) continue;

		{
			std::lock_guard<std::mutex> local_lock( local_player_mutex );
			world_data->local_pawn = io->read<uintptr_t>( world_data->player_controller + offsets->acknowledgedpawn );
		}

		world_data->player_array = io->read<uengine::tarray<std::uintptr_t>>( world_data->game_state + offsets->playerarray ).get_all( );

		std::cout << " player array size : " << world_data->player_array.size() << std::endl;

		std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
	}
}