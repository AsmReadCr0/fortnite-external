#include "visuals.hpp"

#include <core/thread/entity.hpp>

#include <core/sdk/utilities/utilities.hpp>

#include <dependencies/cipher/function/function_imports.hpp>

#include <dependencies/driver/driver.hpp>

#include <core/overlay/overlay.hpp>

#include <core/features/features.hpp>

#include <core/thread/world.hpp>

auto id = 0;

void c_visual::render()
{
	auto closest_distance = DBL_MAX;
	auto closest_entity = a_entity_data(null);

	if (g_vars->combat.drawing.fov)
	{
		drawing.circle(uemath::s_width_center, uemath::s_height_center, g_vars->combat.fov, black, 3, false);
		drawing.circle(uemath::s_width_center, uemath::s_height_center, g_vars->combat.fov, white, 2, false);
	}

	auto draw_list = ImGui::GetBackgroundDrawList();
	auto offset = 15;

	sdk::camera::update();

	char text[128];
	sprintf_s(text, hash_str("FPS: %d"), static_cast<int>(ImGui::GetIO().Framerate));

	drawing.outlined_text(draw_list, 15, 15, ImColor(255, 255, 255, 255), hash_str("soller"));
	drawing.outlined_text(draw_list, 15 + ImGui::CalcTextSize(hash_str("soller")).x, 15, blue, ".run");
	drawing.outlined_text(draw_list, 15, 30, white, text);

	for (auto& entity : entity_data) 
	{
		entity.dying = (io->read<char>(entity.pawn_private + offsets->b_is_dying) >> 4) & 1;

		if (entity.dying) continue;

		entity.downed = (io->read<char>(entity.pawn_private + offsets->b_is_dbno) >> 4) & 1;
		entity.visible = sdk::other::is_visible(entity.mesh);

		auto head_location = sdk::bone::get_bone_position(entity.mesh, 67);
		auto root_location = sdk::bone::get_bone_position(entity.mesh, 0);

		auto distance = camera_data->location.distance(head_location) / 100;

		auto head_w2s = sdk::screen::world_to_screen(head_location);
		auto head_box_w2s = sdk::screen::world_to_screen(uemath::fvector(head_location.x, head_location.y, head_location.z + 15));
		auto root_w2s = sdk::screen::world_to_screen(root_location);

		if ( g_vars->visuals.box && g_vars->visuals.enable_visuals)
		{
			float height = abs(head_box_w2s.y - root_w2s.y);
			float width = height / 2;

			uemath::fvector2d top_left(head_w2s.x - width / 2, head_box_w2s.y);
			uemath::fvector2d box_size(width, height);

			if ( g_vars->visuals.style.bounding_box ) {
				drawing.bounding_box(top_left, box_size, black, g_vars->visuals.style.box_thickness + 0.5f);
				drawing.bounding_box(top_left, box_size, main_color_box, g_vars->visuals.style.box_thickness);
			}
			if ( g_vars->visuals.style.filled_box ) {
				drawing.bounding_filled(top_left, box_size, black_low);
			}
			if ( g_vars->visuals.style.cornered_box )
			{
				drawing.bounding_corner(top_left, box_size, white);
			}
		}

		if ( g_vars->visuals.skeleton && g_vars->visuals.enable_visuals )
		{
			auto bone_id_to_position = sdk::bone::get_skeleton_bones(bone_pairs, entity.mesh); 

			for (const auto& pair : bone_pairs)
			{
				if (bone_id_to_position.find(pair.first) != bone_id_to_position.end() && bone_id_to_position.find(pair.second) != bone_id_to_position.end())
				{
					auto start_3d = bone_id_to_position[pair.first];
					auto end_3d = bone_id_to_position[pair.second];

					auto start_screen = sdk::screen::world_to_screen(start_3d);
					auto end_screen = sdk::screen::world_to_screen(end_3d);

					if (g_vars->visuals.style.skeleton_outline) {
						drawing.line(start_screen.x, start_screen.y, end_screen.x, end_screen.y, 
							main_color_skeleton, g_vars->visuals.style.skeleton_thickness + 1.0f); }

					drawing.line(start_screen.x, start_screen.y, end_screen.x, end_screen.y, main_color_skeleton, g_vars->visuals.style.skeleton_thickness);
				}
			}
		}

		if ( g_vars->visuals.distance && g_vars->visuals.enable_visuals )
		{
			auto text = hash_str("[") + (std::to_string((int)distance) + hash_str("m")) + hash_str("]");

			drawing.text(text.c_str(), root_w2s.x, root_w2s.y, white);
		}

		if ( g_vars->visuals.name && g_vars->visuals.enable_visuals )
		{
			auto text = (entity.username.empty() ? hash_str("null") : entity.username);

			drawing.text(text.c_str(), head_box_w2s.x, head_box_w2s.y - offset, white);

			offset += 15;
		}

		if ( g_vars->visuals.rank && g_vars->visuals.enable_visuals )
		{
			auto text = sdk::other::get_rank(entity.rank_progress);

			auto color = sdk::other::get_rank_color(entity.rank_progress);

			drawing.text(text.c_str(), head_box_w2s.x, head_box_w2s.y - offset, color);

			offset += 15;
		}

		if ( g_vars->visuals.team_id && g_vars->visuals.enable_visuals )
		{
			auto text = std::to_string(entity.team_id);

			drawing.text(text.c_str(), head_box_w2s.x, head_box_w2s.y - offset, white);

			offset += 15;
		}

		if ( g_vars->visuals.head_circle && g_vars->visuals.enable_visuals )
		{
			auto head_location = sdk::bone::get_bone_position(entity.mesh, 110);
			auto head_w2s = sdk::screen::world_to_screen(head_location);

			float current_player_location = (head_location.distance(camera_data->location) / 100);

			float circle_radius = 7.5f;
			float radius = circle_radius / current_player_location;

			drawing.circle(head_w2s.x, head_w2s.y, radius, main_color_skeleton, 1.0f, false);
		}

		if ( g_vars->visuals.indicators && g_vars->visuals.enable_visuals )
		{
			add_indicator(head_location, distance, entity.visible);
		}

		if (g_vars->visuals.weapon && g_vars->visuals.enable_visuals)
		{
			auto current_weapon = io->read<uintptr_t>(entity.pawn_private + offsets->current_weapon);
			auto text = sdk::weapon::get_weapon_name(current_weapon);

			drawing.text(text.c_str(), root_w2s.x + 25, root_w2s.y - offset, white);
		}
	}
}

