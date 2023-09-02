#include "sector.h"

#include <cmath>
#include <iostream>

#include "../../lib/GLFW/glfw3.h"

Sector::Sector(Vec3 pos, Vec3 scl, Vec3 foliage_color, double (*generate_point) (Vec3)) : pos(pos), scl(scl), foliage_color(foliage_color), generate_point(generate_point)
{
    mesh = new Mesh(2);
    marked = true;

    state = 0;
}

void Sector::build()
{
    mesh->build();
    mesh->clear_data();

    state = 2;
}

void Sector::destroy()
{
    mesh->destroy();
    delete mesh;
}

void Sector::draw(Renderer& renderer, Shader& shader, unsigned int level, unsigned int level_cutoff)
{
    float dist = 0;
    switch(level)
    {
        case 0:
            dist = 0.5f;
            break;
        case 1:
            dist = 32.f;
            break;
        case 2:
            dist = 96.f;
            break;
        case 3:
            dist = 288.f;
            break;
        case 4:
            dist = 864.f;
            break;
        case 5:
            dist = 2592.f;
            break;
        case 6:
            dist = 7776.f;
            break;
        case 7:
            dist = 23328.f;
            break;
        case 8:
            dist = 69984.f;
            break;
        case 9:
            dist = 209952.f;
            break;
        case 10:
            dist = 629856.f;
            break;
    }

    if(level == level_cutoff)
    {
        dist = 0.5f;
    }

    shader.transfer_mat4("transform", mat_transform(pos * 32, Vec3(0, 0, 0), scl));
    shader.transfer_vec2("draw_distance", Vec2(dist, scl.x));
    shader.transfer_float("draw_attenuation", scl.x / 32);

    renderer.draw_mesh(mesh);
}

void Sector::load(float resolution, float tolerance, int jump_amnt)
{
    float r = 1.f / resolution;

    double*** data = new double**[33];
    for(unsigned int i = 0; i < 33; i++)
    {
        data[i] = new double*[33];
        for(unsigned int j = 0; j < 33; j++)
            data[i][j] = new double[33];
    }

    float s = scl.x / 32;
    bool start;
    bool should_generate = false;

    float time = glfwGetTime();

    for(float x = 0; x <= r * 32; x += r * 8)
    for(float y = 0; y <= r * 32; y += r * 8)
    for(float z = 0; z <= r * 32; z += r * 8)
    {
        if(x == 0 && y == 0 && z == 0)
        {
            start = (generate_point(Vec3(pos.x + x * s, pos.y + y * s, pos.z + z * s)) >= 0);
            continue;
        }

        float val = generate_point(Vec3(pos.x + x * s, pos.y + y * s, pos.z + z * s));
        if(start)
        {
            if(val < 0)
            {
                should_generate = true;
                break;
            }
        }
        else
        {
            if(val >= 0)
            {
                should_generate = true;
                break;
            }
        }
    }

    if(should_generate)
    {
        for(float x = 0; x <= r * 32; x += r)
        for(float y = 0; y <= r * 32; y += r)
        for(float z = 0; z <= r * 32; z += r)
        {
            int ix = x * resolution;
            int iy = y * resolution;
            int iz = z * resolution;

            data[ix][iy][iz] = generate_point(Vec3(pos.x + x * s, pos.y + y * s, pos.z + z * s));
            if(tolerance == 0 && data[ix][iy][iz] == 0) std::cout << data[ix][iy][iz] << std::endl;
            /*
            if(data[ix][iy][iz] > tolerance)
            {
                for(int i = iz; i < iz + jump_amnt; i++)
                {
                    if(i > 32) continue;
                    data[ix][iy][i] = tolerance;
                }
                z += r * (float) (jump_amnt - 1);
            }
            else if(data[ix][iy][iz] < -tolerance)
            {
                for(int i = iz; i < iz + jump_amnt; i++)
                {
                    if(i > 32) continue;
                    data[ix][iy][i] = -tolerance;
                }
                z += r * (float) (jump_amnt - 1);
            }
            */
        }

        unsigned int index_count = 0;

        for(unsigned int x = 0; x < 32; x++)
        for(unsigned int y = 0; y < 32; y++)
        for(unsigned int z = 0; z < 32; z++)
        {
            double aaa = data[x][y][z];
            double baa = data[x + 1][y][z];
            double aba = data[x][y + 1][z];
            double bba = data[x + 1][y + 1][z];
            double aab = data[x][y][z + 1];
            double bab = data[x + 1][y][z + 1];
            double abb = data[x][y + 1][z + 1];
            double bbb = data[x + 1][y + 1][z + 1];

            if(aaa < 0 && baa < 0 && aba < 0 && bba < 0 && aab < 0 && bab < 0 && abb < 0 && bbb < 0) continue;
            if(aaa >= 0 && baa >= 0 && aba >= 0 && bba >= 0 && aab >= 0 && bab >= 0 && abb >= 0 && bbb >= 0) continue;

            double dx = (double) x / resolution;
            double dy = (double) y / resolution;
            double dz = (double) z / resolution;

            generate_tetrahedra(Vec3(dx, dy, dz), Vec3(dx, dy + r, dz),  Vec3(dx + r, dy, dz), Vec3(dx + r, dy, dz + r), aaa, aba, baa, bab, index_count);
            generate_tetrahedra(Vec3(dx, dy, dz), Vec3(dx, dy + r, dz),  Vec3(dx, dy, dz + r), Vec3(dx + r, dy, dz + r), aaa, aba, aab, bab, index_count);
            generate_tetrahedra(Vec3(dx, dy + r, dz + r), Vec3(dx, dy + r, dz), Vec3(dx, dy, dz + r), Vec3(dx + r, dy, dz + r), abb, aba, aab, bab, index_count);
            generate_tetrahedra(Vec3(dx, dy + r, dz + r), Vec3(dx, dy + r, dz), Vec3(dx + r, dy + r, dz + r), Vec3(dx + r, dy, dz + r), abb, aba, bbb, bab, index_count);
            generate_tetrahedra(Vec3(dx + r, dy + r, dz), Vec3(dx, dy + r, dz), Vec3(dx + r, dy + r, dz + r), Vec3(dx + r, dy, dz + r), bba, aba, bbb, bab, index_count);
            generate_tetrahedra(Vec3(dx + r, dy + r, dz), Vec3(dx, dy + r, dz), Vec3(dx + r, dy, dz), Vec3(dx + r, dy, dz + r), bba, aba, baa, bab, index_count);
        }
    }

    for(unsigned int i = 0; i < 33; i++)
    {
        for(unsigned int j = 0; j < 33; j++)
            delete[] data[i][j];
        delete[] data[i];
    }
    delete[] data;

    state = should_generate ? 1 : -1;
    //std::cout << "Time elapsed: " << (glfwGetTime() - time) * 1000 << " ms (" << state << ")" << std::endl;
}

