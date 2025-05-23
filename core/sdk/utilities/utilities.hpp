#pragma once
#include <core/includes.hpp>
#include <core/thread/entity.hpp>

#include <core/sdk/engine/engine.hpp>

#include <dependencies/driver/driver.hpp>
#include <dependencies/imgui/imgui.h>

#include <dependencies/render/drawing/drawing.hpp>

namespace sdk
{
	namespace camera
	{
		void setup( );
		void update( );
		void rotate( uemath::fvector& aim_location, float smooth );
		inline uintptr_t view_state = null;
		inline const float rad_to_deg = 180.0f / m_pi;
	}

	namespace bone
	{
		uemath::fvector get_bone_position(uintptr_t mesh, int index);
		std::unordered_map<int, uemath::fvector> get_skeleton_bones(const std::vector<std::pair<int, int>>& bone_pairs, uintptr_t mesh);
	}

	namespace screen
	{
		uemath::fvector2d world_to_screen( uemath::fvector location);
	}

	namespace weapon
	{
		std::string get_weapon_name( uintptr_t current_weapon );

		inline std::string weapon_name;
		inline float speed;
		inline float gravity;
	}

	namespace other
	{
		bool is_visible( uintptr_t mesh );
		std::string get_rank( int32_t rank_progress );
		ImColor get_rank_color( int32_t rank_progress );
		std::string get_spectator_name( );
		std::string wchar_to_char(const WCHAR* wstr);
	}
}

class a_camera_data
{
public:
	uemath::fvector location;
	uemath::frotator rotation;
	float fov;
};
inline std::unique_ptr<a_camera_data> camera_data = std::make_unique<a_camera_data>( );

inline bool on_screen( uemath::fvector2d pos )
{
	const auto check = 30;

	if ( pos.x > -check && pos.x < uemath::s_width + check && pos.y > -check && pos.y < uemath::s_height + check ) { return true; }
	else { return false; }
}