#include <cmath>
#include <thread>

#include <iostream>

#include "../lib/GL/glew.h"
#include "../lib/GLFW/glfw3.h"

#include "game/player.h"
#include "game/sector.h"
#include "game/world.h"

#include "rendering/mesh.h"
#include "rendering/renderer.h"
#include "rendering/shader.h"

int engine_width, engine_height;
double engine_delta, engine_time;
bool is_running;

static World* world;

static const char* vs_main = "\
#version 330 core\n\
layout(location = 0) in vec3 pos;\n\
layout(location = 1) in vec3 col;\n\
\n\
out mat4 modelview;\n\
out vec3 o_pos, o_col;\n\
\n\
uniform mat4 transform, projection, viewport;\n\
\n\
void main()\n\
{\n\
    o_pos = pos;\n\
    o_col = col;\n\
    modelview = transform;\n\
    gl_Position = projection * viewport * transform * vec4(pos, 1);\n\
}\n\0";

static const char* fs_main = "\
#version 330 core\n\
out vec4 color;\n\
\n\
in mat4 modelview;\n\
in vec3 o_pos, o_col;\n\
\n\
uniform vec2 draw_distance;\n\
uniform vec3 player_pos, sky_col, sky_mod;\n\
uniform float draw_attenuation = -1;\n\
uniform int draw_mode = 0;\n\
\n\
uniform float time;\n\
\n\
float get_alpha();\n\
vec3 add_light(vec3 pos, vec3 cur, vec3 color, float dist, float attenuation, float speed, float pause);\n\
\n\
void main()\n\
{\n\
    switch(draw_mode)\n\
    {\n\
        case 0:\n\
            float distance = sqrt(pow(o_pos.x, 2) + pow(o_pos.y, 2) + pow(o_pos.z, 2)) * 100;\n\
            float main_sky_color = sin(time / 10) + 0.15f;\n\
            color = vec4(distance * sky_mod.x + sky_col.x * main_sky_color, distance * sky_mod.y + sky_col.y * main_sky_color, distance * sky_mod.z + sky_col.z * main_sky_color, 1);\n\
            break;\n\
        case 1:\n\
            vec3 modelview_pos = vec4(modelview * vec4(o_pos, 1)).xyz;\n\
            //modelview_pos /= 4096;\n\
            float r = modelview_pos.x / 1024 - int (modelview_pos.x / 1024);\n\
            float g = modelview_pos.y / 1024 - int (modelview_pos.y / 1024);\n\
            float b = modelview_pos.z / 1024 - int (modelview_pos.z / 1024);\n\
            \n\
            if(draw_attenuation == -1)\n\
                color = vec4(r, g, b, 1);\n\
            else\n\
            {\n\
                vec3 get_light = add_light(vec3(0, 10000, 10000000), modelview_pos, vec3(1, 0.98f, 0.65f), 200000, 40000000, 75000, 3);\n\
                float alpha = get_alpha();\n\
                if(alpha == 0) discard;\n\
                vec3 rgb = vec3(r, g, b);\n\
                float time_val = max(sin(time / 10) + 0.15f, 0.05f);\n\
                vec3 ambient = time_val * sky_col;\n\
                vec3 diffuse_color = o_col * ((ambient + vec3(time_val, time_val, time_val) + vec3(time_val, time_val, time_val)) / 3) + get_light;\n\
                vec3 mvp = vec4(modelview * vec4(o_pos, 1)).xyz;\n\
                float distance = sqrt(pow(mvp.x - player_pos.x, 2) + pow(mvp.y - player_pos.y, 2) * 150 + pow(mvp.z - player_pos.z, 2));\n\
                float atmos_layer = distance / 2000000;\n\
                if(atmos_layer > 1) atmos_layer = 1;\n\
                color = vec4(diffuse_color * (1 - atmos_layer) + (ambient + vec3(0.1f, 0.1f, 0.1f)) * atmos_layer, alpha);\n\
            }\n\
            break;\n\
    }\n\
}\n\
\n\
vec3 add_light(vec3 pos, vec3 cur, vec3 color, float dist, float attenuation, float speed, float pause)\n\
{\n\
    float distance = sqrt(pow(pos.x - cur.x, 2) + pow(pos.y - cur.y, 2) + pow(pos.z - cur.z, 2));\n\
    float wave = distance - time * speed;\n\
    float pause_dist = dist * pause;\n\
    float wave_round = floor(wave / pause_dist) * pause_dist;\n\
    wave -= wave_round;\n\
    float brightness = (1 - distance / attenuation) * (wave / dist);\n\
    brightness -= pause - 1;\n\
    if(brightness < 0) brightness = 0;\n\
    return vec3(brightness * color.x, brightness * color.y, brightness * color.z);\n\
}\n\
\n\
float get_alpha()\n\
{\n\
    float alpha = 0;\n\
    vec3 mvp = vec4(modelview * vec4(o_pos, 1)).xyz;\n\
    float distance = sqrt(pow(mvp.x - player_pos.x, 2) + pow(mvp.y - player_pos.y, 2) + pow(mvp.z - player_pos.z, 2));\n\
    if(distance < draw_distance.x - draw_attenuation)\n\
    {\n\
        alpha = 0;\n\
    }\n\
    else if(distance >= draw_distance.x - draw_attenuation && distance < draw_distance.x)\n\
    {\n\
        alpha = 1;\n\
    }\n\
    else if(distance >= draw_distance.x && distance <= draw_distance.y)\n\
    {\n\
        alpha = 1;\n\
    }\n\
    else if(distance > draw_distance.y && distance < draw_distance.y + draw_attenuation)\n\
    {\n\
        alpha = (1 + draw_distance.y - distance) / draw_attenuation;\n\
    }\n\
    else\n\
    {\n\
        alpha = 0;\n\
    }\n\
    return alpha;\n\
}\n\0";

