#include "player.h"

#include <cmath>
#include <iostream>

Player::Player()
{
    cam_x = 0;
    cam_y = 0;
    prev_x = 0;
    prev_y = 0;
    speed = 4.f;

    cur_grav = 0;

    pos = Vec3(0, 0, 0);

    double p = generate_point(pos / 32);
    while(p > 0.5f || p < -0.5f)
    {
        if(p > 0.5f)
        {
            pos.y += 0.1f;
        }
        if(p < -0.5f)
        {
            pos.y -= 0.1f;
        }

        p = generate_point(pos / 32);
    }

    std::cout << "Initial pos: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;

    pos_prev = pos;
}

void Player::update_camera(double x, double y, double sensitivity)
{
    float delta_x = x * sensitivity - prev_x;
    float delta_y = y * sensitivity - prev_y;

    cam_x += delta_x;
    
    if(cam_y >= 1.57f)
    {
        cam_y = 1.57f;
        if(delta_y < 0)
            cam_y += delta_y;
    }
    else if(cam_y <= -1.57f)
    {
        cam_y = -1.57f;
        if(delta_y > 0)
            cam_y += delta_y;
    }
    else
        cam_y += delta_y;

    rot.x = std::sin(cam_x);
    rot.y = std::tan(cam_y);
    rot.z = std::cos(cam_x);

    rot = normalize(rot);

    prev_x = x * sensitivity;
    prev_y = y * sensitivity;
}

void Player::update_collision(World* world)
{
    world->player_speed_cur = Vec3(pos - pos_prev).length() / engine_delta;
    pos_prev = pos;

    //std::cout << "POS: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;

    float bottom_a = generate_point(Vec3(pos.x / 32, pos.y / 32, pos.z / 32) - Vec3(0, 1.f / 32, 0));
    float bottom_b = generate_point(Vec3(pos.x / 32, pos.y / 32, pos.z / 32) - Vec3(0, 2.f / 32, 0));

    float bottom_mid = midpoint(bottom_a, bottom_b, 0);

    //std::cout << bottom_a << ", " << bottom_b << " -> " << bottom_mid << std::endl;

    if(bottom_mid < 0)
    {
        pos.y += 1;
    }
    else if(bottom_mid <= 0.55f)
    {
        cur_grav = 0;
        if(vel.y < 0)
        {
            vel.y = 0;
        }
        pos.y += (0.5f - bottom_mid) / 2;
    }
    else
    {
        cur_grav -= 0.25f;
        if(cur_grav < -100)
        {
            cur_grav = -100;
        }

        vel.y = cur_grav * engine_delta;
    }

    pos += vel;
}

void Player::update_matrices(Shader& shader)
{
    shader.transfer_vec3("player_pos", pos);
}

void Player::update_movement(GLFWwindow* window, bool is_freemoving)
{
    Vec3 side_free = normalize(cross(rot, Vec3(0, 1, 0)));
    vel = Vec3(0, 0, 0);

    if(is_freemoving)
    {
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
        {
            speed += engine_delta * 600;
            std::cout << "SPEED: " << speed << std::endl;
        }
        else
        {
            speed = 4.f;
        }

        if(glfwGetKey(window, GLFW_KEY_W))
        {
            pos -= rot * (engine_delta * speed);
        }
        if(glfwGetKey(window, GLFW_KEY_S))
        {
            pos += rot * (engine_delta * speed);
        }
        if(glfwGetKey(window, GLFW_KEY_A))
        {
            pos -= side_free * (engine_delta * speed);
        }
        if(glfwGetKey(window, GLFW_KEY_D))
        {
            pos += side_free * (engine_delta * speed);
        }
    }
    else
    {
        Vec3 forward = normalize(Vec3(rot.x, 0, rot.z));
        Vec3 side = normalize(cross(forward, Vec3(0, 1, 0)));

        if(glfwGetKey(window, GLFW_KEY_W))
        {
            vel -= forward * (engine_delta * speed);
        }
        if(glfwGetKey(window, GLFW_KEY_S))
        {
            vel += forward * (engine_delta * speed);
        }
        if(glfwGetKey(window, GLFW_KEY_A))
        {
            vel -= side * (engine_delta * speed);
        }
        if(glfwGetKey(window, GLFW_KEY_D))
        {
            vel += side * (engine_delta * speed);
        }       
    }
}