#include "utilities.hpp"
#include <core/sdk/engine/engine.hpp>
#include <core/thread/world.hpp>
#include <core/sdk/math/primitives.hpp>
#include <dependencies/driver/driver.hpp>

uemath::fvector sdk::bone::get_bone_position(uintptr_t mesh, int index) {
	int32_t active_transforms = io->read<int32_t>(mesh + offsets->bone_array + 0x48) * 0x10;
	uintptr_t bone_array_ptr = mesh + offsets->bone_array + active_transforms;
	auto bone_array = io->read<uengine::tarray<uengine::ftransform>>(bone_array_ptr);
	auto bone = bone_array.get(index);
	auto component_to_world = io->read<uengine::ftransform>(mesh + offsets->c2w);

	auto bone_matrix = uengine::matrix_multiplication(bone.to_matrix_with_scale(), component_to_world.to_matrix_with_scale());
	return { bone_matrix._41, bone_matrix._42, bone_matrix._43 };
}

std::unordered_map<int, uemath::fvector> sdk::bone::get_skeleton_bones(const std::vector<std::pair<int, int>>& bone_pairs, uintptr_t mesh)
{
	std::unordered_map<int, uemath::fvector> bone_positions;
	bone_positions.reserve(bone_pairs.size() * 2);

	for (const auto& pair : bone_pairs)
	{
		for (int id : {pair.first, pair.second})
		{
			if (bone_positions.find(id) == bone_positions.end())
			{
				int32_t active_transforms = io->read<int32_t>(mesh + offsets->bone_array + 0x48) * 0x10;
				uintptr_t bone_array_ptr = mesh + offsets->bone_array + active_transforms;
				auto bone_array = io->read<uengine::tarray<uengine::ftransform>>(bone_array_ptr);
				auto bone = bone_array.get(id);
				auto component_to_world = io->read<uengine::ftransform>(mesh + offsets->c2w);

				auto bone_matrix = uengine::matrix_multiplication(bone.to_matrix_with_scale(), component_to_world.to_matrix_with_scale());
				bone_positions[id] = { bone_matrix._41, bone_matrix._42, bone_matrix._43 };
			}
		}
	}

	return bone_positions;
}


void sdk::camera::setup( )
{
	auto uworld = io->read<uintptr_t>(io->m_image_base + offsets->gworld);
	auto owning_game_instance = io->read<uintptr_t>(uworld + offsets->gameinstance);
	auto local_player = io->read<uengine::tarray<std::uintptr_t>>(owning_game_instance + offsets->localplayers).get(0);
	auto view_matrix = io->read<uintptr_t>(local_player + 0xd0);

	sdk::camera::view_state = io->read<uintptr_t>(view_matrix + 0x8);
}

void sdk::camera::update( )
{
	auto projection = io->read<uengine::fmatrix>(sdk::camera::view_state + 0x940);

	camera_data->rotation.pitch = asin(projection.z_plane.w) * 180.0f / std::numbers::pi;
	camera_data->rotation.yaw = atan2(projection.y_plane.w, projection.x_plane.w) * 180.0f / std::numbers::pi;
	camera_data->rotation.roll = 0.0f;

	camera_data->location.x = projection.m[3][0];
	camera_data->location.y = projection.m[3][1];
	camera_data->location.z = projection.m[3][2];

	auto fov_radians = 2.0f * atanf(1.0f / static_cast<float>(io->read<double>(sdk::camera::view_state + 0x740)));
	camera_data->fov = fov_radians * 180.0f / std::numbers::pi;
}

