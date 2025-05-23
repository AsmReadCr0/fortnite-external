#pragma once
#include "..\..\includes.hpp"

class c_aimbot
{
public:
	uemath::fvector prediction_calculations( uemath::fvector target, uemath::fvector velocity, uemath::fvector local_velocity, float projectile_speed_scale, float projectile_gravity_scale, float distance );

	float powf_(float _X, float _Y);
	double get_cross_distance(double x1, double y1, double z1);
};
inline const auto aimbot = std::make_unique<c_aimbot>();
