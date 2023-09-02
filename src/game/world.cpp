#include "world.h"

#include <cmath>
#include <iostream>

#include "../../lib/GL/glew.h"

static unsigned int noise_seed;

double generate_point(Vec3 point)
{
    double val_1 = fractal_noise(noise_seed, point.x / 2000, point.y / 200, point.z / 2000, 4, 1.5f) * 500;
    double val_2 = (fractal_noise(noise_seed + 1, point.x / 250, point.z / 250, 1, 1) + 1) * 2;
    double val_3 = fractal_noise(noise_seed + 2, point.x / 10, point.y / 10, point.z / 10, 3, 1.5f);

    double val = val_1 + (val_2 * val_3) - (point.y);
    return val;
}

World::World(unsigned int seed) : seed(seed)
{
    cur_process = 0;
    player_speed_cur = 0;

    skybox = new Mesh(1);
    create_skybox();

    noise_seed = seed;
    sectors.resize(11);
}

void World::create_skybox()
{
    skybox->add_data(0, Vec3(-1, 0.0001f, -1));
    skybox->add_data(0, Vec3(1, 0.0001f, -1));
    skybox->add_data(0, Vec3(1, 0.0001f, 1));
    skybox->add_data(0, Vec3(-1, 0.0001f, 1));

    skybox->add_data(0, Vec3(-1, 0.0001f, -1));
    skybox->add_data(0, Vec3(-1, -1, -1));
    skybox->add_data(0, Vec3(-1, -1, 1));
    skybox->add_data(0, Vec3(-1, 0.0001f, 1));

    skybox->add_data(0, Vec3(1, 0.0001f, -1));
    skybox->add_data(0, Vec3(1, -1, -1));
    skybox->add_data(0, Vec3(1, -1, 1));
    skybox->add_data(0, Vec3(1, 0.0001f, 1));

    skybox->add_data(0, Vec3(1, 0.0001f, -1));
    skybox->add_data(0, Vec3(1, -1, -1));
    skybox->add_data(0, Vec3(-1, -1, -1));
    skybox->add_data(0, Vec3(-1, 0.0001f, -1));

    skybox->add_data(0, Vec3(1, 0.0001f, 1));
    skybox->add_data(0, Vec3(1, -1, 1));
    skybox->add_data(0, Vec3(-1, -1, 1));
    skybox->add_data(0, Vec3(-1, 0.0001f, 1));

    for(int i = 0; i < 5; i++)
    {
        skybox->add_index(0 + i * 4);
        skybox->add_index(1 + i * 4);
        skybox->add_index(2 + i * 4);
        skybox->add_index(0 + i * 4);
        skybox->add_index(3 + i * 4);
        skybox->add_index(2 + i * 4);
    }

    skybox->build();
    skybox->clear_data();

    sky_col = Vec3(0.3f, 0.6f, 1);
    sky_mod = Vec3(1, 0.8f, 0.2f);

    foliage_color = Vec3(0.7f, 1, 0.6f);
}

void World::destroy()
{
    for(unsigned int i = 0; i < sectors.size(); i++)
    for(unsigned int j = 0; j < sectors[i].size(); j++)
    {
        sectors[i][j].destroy();
    }

    for(unsigned int i = 0; i < sectors.size(); i++)
        std::vector<Sector>().swap(sectors[i]);

    std::vector<std::vector<Sector> >().swap(sectors);

    delete skybox;
}

void World::draw(Renderer& renderer, Shader& shader, Vec3 player_pos, Vec3 player_rot)
{
    glDisable(GL_DEPTH_TEST);

    shader.transfer_mat4("projection", mat_perspective(70.f, (float) engine_width / (float) engine_height, 0.00001f, 5.f));
    shader.transfer_mat4("viewport", mat_view(Vec3(0, 0, 0), player_rot, Vec3(0, 1, 0)));
    shader.transfer_mat4("transform", mat_transform(Vec3(0, 0, 0), Vec3(0, 0, 0), Vec3(1, 1, 1)));

    shader.transfer_vec3("sky_col", sky_col);
    shader.transfer_vec3("sky_mod", sky_mod);
    shader.transfer_int("draw_mode", 0);

    renderer.draw_mesh(skybox);

    glEnable(GL_DEPTH_TEST);

    shader.transfer_mat4("projection", mat_perspective(70.f, (float) engine_width / (float) engine_height, 0.5f, 2000000.f));
    shader.transfer_mat4("viewport", mat_view(player_pos, player_pos + player_rot, Vec3(0, 1, 0)));

    shader.transfer_int("draw_mode", 1);

    for(int i = sectors.size() - 1; i >= update_level; i--)
    for(unsigned int j = 0; j < sectors[i].size(); j++)
    {
        if(sectors[i][j].state == 2)
            sectors[i][j].draw(renderer, shader, i, update_level);
        sectors[i][j].marked = false;
    }
}