uemath::fvector2d sdk::screen::world_to_screen( uemath::fvector location)
{
	auto rotation_matrix = uengine::create_rotation_matrix(camera_data->rotation);

	auto& axis_x = rotation_matrix.m[0];
	auto& axis_y = rotation_matrix.m[1];
	auto& axis_z = rotation_matrix.m[2];

	auto delta = location - camera_data->location;

	auto transformed_x = delta.dot(uemath::fvector(axis_y[0], axis_y[1], axis_y[2]));
	auto transformed_y = delta.dot(uemath::fvector(axis_z[0], axis_z[1], axis_z[2]));
	auto transformed_z = max(delta.dot(uemath::fvector(axis_x[0], axis_x[1], axis_x[2])), 1.0f);

	auto aspect_ratio = static_cast<float>(uemath::s_width) / uemath::s_height;
	auto inv_fov = tanf(camera_data->fov * static_cast<float>(std::numbers::pi) / 360.0f) / aspect_ratio;

	auto screen_x = uemath::s_width_center + transformed_x * (uemath::s_height_center / inv_fov) / transformed_z;
	auto screen_y = uemath::s_height_center - transformed_y * (uemath::s_height_center / inv_fov) / transformed_z;

	return uemath::fvector2d(screen_x, screen_y);
}

uemath::frotator find_look_at_rotation(uemath::fvector& start, uemath::fvector& target) {

	uemath::fvector direction = target - start;
	direction = direction / sqrt(direction.length());

	double yaw = atan2(direction.y, direction.x) * (180.0 / m_pi);
	double pitch = atan2(direction.z, sqrt(direction.x * direction.x + direction.y * direction.y)) * (180.0 / m_pi);

	return uemath::frotator(pitch, yaw, 0.0);
}

uemath::frotator clamp_angle(uemath::frotator start_rotation, uemath::frotator end_rotation, float smooth_scale)
{
	uemath::frotator ret = end_rotation;

	ret.pitch -= start_rotation.pitch;
	ret.yaw -= start_rotation.yaw;

	//normalize yaw
	if (ret.yaw < -180.0f)
		ret.yaw += 360.0f;
	if (ret.yaw > 180.0f)
		ret.yaw -= 360.0f;

	//clamp pitch
	if (ret.pitch < -74.0f)
		ret.pitch = -74.0f;
	if (ret.pitch > 74.0f)
		ret.pitch = 74.0f;

	//smooth rotation
	ret.pitch = (ret.pitch / smooth_scale) + start_rotation.pitch;
	ret.yaw = (ret.yaw / smooth_scale) + start_rotation.yaw;

	//normalize yaw again
	if (ret.yaw < -180.0f)
		ret.yaw += 360.0f;
	if (ret.yaw > 180.0f)
		ret.yaw -= 360.0f;

	//clamp pitch again
	if (ret.pitch < -74.0f)
		ret.pitch = -74.0f;
	if (ret.pitch > 74.0f)
		ret.pitch = 74.0f;

	return ret;
}


void sdk::camera::rotate( uemath::fvector& aim_location, float smooth )
{
	auto aim_rotation = find_look_at_rotation(camera_data->location, aim_location);

	if (smooth > 1)
	{
		aim_rotation = clamp_angle(camera_data->rotation, aim_rotation, smooth);
	}

	aim_rotation = aim_rotation - camera_data->rotation;
	aim_rotation.normalize();

	if (abs(aim_rotation.pitch) > 0.68f)
	{
		auto pitch_scale = 0.68f / abs(aim_rotation.pitch);
		aim_rotation *= pitch_scale;
	}

	if (abs(aim_rotation.yaw) > 0.68f)
	{
		auto yaw_scale = 0.68f / abs(aim_rotation.yaw);
		aim_rotation *= yaw_scale;
	}

	//if (!io->write<uemath::frotator>(world_data->player_controller + 0x520, aim_rotation)) { log_error(); }
}

std::string sdk::weapon::get_weapon_name( uintptr_t current_weapon )
{
	auto weapon_data = io->read<uint64_t>( current_weapon + offsets->weapon_data );
	if ( !weapon_data ) return hash_str( "null" );

	auto fname_text = io->read<uint64_t>( weapon_data + offsets->item_name );
	if ( !fname_text ) return hash_str( "null" );

	auto name_length = io->read<uint32_t>( fname_text + 0x30 );
	if ( !name_length ) return hash_str( "null" );

	wchar_t* name = new wchar_t[ uint64_t( name_length ) + 1 ];

	if ( !io->read_array( (uintptr_t)io->read<PVOID>( fname_text + 0x28 ), (name), name_length * sizeof( wchar_t ) ) ) {  }

	std::wstring wname( name );
	return std::string( wname.begin( ), wname.end( ) );
}