void exit(GLFWwindow* window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

GLFWwindow* init(const char* title)
{
    glfwInit();
    return glfwCreateWindow(1280, 720, title, NULL, NULL);
}

void init_gl(GLFWwindow* window)
{
    glfwMakeContextCurrent(window);
    glewInit();
}

void init_shaders(Shader* shader_main)
{
    shader_main->add_shader(vs_main, GL_VERTEX_SHADER);
    shader_main->add_shader(fs_main, GL_FRAGMENT_SHADER);

    shader_main->compile();
}

bool is_open(GLFWwindow* window)
{
    return !glfwWindowShouldClose(window);
}

void loop(GLFWwindow* window)
{
    engine_delta = glfwGetTime() - engine_time;
    engine_time = glfwGetTime();

    std::cout << 1 / engine_delta << std::endl;

    glfwGetWindowSize(window, &engine_width, &engine_height);
    glViewport(0, 0, engine_width, engine_height);

    glfwSwapBuffers(window);
    glfwPollEvents();

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1))
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if(glfwGetKey(window, GLFW_KEY_ESCAPE))
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void world_update()
{
    while(is_running)
    {
        world->update_alt();
    }
}

int main()
{
    GLFWwindow* window = init("Demo");
    init_gl(window);

    Shader shader_main;
    init_shaders(&shader_main);

    Renderer renderer;

    world = new World(1110111);
    Player player;
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    is_running = true;
    std::thread updater{world_update};

    glfwSwapInterval(1);

    while(is_open(window))
    {
        loop(window);

        renderer.prepare();
        shader_main.use();
        shader_main.transfer_float("time", engine_time);

        glEnable(GL_DEPTH_TEST);

        double cursor_x, cursor_y;
        glfwGetCursorPos(window, &cursor_x, &cursor_y);

        player.update_camera(cursor_x, cursor_y, 0.005f);
        player.update_movement(window, true);
        //player.update_collision(world);
        player.update_matrices(shader_main);

        world->update_main(player.get_pos());
        world->draw(renderer, shader_main, player.get_pos(), player.get_rot());
    }

    is_running = false;
    updater.join();

    world->destroy();
    delete world;

    exit(window);
    return 0;
}