void Sector::generate_color(Vec3 v, bool is_quad)
{
    Vec3 point_transformed = Vec3(v * (scl.x / 32)) + pos;
    float a = generate_point(point_transformed);
    float b = generate_point(point_transformed + Vec3(0, 1.f / 32, 0));

    double color_mod = (fractal_noise(36271, point_transformed.x, point_transformed.y, point_transformed.z, 8, 1) - 0.5f) / 20;

    float diff = (b - a) * 100;
    diff += 3;
    if(diff < 0) diff = 0;
    if(diff > 1) diff = 1;

    Vec3 color = Vec3(Vec3(0.5f, 0.5f, 0.5f) * diff) + Vec3(foliage_color * (1 - diff));
    color += Vec3(color_mod, color_mod, color_mod);

    mesh->add_data(1, color);
}

void Sector::generate_tetrahedra(Vec3 p1, Vec3 p2, Vec3 p3, Vec3 p4, double a, double b, double c, double d, unsigned int& index_count)
{
    int s_case = 0;

    if(a >= 0) s_case |= 1;
    if(b >= 0) s_case |= 2;
    if(c >= 0) s_case |= 4;
    if(d >= 0) s_case |= 8;

    Vec3 va, vb, vc, vd;

    switch(s_case)
    {
        case 0:
        case 15:
            break;
        case 1:
        case 14:
            va = midpoint_linear(p1, p2, a, b, 0);
            vb = midpoint_linear(p1, p3, a, c, 0);
            vc = midpoint_linear(p1, p4, a, d, 0);

            mesh->add_data(0, va);
            mesh->add_data(0, vb);
            mesh->add_data(0, vc);

            generate_color(va, false);
            generate_color(vb, false);
            generate_color(vc, false);

            mesh->add_index(0 + index_count);
            mesh->add_index(1 + index_count);
            mesh->add_index(2 + index_count);

            index_count += 3;
            break;
        case 2:
        case 13:
            va = midpoint_linear(p2, p1, b, a, 0);
            vb = midpoint_linear(p2, p3, b, c, 0);
            vc = midpoint_linear(p2, p4, b, d, 0);

            mesh->add_data(0, va);
            mesh->add_data(0, vb);
            mesh->add_data(0, vc);

            generate_color(va, false);
            generate_color(vb, false);
            generate_color(vc, false);

            mesh->add_index(0 + index_count);
            mesh->add_index(1 + index_count);
            mesh->add_index(2 + index_count);

            index_count += 3;
            break;
        case 3:
        case 12:
            va = midpoint_linear(p1, p4, a, d, 0);
            vb = midpoint_linear(p1, p3, a, c, 0);
            vc = midpoint_linear(p2, p4, b, d, 0);
            vd = midpoint_linear(p2, p3, b, c, 0);

            mesh->add_data(0, va);
            mesh->add_data(0, vb);
            mesh->add_data(0, vc);
            mesh->add_data(0, vd);

            generate_color(va, false);
            generate_color(vb, false);
            generate_color(vc, false);
            generate_color(vd, false);

            mesh->add_index(0 + index_count);
            mesh->add_index(1 + index_count);
            mesh->add_index(2 + index_count);
            mesh->add_index(2 + index_count);
            mesh->add_index(3 + index_count);
            mesh->add_index(1 + index_count);

            index_count += 4;
            break;
        case 4:
        case 11:
            va = midpoint_linear(p3, p1, c, a, 0);
            vb = midpoint_linear(p3, p2, c, b, 0);
            vc = midpoint_linear(p3, p4, c, d, 0);

            mesh->add_data(0, va);
            mesh->add_data(0, vb);
            mesh->add_data(0, vc);

            generate_color(va, false);
            generate_color(vb, false);
            generate_color(vc, false);

            mesh->add_index(0 + index_count);
            mesh->add_index(1 + index_count);
            mesh->add_index(2 + index_count);

            index_count += 3;
            break;
        case 5:
        case 10:
            va = midpoint_linear(p1, p2, a, b, 0);
            vb = midpoint_linear(p3, p4, c, d, 0);
            vc = midpoint_linear(p1, p4, a, d, 0);
            vd = midpoint_linear(p2, p3, b, c, 0);

            mesh->add_data(0, va);
            mesh->add_data(0, vb);
            mesh->add_data(0, vc);
            mesh->add_data(0, vd);

            generate_color(va, false);
            generate_color(vb, false);
            generate_color(vc, false);
            generate_color(vd, false);

            mesh->add_index(0 + index_count);
            mesh->add_index(1 + index_count);
            mesh->add_index(2 + index_count);
            mesh->add_index(0 + index_count);
            mesh->add_index(3 + index_count);
            mesh->add_index(1 + index_count);

            index_count += 4;
            break;
        case 6:
        case 9:
            va = midpoint_linear(p1, p2, a, b, 0);
            vb = midpoint_linear(p2, p4, b, d, 0);
            vc = midpoint_linear(p3, p4, c, d, 0);
            vd = midpoint_linear(p1, p3, a, c, 0);

            mesh->add_data(0, va);
            mesh->add_data(0, vb);
            mesh->add_data(0, vc);
            mesh->add_data(0, vd);

            generate_color(va, false);
            generate_color(vb, false);
            generate_color(vc, false);
            generate_color(vd, false);

            mesh->add_index(0 + index_count);
            mesh->add_index(1 + index_count);
            mesh->add_index(2 + index_count);
            mesh->add_index(0 + index_count);
            mesh->add_index(3 + index_count);
            mesh->add_index(2 + index_count);

            index_count += 4;
            break;
        case 7:
        case 8:
            va = midpoint_linear(p4, p1, d, a, 0);
            vb = midpoint_linear(p4, p2, d, b, 0);
            vc = midpoint_linear(p4, p3, d, c, 0);

            mesh->add_data(0, va);
            mesh->add_data(0, vb);
            mesh->add_data(0, vc);

            generate_color(va, false);
            generate_color(vb, false);
            generate_color(vc, false);

            mesh->add_index(0 + index_count);
            mesh->add_index(1 + index_count);
            mesh->add_index(2 + index_count);

            index_count += 3;
            break;
    }
}