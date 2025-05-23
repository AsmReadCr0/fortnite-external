#include "drawing.hpp"

void drawing_::outlined_text(ImDrawList* drawlist, int x, int y, ImColor Color, const char* text)
{
    ImVec2 offsets[] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

    for (const auto& offset : offsets)
    {
        drawlist->AddText(ImVec2(x + offset.x, y + offset.y), ImColor(0, 0, 0, 255), text);
    }

    drawlist->AddText(ImVec2(x, y), Color, text);
}

void drawing_::box_2d(ImDrawList* drawlist, float X, float Y, float W, float H, ImColor col, int thickness, bool filled)
{
    if (filled)
    {
        drawlist->AddRectFilled(ImVec2(X, Y), ImVec2(X + W, Y + H), ImColor(255, 255, 255, 85), 0, 0);
    }

    drawlist->AddRect(ImVec2(X, Y), ImVec2(X + W, Y + H), col, 0, 0, thickness);
}

void drawing_::image_box(int X, int Y, int W, int H, const ImU32& color, int thickness, ImTextureID texture)
{
    float lineW = static_cast<float>(W) / 3;
    float lineH = static_cast<float>(H) / 3;

    float uvX = 0.0f;
    float uvY = 0.0f;
    float uvW = 1.0f;
    float uvH = 1.0f;

    ImDrawList* drawlist = ImGui::GetBackgroundDrawList();
    drawlist->AddImage(texture, ImVec2(X, Y), ImVec2(X + lineW, Y + lineH), ImVec2(uvX, uvY), ImVec2(uvX + uvW / 3, uvY + uvH / 3));
    drawlist->AddImage(texture, ImVec2(X + W - lineW, Y), ImVec2(X + W, Y + lineH), ImVec2(uvX + 2 * uvW / 3, uvY), ImVec2(uvX + uvW, uvY + uvH / 3));
    drawlist->AddImage(texture, ImVec2(X, Y + H - lineH), ImVec2(X + lineW, Y + H), ImVec2(uvX, uvY + 2 * uvH / 3), ImVec2(uvX + uvW / 3, uvY + uvH));
    drawlist->AddImage(texture, ImVec2(X + W - lineW, Y + H - lineH), ImVec2(X + W, Y + H), ImVec2(uvX + 2 * uvW / 3, uvY + 2 * uvH / 3), ImVec2(uvX + uvW, uvY + uvH));
    drawlist->AddImage(texture, ImVec2(X, Y + lineH), ImVec2(X + lineW, Y + H - lineH), ImVec2(uvX, uvY + uvH / 3), ImVec2(uvX + uvW / 3, uvY + 2 * uvH / 3));
    drawlist->AddImage(texture, ImVec2(X + W - lineW, Y + lineH), ImVec2(X + W, Y + H - lineH), ImVec2(uvX + 2 * uvW / 3, uvY + uvH / 3), ImVec2(uvX + uvW, uvY + 2 * uvH / 3));
    drawlist->AddImage(texture, ImVec2(X + lineW, Y), ImVec2(X + W - lineW, Y + lineH), ImVec2(uvX + uvW / 3, uvY), ImVec2(uvX + 2 * uvW / 3, uvY + uvH / 3));
    drawlist->AddImage(texture, ImVec2(X + lineW, Y + H - lineH), ImVec2(X + W - lineW, Y + H), ImVec2(uvX + uvW / 3, uvY + 2 * uvH / 3), ImVec2(uvX + 2 * uvW / 3, uvY + uvH));
    drawlist->AddImage(texture, ImVec2(X + lineW, Y + lineH), ImVec2(X + W - lineW, Y + H - lineH), ImVec2(uvX + uvW / 3, uvY + uvH / 3), ImVec2(uvX + 2 * uvW / 3, uvY + 2 * uvH / 3));
}

void drawing_::text(
    const char* input, float x, float y, ImColor color) {

    auto text_size = ImGui::CalcTextSize(input).x;
    auto text_center = x - text_size * 0.5f;

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx != 0 || dy != 0) {
                ImGui::GetBackgroundDrawList()->AddText(
                    ImVec2(text_center + dx, y + dy),
                    ImColor(0, 0, 0, 255),
                    input
                );
            }
        }
    }

    ImGui::GetBackgroundDrawList()->AddText(
        ImVec2(text_center, y),
        color,
        input
    );

}

void drawing_::line(float x1, float y1, float x2, float y2, ImColor color, float stroke) {

    ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), color, stroke);
}

void drawing_::circle(float x, float y, float radius, ImColor color, float stroke, bool filled) {
    if (filled)
        ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(x, y), radius, color, 100);
    else
        ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(x, y), radius, color, 100, stroke);
}

