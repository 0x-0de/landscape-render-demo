#include "ui.h"

UIQuad::UIQuad(Vec2 pos, Vec2 scl, bool use_relative_coords, Vec4 color) : pos(pos), scl(scl), use_relative_coords(use_relative_coords), color(color)
{}

void UIQuad::draw(Renderer& renderer, ShaderProgram& shader)
{
    float px = pos.x * (float) std::max((engine_width / 2) * use_relative_coords, 1);
    float py = pos.y * (float) std::max((engine_height / 2) * use_relative_coords, 1);

    float sx = scl.x * (float) std::max((engine_width / 2) * use_relative_coords, 1);
    float sy = scl.y * (float) std::max((engine_height / 2) * use_relative_coords, 1);

    shader.transfer_mat4("transform", mat_transform(Vec3(px, py, 0), Vec3(0, 0, 0), Vec3(sx, sy, 1)));
    shader.transfer_vec4("color_in", color);
    shader.transfer_int("draw_mode", 0);
    renderer.draw_mesh(ref_zquad());
}

void UIQuad::set_pos(Vec2 val)
{
    pos = val;
}

void UIQuad::set_scl(Vec2 val)
{
    scl = val;
}

UIText::UIText(Vec2 pos, float scale, bool use_relative_coords, std::string message, Font* font) : pos(pos), scl(Vec2(scale, scale)), use_relative_coords(use_relative_coords), message(message), font(font)
{
    mesh = new Mesh(2);
    build(message);
}

void UIText::build(std::string new_message)
{
    font->get_message_mesh(mesh, new_message);
    message = new_message;
}

void UIText::draw(Renderer& renderer, ShaderProgram& shader)
{
    float px = pos.x * (float) std::max((engine_width / 2) * use_relative_coords, 1);
    float py = pos.y * (float) std::max((engine_height / 2) * use_relative_coords, 1);

    float sx = scl.x * (float) std::max((engine_width / 2) * use_relative_coords, 1);
    float sy = scl.y * (float) std::max((engine_height / 2) * use_relative_coords, 1);

    shader.transfer_mat4("transform_static", mat_transform(Vec3(px, py, 0), Vec3(0, 0, 0), Vec3(sx, sy, 1)));
    shader.transfer_vec4("color_in", Vec4(1, 1, 1, 1));
    shader.transfer_int("draw_mode", 1);
    renderer.draw_mesh(mesh);
}

void UIText::set_pos(Vec2 val)
{
    pos = val;
}

void UIText::set_scl(Vec2 val)
{
    scl = val;
}