#pragma once

class vars
{
public:
	//void load(const char* config_name);
	//void save(const char* config_name);
public:
	struct
	{
		bool enabled = false;
		bool prediction = false;

		int vkey = 0x02;
		int vkey_0;

		int hitbox = 1;

		float smooth = 2;
		float fov = 150;

		bool hitsound = true;

		bool visible_only = false;
		bool ignore_dbno = true;
		bool ignore_knocked = true;

		struct
		{
			bool fov = true;
			bool line = false;
			bool dot = false;
		} drawing;

		struct
		{
			bool enabled = false;
			bool shotgun = true; // shotgun only
			float delay = 30.0f;
		} triggerbot;

	} combat;


	struct
	{
		bool menu = true;
		bool vsync = true;

		bool enable_visuals = true;

		double render_distance = 300;

		bool box = true;
		bool skeleton = false;
		bool distance = false;
		bool rank = false;
		bool name = false;
		bool team_id = false;
		bool head_circle = false;
		bool indicators = false;
		bool weapon = false;

		struct
		{
			bool enable_loot = false;
			bool chest = false;
			bool ammo_box = false;
			bool dropped = false;

			float render_distnace = 200.0f;
		} loot;

		struct {
			bool box_outline = false;
			bool skeleton_outline = false;
			bool text_outline = true;
			bool cornered_box = false;
			bool filled_box = false;
			bool bounding_box = true;

			float skeleton_thickness = 1.3f;
			float box_thickness = 1.3f;
		} style;

	} visuals;

	struct
	{
		int width = GetSystemMetrics(SM_CXSCREEN);
		int height = GetSystemMetrics(SM_CYSCREEN);
	} monitor;
};

inline std::unique_ptr<vars> g_vars = std::make_unique<vars>();