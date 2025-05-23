#include <core/thread/weapon/a_weapon.hpp>
#include <core/thread/world.hpp>

#include <core/includes.hpp>

#include <core/sdk/utilities/utilities.hpp>

void a_weapon::run()
{
	SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_LOWEST );

	while (true)
	{
		auto current_weapon = io->read<uintptr_t>(world_data->local_pawn + offsets->current_weapon);

		sdk::weapon::speed = io->read<float>(current_weapon + 0x1E40);
		sdk::weapon::gravity = io->read<float>(current_weapon + 0x1E44);

		if ((sdk::weapon::gravity < 0) || (sdk::weapon::speed < 0))
		{
			sdk::weapon::gravity = 0;
			sdk::weapon::speed = 0;
		}

		if ( g_vars->visuals.weapon && current_weapon != 0 )
		{
			sdk::weapon::weapon_name = sdk::weapon::get_weapon_name(current_weapon);
		}

		//if ( world_data->player_controller )
		//{
		//	auto spectator_name = sdk::other::get_spectator_name( );

		//	std::cout << "spectator_name: " << spectator_name << std::endl;
		//}

		if ( g_vars->combat.hitsound && world_data->local_pawn )
		{
			auto has_hit = io->read<bool>(world_data->local_pawn + 0x40b4 ); // AFortPlayerPawn->bDamagedEnemy

			if ( has_hit )
			{
				std::cout << hash_str("enemy damaged by localpawn")  << std::endl;
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}