bool sdk::other::is_visible( uintptr_t mesh )
{
	auto seconds = io->read<double>(world_data->gworld + 0x158);
	auto last_render_time = io->read<float>(mesh + 0x32C);
	return seconds - last_render_time <= 0.06f;
}

std::string sdk::other::get_rank( int32_t rank_progress )
{
	if ( rank_progress == 0 )
		return std::string( hash_str( "bronze 1" ) );
	else if ( rank_progress == 1 )
		return std::string( hash_str( "bronze 2" ) );
	else if ( rank_progress == 2 )
		return std::string( hash_str( "bronze 3" ) );
	else if ( rank_progress == 3 )
		return std::string( hash_str( "silver 1" ) );
	else if ( rank_progress == 4 )
		return std::string( hash_str( "silver 2" ) );
	else if ( rank_progress == 5 )
		return std::string( hash_str( "silver 3" ) );
	else if ( rank_progress == 6 )
		return std::string( hash_str( "gold 1" ) );
	else if ( rank_progress == 7 )
		return std::string( hash_str( "gold 2" ) );
	else if ( rank_progress == 8 )
		return std::string( hash_str( "gold 3" ) );
	else if ( rank_progress == 9 )
		return std::string( hash_str( "platinum 1" ) );
	else if ( rank_progress == 10 )
		return std::string( hash_str( "platinum 2" ) );
	else if ( rank_progress == 11 )
		return std::string( hash_str( "platinum 3" ) );
	else if ( rank_progress == 12 )
		return std::string( hash_str( "diamond 1" ) );
	else if ( rank_progress == 13 )
		return std::string( hash_str( "diamond 2" ) );
	else if ( rank_progress == 14 )
		return std::string( hash_str( "diamond 3" ) );
	else if ( rank_progress == 15 )
		return std::string( hash_str( "elite" ) );
	else if ( rank_progress == 16 )
		return std::string( hash_str( "champion" ) );
	else if ( rank_progress == 17 )
		return std::string( hash_str( "unreal" ) );
	else
		return std::string( hash_str( "null" ) );
}

ImColor sdk::other::get_rank_color( int32_t rank_progress )
{
	if ( rank_progress == 0 || rank_progress == 1 || rank_progress == 2 )
		return ImColor( 255, 128, 0, 255 );
	else if ( rank_progress == 3 || rank_progress == 4 || rank_progress == 5 )
		return ImColor( 138, 138, 138, 255 );
	else if ( rank_progress == 6 || rank_progress == 7 || rank_progress == 8 )
		return ImColor( 255, 166, 71, 255 );
	else if ( rank_progress == 9 || rank_progress == 10 || rank_progress == 11 )
		return ImColor( 96, 188, 209, 255 );
	else if ( rank_progress == 12 || rank_progress == 13 || rank_progress == 14 )
		return ImColor( 0, 128, 255, 255 );
	else if ( rank_progress == 15 )
		return ImColor( 122, 155, 163, 255 );
	else if ( rank_progress == 16 )
		return ImColor( 255, 42, 0, 255 );
	else if ( rank_progress == 17 )
		return ImColor( 102, 0, 255, 255 );
	else
		return ImColor( 255, 255, 255, 255 );
}

std::string sdk::other::get_spectator_name( )
{
	auto fname_text = io->read<uint64_t>( world_data->player_controller + 0x6b0 );
	if ( !fname_text ) return hash_str( "null" );

	auto name_length = io->read<uint32_t>( fname_text + 0x30 );
	if ( !name_length ) return hash_str( "null" );

	wchar_t* name = new wchar_t[ uint64_t( name_length ) + 1 ];

	if ( !io->read_array( (uintptr_t)io->read<PVOID>( fname_text + 0x28 ), (name), name_length * sizeof( wchar_t ) ) ) {  }

	std::wstring wname( name );
	return std::string( wname.begin( ), wname.end( ) );
}

std::string sdk::other::wchar_to_char(const WCHAR* wstr) {
	if (!wstr) return hash_str("");

	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
	if (size_needed <= 0) return hash_str("");

	std::string result(size_needed - 1, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &result[0], size_needed, nullptr, nullptr);

	return result;
}