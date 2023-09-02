#ifndef _SECTOR_H_
#define _SECTOR_H_

#include "../rendering/mesh.h"
#include "../rendering/renderer.h"
#include "../rendering/shader.h"

class Sector
{
    public:
        Sector(Vec3 pos, Vec3 scl, Vec3 foliage_color, double (*generate_point) (Vec3));

        void build();

        void destroy();
        void draw(Renderer& renderer, Shader& shader, unsigned int level, unsigned int level_cutoff);

        void generate_color(Vec3 v, bool is_quad);
        void generate_tetrahedra(Vec3 p1, Vec3 p2, Vec3 p3, Vec3 p4, double a, double b, double c, double d, unsigned int& index_count);

        Vec3 get_pos() const { return pos; }
        Vec3 get_scl() const { return scl; }

        void load(float resolution, float tolerance, int jump_amnt);

        bool marked;
        int state;
    private:
        Vec3 pos, scl, foliage_color;
        Mesh* mesh;

        double (*generate_point) (Vec3);
};

#endif