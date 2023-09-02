#ifndef _MESH_H_
#define _MESH_H_

#include <vector>

#include "../utils/utilmath.h"

class Mesh
{
    public:
        Mesh(unsigned char size);

        void add_data(unsigned int index, float val);
        void add_data(unsigned int index, Vec2 val);
        void add_data(unsigned int index, Vec3 val);
        void add_data(unsigned int index, Vec4 val);

        void add_index(unsigned int index);

        void add_indices(unsigned int* ind, unsigned int size);
        void add_indices(std::vector<unsigned int> ind);

        void bind();

        void build();

        void clear_data();
        void destroy();

        unsigned int get_size() const { return size; }
    private:
        unsigned int vao, ibo, size;

        std::vector<unsigned int> vbos, indices;
        std::vector<std::vector<float> > data;
        std::vector<unsigned char> dimensions;
};

#endif