void c_visual::add_indicator(uemath::fvector world_location, float distance, bool visible)
{   
	auto camera_angle = camera_data->rotation;
	float yaw = static_cast<float>(camera_angle.yaw) * m_pi / 180.0f;
	float dx = world_location.x - camera_data->location.x;
	float dy = world_location.y - camera_data->location.y;
	float fsin_yaw = sinf(yaw);
	float fminus_cos_yaw = -cosf(yaw);

	auto center = ImVec2(uemath::s_width / 2, uemath::s_height / 2);

	float x = -(dy * fminus_cos_yaw + dx * fsin_yaw);
	float y = dx * fminus_cos_yaw - dy * fsin_yaw;

	float angle = atan2f(y, x);
	float indicator_size = 12.0f;
	float width_factor = 8.0f;

	ImVec2 indicator_center = ImVec2(center.x + cosf(angle) * (g_vars->combat.fov + indicator_size),
		center.y + sinf(angle) * (g_vars->combat.fov + indicator_size));

	ImVec2 point1 = ImVec2(indicator_center.x + cosf(angle) * indicator_size,
		indicator_center.y + sinf(angle) * indicator_size);

	ImVec2 point2 = ImVec2(indicator_center.x + cosf(angle + width_factor) * indicator_size,
		indicator_center.y + sinf(angle + width_factor) * indicator_size);

	ImVec2 point3 = ImVec2(indicator_center.x + cosf(angle - width_factor) * indicator_size,
		indicator_center.y + sinf(angle - width_factor) * indicator_size);

	drawing.triangle(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y, pink, 1.0f);
}