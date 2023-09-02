#include "mesh.h"

#include <iostream>

#include "../../lib/GL/glew.h"

Mesh::Mesh(unsigned char size)
{
    dimensions.resize(size);
    data.resize(size);

    glGenVertexArrays(1, &vao);

    for(unsigned int i = 0; i < size; i++)
    {
        unsigned int vbo;
        glGenBuffers(1, &vbo);
        vbos.push_back(vbo);
    }

    glGenBuffers(1, &ibo);
}

void Mesh::add_data(unsigned int index, float val)
{
    data[index].push_back(val);
    dimensions[index] = 1;
}

void Mesh::add_data(unsigned int index, Vec2 val)
{
    data[index].push_back(val.x);
    data[index].push_back(val.y);
    dimensions[index] = 2;
}

void Mesh::add_data(unsigned int index, Vec3 val)
{
    data[index].push_back(val.x);
    data[index].push_back(val.y);
    data[index].push_back(val.z);
    dimensions[index] = 3;
}

void Mesh::add_data(unsigned int index, Vec4 val)
{
    data[index].push_back(val.x);
    data[index].push_back(val.y);
    data[index].push_back(val.z);
    data[index].push_back(val.w);
    dimensions[index] = 4;
}

void Mesh::add_index(unsigned int index)
{
    indices.push_back(index);
}

void Mesh::add_indices(unsigned int* ind, unsigned int size)
{
    for(unsigned int i = 0; i < size; i++)
        indices.push_back(ind[i]);
}

void Mesh::add_indices(std::vector<unsigned int> ind)
{
    for(unsigned int i = 0; i < ind.size(); i++)
        indices.push_back(ind[i]);
}

void Mesh::bind()
{
    glBindVertexArray(vao);
}

void Mesh::build()
{
    bind();

    for(unsigned int i = 0; i < data.size(); i++)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
        glBufferData(GL_ARRAY_BUFFER, data[i].size() * sizeof(float), data[i].data(), GL_STATIC_DRAW);
        glVertexAttribPointer(i, dimensions[i], GL_FLOAT, false, dimensions[i] * sizeof(float), 0);
        glEnableVertexAttribArray(i);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    size = indices.size();

    clear_data();
}

void Mesh::clear_data()
{
    for(unsigned int i = 0; i < data.size(); i++)
    {
        std::vector<float>().swap(data[i]);
    }

    std::vector<std::vector<float> >().swap(data);
    std::vector<unsigned int>().swap(indices);
}

void Mesh::destroy()
{
    clear_data();

    for(unsigned int i = 0; i < vbos.size(); i++)
    {
        glDeleteBuffers(1, &vbos[i]);
    }

    glDeleteBuffers(1, &ibo);
    glDeleteVertexArrays(1, &vao);

    std::vector<unsigned char>().swap(dimensions);
    std::vector<unsigned int>().swap(vbos);
}