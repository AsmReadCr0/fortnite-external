#include <dependencies/cipher/function/function_imports.hpp>
#include <dependencies/driver/includes.hpp>

#include <core/thread/world/a_world.hpp>
#include <core/thread/entity/a_entity.hpp>
#include <core/thread/weapon/a_weapon.hpp>

#include <core/overlay/overlay.hpp>
#include <core/sdk/utilities/utilities.hpp>

//------------------------------------------------------------------//

bool enable_virtual_terminal_processing( )
{
	HANDLE h_out = GetStdHandle( STD_OUTPUT_HANDLE );
	if ( h_out == INVALID_HANDLE_VALUE ) {
		return false;
	}

	DWORD dw_mode = 0;
	if ( !GetConsoleMode( h_out, &dw_mode ) ) {
		return false;
	}

	dw_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if ( !SetConsoleMode( h_out, dw_mode ) ) {
		return false;
	}

	return true;
}

auto main( ) -> std::int32_t
{
	enable_virtual_terminal_processing();

	std::printf(hash_str("\033[97m[\033[38;5;205m+\033[97m] attaching to fortniteclient-win64-shipping.exe\n\033[0m"));

	bool status = io->attach_vm(hash_str(L"FortniteClient-Win64-Shipping.exe")); // FortniteClient-Win64-Shipping.exe
	if (!status) { return 0; }

	status = g_render->setup_canvas();
	if (!status) { return 0; }

	key_handler->load( );

	sdk::camera::setup( );

	std::thread( a_world::run ).detach( );

	std::thread( a_entity::run ).detach( );

	std::thread( a_weapon::run ).detach( );

	std::jthread( [ & ]( ) -> void { g_render->tick( ); } ).detach( );

	return std::cin.get( ) != EOF;
}