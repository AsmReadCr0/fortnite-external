#pragma once
#include "..\..\..\core\includes.hpp"
#include <dependencies/imgui/imgui.h>
#include <dependencies/imgui/imgui_internal.h>
#include <dependencies/imgui/imgui_impl_win32.h>
#include <dependencies/imgui/imgui_impl_dx11.h>

class drawing_
{
public:
    void text(const char* input, float x, float y, ImColor color);
    void line(float x1, float y1, float x2, float y2, ImColor color, float thickness);
    void circle(float x, float y, float radius, ImColor color, float stroke, bool filled);
    void outlined_text(ImDrawList* drawlist, int x, int y, ImColor Color, const char* text);
    void box_2d(ImDrawList* drawlist, float X, float Y, float W, float H, ImColor col, int thickness, bool filled);
    void image_box(int X, int Y, int W, int H, const ImU32& color, int thickness, ImTextureID texture);
    void triangle(float x1, float y1, float x2, float y2, float x3, float y3, ImColor color, float stroke);
    void bounding_corner(const uemath::fvector2d& position, const uemath::fvector2d& size, ImColor color);
    void bounding_box(const uemath::fvector2d& position, const uemath::fvector2d& size, ImColor color, float thickness);
    void bounding_filled(const uemath::fvector2d& position, const uemath::fvector2d& size, ImColor color);
    void rect(float x, float y, float width, float height, ImColor color);
    void static_box(float x, float y, float width, float height, ImColor color, float rounded, float thickness);

    ImFont* font;

} inline drawing;

inline ImColor blue(0, 90, 255, 255);
inline ImColor red(255, 0, 0, 255);
inline ImColor white(255, 255, 255, 255);

inline ImColor pink(1.0f, 0.6f, 0.8f);

inline ImColor gray(245, 245, 245, 255);

inline ImColor blue_0(0, 150, 255, 255);
inline ImColor red_0(255, 70, 0, 255);

inline ImColor black(0, 0, 0, 255);

inline ImColor blue_low(0, 90, 255, 100);
inline ImColor red_low(255, 0, 0, 100);
inline ImColor white_low(255, 255, 255, 100);
inline ImColor black_low(0, 0, 0, 50);

inline ImColor main_color_box(255, 255, 255, 255);
inline ImColor main_color_skeleton(255, 255, 255, 255);
inline ImColor main_color_targeted(255, 255, 255, 255);