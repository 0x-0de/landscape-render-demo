#ifndef _WORLD_H_
#define _WORLD_H_

#include <vector>

#include "sector.h"

double generate_point(Vec3 point);

class World
{
    public:
        World(unsigned int seed);

        void create_skybox();

        void destroy();
        void draw(Renderer& renderer, Shader& shader, Vec3 player_pos, Vec3 player_rot);

        int get_sector_index(int level, int x, int y, int z);

        void update_main(Vec3 player_pos);
        void update_alt();

        float player_speed_cur;
    private:
        unsigned int seed, cur_process;
        int update_level;

        std::vector<std::vector<Sector> > sectors;
        Mesh* skybox;

        Vec3 sky_col, sky_mod, foliage_color;
};

#endif