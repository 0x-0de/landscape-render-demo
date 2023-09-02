#ifndef _UI_H_
#define _UI_H_

#include <string>

#include "../engine/mesh.h"
#include "../engine/renderer.h"
#include "../engine/shader.h"

#include "font.h"

class UIQuad
{
public:
    UIQuad(Vec2 pos, Vec2 scl, bool use_relative_coords, Vec4 color);

    void draw(Renderer& renderer, ShaderProgram& shader);

    Vec2 get_pos() const { return pos; }
    Vec2 get_scl() const { return scl; }

    void set_pos(Vec2 val);
    void set_scl(Vec2 val);
private:
    Vec2 pos, scl;
    bool use_relative_coords;

    Vec4 color;
};

class UIText
{
public:
    UIText(Vec2 pos, float scale, bool use_relative_coords, std::string message, Font* font);

    void build(std::string new_message);
    void draw(Renderer& renderer, ShaderProgram& shader);

    Vec2 get_pos() const { return pos; }
    Vec2 get_scl() const { return scl; }

    std::string get_message() const { return message; }

    Mesh* get_mesh() const { return mesh; }

    void set_pos(Vec2 val);
    void set_scl(Vec2 val);
private:
    Vec2 pos, scl;
    bool use_relative_coords;

    std::string message;
    Font* font;

    Mesh* mesh;
};

#endif