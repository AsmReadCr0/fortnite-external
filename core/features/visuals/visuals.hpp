#pragma once
#include "..\..\includes.hpp"

class c_visual
{
public:
	void render();
	void add_indicator(uemath::fvector world_location, float distance, bool visible);
};
inline const auto visual = std::make_unique<c_visual>();