int World::get_sector_index(int level, int x, int y, int z)
{
    for(unsigned int i = 0; i < sectors[level].size(); i++)
    {
        if(sectors[level][i].get_pos().x == x && sectors[level][i].get_pos().y == y && sectors[level][i].get_pos().z == z)
            return i;
    }

    return -1;
}

void World::update_main(Vec3 player_pos)
{
    if(player_speed_cur < 25)
    {
        update_level = 0;
    }
    else if(player_speed_cur < 50)
    {
        update_level = 1;
    }
    else if(player_speed_cur < 100)
    {
        update_level = 2;
    }
    else if(player_speed_cur < 200)
    {
        update_level = 3;
    }
    else if(player_speed_cur < 700)
    {
        update_level = 4;
    }
    else if(player_speed_cur < 1500)
    {
        update_level = 5;
    }
    else if(player_speed_cur < 4000)
    {
        update_level = 6;
    }
    else
    {
        update_level = 7;
    }

    if(cur_process == 0)
    {
        for(unsigned int i = update_level; i < 11; i++)
        {
            float factor = std::pow(3, i);
            Vec3 p = player_pos / (32 * factor);

            float offset = std::floor((factor * 3) / 2);

            if(i == 0) offset = 1;
            else p += Vec3(offset / (factor * 3), offset / (factor * 3), offset / (factor * 3));

            p.x = std::floor(p.x);
            p.y = std::floor(p.y);
            p.z = std::floor(p.z);

            p *= factor;

            float end = 1;
            for(unsigned int j = 0; j < i; j++)
            {
                end += std::pow(3, j);
            }

            //std::cout << i << ": " << factor << ", " << offset << ", " << end << " : " << 32 * factor << std::endl;

            for(int x = p.x - offset; x <= p.x + end; x += (int) factor)
            for(int y = p.y - offset; y <= p.y + end; y += (int) factor)
            for(int z = p.z - offset; z <= p.z + end; z += (int) factor)
            {
                int index = get_sector_index(i, x, y, z);
                if(index == -1)
                {
                    Sector sector(Vec3(x, y, z), Vec3(32 * factor, 32 * factor, 32 * factor), foliage_color, generate_point);
                    sectors[i].push_back(sector);
                }
                else
                {
                    sectors[i][index].marked = true;
                }
            }
        }

        for(unsigned int i = 0; i < sectors.size(); i++)
        for(unsigned int j = 0; j < sectors[i].size(); j++)
        {
            if(!sectors[i][j].marked)
            {
                sectors[i][j].destroy();
                sectors[i].erase(sectors[i].begin() + j);
            }
        }

        cur_process = 1;
    }

    for(unsigned int i = 0; i < sectors.size(); i++)
    for(unsigned int j = 0; j < sectors[i].size(); j++)
    {
        if(sectors[i][j].state == 1)
        {
            sectors[i][j].build();
        }
    }
}

void World::update_alt()
{
    if(cur_process == 1)
    {
        for(unsigned int i = 0; i < sectors.size(); i++)
        for(unsigned int j = 0; j < sectors[i].size(); j++)
        {
            if(sectors[i][j].state == 0)
            {
                float tolerance;
                int jump_amnt;
                switch(i)
                {
                    case 0:
                        tolerance = 0;
                        jump_amnt = 8;
                        break;
                    default:
                        tolerance = 999999;
                        jump_amnt = 0;
                        break;
                }
                sectors[i][j].load(32, tolerance, jump_amnt);
            }
        }

        cur_process = 0;
    }
}