#include "aimbot.hpp"
#include <core/thread/entity.hpp>
#include <core/sdk/utilities/utilities.hpp>
#include <dependencies/driver/driver.hpp>
#include <dependencies/cipher/function/function_imports.hpp>

uemath::fvector c_aimbot::prediction_calculations( uemath::fvector target, uemath::fvector velocity, uemath::fvector local_velocity, float projectile_speed_scale, float projectile_gravity_scale, float distance )
{
	float vertical_distance = target.z - local_velocity.z;

	if (vertical_distance > 0)
	{
		int exceed_count_70 = static_cast<int>((distance - 70) / 70);
		int exceed_count_125 = static_cast<int>((distance - 125) / 125);
		int exceed_count_150 = static_cast<int>((distance - 150) / 150);

		if (exceed_count_150 > 0)
		{
			projectile_gravity_scale -= 0.3f;
		}
		else if (exceed_count_125 > 0)
		{
			projectile_gravity_scale -= 0.4f;
		}
		else if (exceed_count_70 > 0)
		{
			projectile_gravity_scale -= 0.5f;
		}
	}

	float horizontal_time = distance / projectile_speed_scale;
	float vertical_time = distance / projectile_speed_scale;

	target.x += velocity.x * horizontal_time;
	target.y += velocity.y * horizontal_time;
	target.z += velocity.z * vertical_time + fabs(-980 * projectile_gravity_scale) * 0.5f * (vertical_time * vertical_time);

	return target;
}

float c_aimbot::powf_(float _X, float _Y)
{
	return (_mm_cvtss_f32(_mm_pow_ps(_mm_set_ss(_X), _mm_set_ss(_Y))));
}

double c_aimbot::get_cross_distance(double x1, double y1, double z1)
{
	return sqrtf(powf((g_vars->monitor.width / 2 - x1), 2) + powf_((g_vars->monitor.width / 2 - y1), 2));
}