void drawing_::triangle(float x1, float y1, float x2, float y2, float x3, float y3, ImColor color, float stroke) {
    ImGui::GetBackgroundDrawList()->AddTriangleFilled(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), color);
}

void drawing_::bounding_corner(const uemath::fvector2d& position, const uemath::fvector2d& size, ImColor color) {
    auto line_width = (size.x / 5);
    auto line_height = (size.y / 4);

    if (g_vars->visuals.style.box_outline)
    {
        ImGui::GetBackgroundDrawList()->AddLine(ImVec2(position.x, position.y), ImVec2(position.x, position.y + line_height), black, 3.f);
        ImGui::GetBackgroundDrawList()->AddLine(ImVec2(position.x, position.y), ImVec2(position.x + line_width, position.y), black, 3.f);
        ImGui::GetBackgroundDrawList()->AddLine(ImVec2(position.x + size.x - line_width, position.y), ImVec2(position.x + size.x, position.y), black, 3.f);
        ImGui::GetBackgroundDrawList()->AddLine(ImVec2(position.x + size.x, position.y), ImVec2(position.x + size.x, position.y + line_height), black, 3.f);
        ImGui::GetBackgroundDrawList()->AddLine(ImVec2(position.x, position.y + size.y - line_height), ImVec2(position.x, position.y + size.y), black, 3.f);
        ImGui::GetBackgroundDrawList()->AddLine(ImVec2(position.x, position.y + size.y), ImVec2(position.x + line_width, position.y + size.y), black, 3.f);
        ImGui::GetBackgroundDrawList()->AddLine(ImVec2(position.x + size.x - line_width, position.y + size.y), ImVec2(position.x + size.x, position.y + size.y), black, 3.f);
        ImGui::GetBackgroundDrawList()->AddLine(ImVec2(position.x + size.x, position.y + size.y - line_height), ImVec2(position.x + size.x, position.y + size.y), black, 3.f);
    }

    ImGui::GetBackgroundDrawList()->AddLine(ImVec2(position.x, position.y), ImVec2(position.x, position.y + line_height), color, 2.f);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2(position.x, position.y), ImVec2(position.x + line_width, position.y), color, 2.f);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2(position.x + size.x - line_width, position.y), ImVec2(position.x + size.x, position.y), color, 2.f);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2(position.x + size.x, position.y), ImVec2(position.x + size.x, position.y + line_height), color, 2.f);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2(position.x, position.y + size.y - line_height), ImVec2(position.x, position.y + size.y), color, 2.f);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2(position.x, position.y + size.y), ImVec2(position.x + line_width, position.y + size.y), color, 2.f);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2(position.x + size.x - line_width, position.y + size.y), ImVec2(position.x + size.x, position.y + size.y), color, 2.f);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2(position.x + size.x, position.y + size.y - line_height), ImVec2(position.x + size.x, position.y + size.y), color, 2.f);
}

void drawing_::bounding_box(const uemath::fvector2d& position, const uemath::fvector2d& size, ImColor color, float thickness) {
    if (g_vars->visuals.style.box_outline)
    {
        ImGui::GetBackgroundDrawList()->AddRect(ImVec2(position.x, position.y), ImVec2(size.x + position.x, size.y + position.y), color, 0.0f, 0, thickness + 1);
    }   
    ImGui::GetBackgroundDrawList()->AddRect(ImVec2(position.x, position.y), ImVec2(size.x + position.x, size.y + position.y), color, 0.0f, 0, thickness);
}

void drawing_::bounding_filled(const uemath::fvector2d& position, const uemath::fvector2d& size, ImColor color) {
    ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(position.x, position.y), ImVec2(size.x + position.x, size.y + position.y), color);
}

void drawing_::rect(
    float x,
    float y,
    float width,
    float height,
    ImColor color) {
    float lineW = (width);
    float lineH = (height);

    ImGui::GetBackgroundDrawList()->AddRect(ImVec2(x, y), ImVec2(width, height), color);
}

void drawing_::static_box(
    float x,
    float y,
    float width,
    float height,
    ImColor color,
    float rounded,
    float thickness) {

    float lineW = (width);
    float lineH = (height);

    ImVec2 top_left(x, y);
    ImVec2 bottom_right(x + width, y + height);

    if (g_vars->visuals.style.box_outline)
    {
        ImGui::GetBackgroundDrawList()->AddRect(top_left, bottom_right, ImColor(0, 0, 0, 255), 0, 0, thickness + 1);
    }
    ImGui::GetBackgroundDrawList()->AddRect(top_left, bottom_right, color, 0, 0, thickness);

}