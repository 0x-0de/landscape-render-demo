#include "utilmath.h"

#include <algorithm>
#include <cmath>

static unsigned char permutations[] =
{4, 12, 4, 7, 8, 1, 2, 11, 5, 5, 9, 0, 1, 6, 3, 2, 12, 7, 0, 11, 4, 1, 6, 11, 5, 8, 8, 3, 10, 12, 2, 9, 10, 0, 9, 3, 6, 7, 10};

Vec2::Vec2()
{
    x = 0;
    y = 0;
}

double Vec2::length()
{
    return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
}

Vec3::Vec3()
{
    x = 0;
    y = 0;
    z = 0;
}

double Vec3::length()
{
    return std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));
}

Vec4::Vec4()
{
    x = 0;
    y = 0;
    z = 0;
    w = 0;
}

double Vec4::length()
{
    return std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2) + std::pow(w, 2));
}

Vec2::Vec2(double x, double y) : x(x), y(y) {}
Vec3::Vec3(double x, double y, double z) : x(x), y(y), z(z) {}
Vec4::Vec4(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}

Mat4x4::Mat4x4()
{
    for(int x = 0; x < 4; x++)
    for(int y = 0; y < 4; y++)
    {
        if(x == y) m[x][y] = 1;
        else m[x][y] = 0;
    }
}

float* Mat4x4::get_readable_array(float* blank)
{
    for(int i = 0; i < 16; i++)
    {
        blank[i] = (float) m[i / 4][i % 4];
    }

    return blank;
}

Ray::Ray(Vec3 a, Vec3 b) : a(a), b(b)
{
    delta = normalize(b - a);
}

Vec3 Ray::get_point(double t)
{
    t_ = t;
    return a + (delta * t);
}

Vec3 Ray::get_point_closest(Vec3 point)
{
    t_ = -dot(b - a, a - point) / dot(b - a, b - a);
    return get_point(t_);
}

Vec3 Ray::get_difference()
{
    return b - a;
}

void Ray::set_recent_t(double t)
{
    t_ = t;
}

Plane::Plane() {}

Plane::Plane(Vec3 a, Vec3 b, Vec3 c)
{
    Vec3 ab = b - a;
    Vec3 cb = b - c;

    Vec3 normal = normalize(cross(ab, cb));

    v_a = normal.x;
    v_b = normal.y;
    v_c = normal.z;

    double p_x = v_a * -a.x;
    double p_y = v_b * -a.y;
    double p_z = v_c * -a.z;

    v_d = p_x + p_y + p_z;

    //Ax + By + Cz + D = 0
}

Vec3 Plane::line_intersection(Ray ray)
{
    Vec3 origin = ray.get_origin();
    Vec3 direction = ray.get_direction();

    double const_a = v_a * origin.x;
    double const_b = v_b * origin.y;
    double const_c = v_c * origin.z;

    double constant = const_a + const_b + const_c; 

    double param_a = v_a * direction.x;
    double param_b = v_b * direction.y;
    double param_c = v_c * direction.z;

    double parameter = param_a + param_b + param_c;

    double t = (-v_d - constant) / parameter;

    Vec3 result = ray.get_point(t);

    result *= 10000;
    result.x = std::round(result.x);
    result.y = std::round(result.y);
    result.z = std::round(result.z);
    result /= 10000;

    return result;
}

double Plane::line_intersection_val(Ray ray)
{
    Vec3 origin = ray.get_origin();
    Vec3 direction = ray.get_direction();

    double const_a = v_a * origin.x;
    double const_b = v_b * origin.y;
    double const_c = v_c * origin.z;

    double constant = const_a + const_b + const_c; 

    double param_a = v_a * direction.x;
    double param_b = v_b * direction.y;
    double param_c = v_c * direction.z;

    double parameter = param_a + param_b + param_c;

    double t = (-v_d - constant) / parameter;

    return t;
}

Vec3 Plane::get_normal()
{
    return Vec3(v_a, v_b, v_c);
}

bool Plane::is_parallel(Plane plane)
{
    return (plane.get_normal() == get_normal() || plane.get_normal() == -get_normal());
}

Triangle::Triangle() {}

Triangle::Triangle(Vec3 a, Vec3 b, Vec3 c) : a(a), b(b), c(c)
{
    plane = Plane(a, b, c);

    o_a = a;
    o_b = b;
    o_c = c;
}

double Triangle::triplane_intesection_val(Ray ray)
{
    return plane.line_intersection_val(ray);
}

double Triangle::area()
{
    Vec3 ab = b - a;
    Vec3 cb = c - b;
    Vec3 ac = a - c;

    double length_a = ab.length();
    double length_b = cb.length();
    double length_c = ac.length();

    double semi_perimeter = (length_a + length_b + length_c) / 2;

    double area = std::sqrt(semi_perimeter * (semi_perimeter - length_a) * (semi_perimeter - length_b) * (semi_perimeter - length_c));
    return area;
}

void Triangle::transform(Vec3 translation, Vec3 rotation, Vec3 scale)
{
    Mat4x4 trans = mat_transform(translation, rotation, scale);

    Vec4 v_a = Vec4(a.x, a.y, a.z, 1);
    Vec4 v_b = Vec4(b.x, b.y, b.z, 1);
    Vec4 v_c = Vec4(c.x, c.y, c.z, 1);

    v_a = trans * v_a;
    v_b = trans * v_b;
    v_c = trans * v_c;

    a = Vec3(v_a.x, v_a.y, v_a.z);
    b = Vec3(v_b.x, v_b.y, v_b.z);
    c = Vec3(v_c.x, v_c.y, v_c.z);

    plane = Plane(a, b, c);
}

bool Triangle::ray_intersects(Ray& ray)
{
    double plane_intersection_t = triplane_intesection_val(ray);
    Vec3 plane_intersection = ray.get_point(plane_intersection_t);

    ray.set_recent_t(plane_intersection_t);

    double area_total = area();

    double area_pab = Triangle(plane_intersection, a, b).area();
    double area_pbc = Triangle(plane_intersection, b, c).area();
    double area_pca = Triangle(plane_intersection, c, a).area();

    double sum = area_pab + area_pbc + area_pca;

    return (sum >= area_total - 0.0001 && sum <= area_total + 0.0001 && plane_intersection_t >= 0);
}

Vec3 Triangle::get_normal()
{
    return plane.get_normal();
}

void Triangle::set_a(Vec3 val)
{
    a = val;
}

void Triangle::set_b(Vec3 val)
{
    b = val;
}

void Triangle::set_c(Vec3 val)
{
    c = val;
}

Vec2 normalize(Vec2 v)
{
    double distance = std::sqrt(std::pow(v.x, 2) + std::pow(v.y, 2));
    return v / distance;
}

Vec3 normalize(Vec3 v)
{
    double distance = std::sqrt(std::pow(v.x, 2) + std::pow(v.y, 2) + std::pow(v.z, 2));
    return v / distance;
}

Vec4 normalize(Vec4 v)
{
    double distance = std::sqrt(std::pow(v.x, 2) + std::pow(v.y, 2) + std::pow(v.z, 2) + std::pow(v.w, 2));
    return v / distance;
}

double dot(Vec2 a, Vec2 b)
{
    return a.x * b.x + a.y * b.y;
}

double dot(Vec3 a, Vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

double dot(Vec4 a, Vec4 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

double angle(Vec2 a, Vec2 b)
{
    return std::acos((a.x * b.x + a.y * b.y) / (std::sqrt(std::pow(a.x, 2) + std::pow(a.y, 2)) * std::sqrt(std::pow(b.x, 2) + std::pow(b.y, 2))));
}

double angle(Vec3 a, Vec3 b)
{
    return std::acos((a.x * b.x + a.y * b.y + a.z * b.z) / (std::sqrt(std::pow(a.x, 2) + std::pow(a.y, 2) + std::pow(a.z, 2)) * std::sqrt(std::pow(b.x, 2) + std::pow(b.y, 2) + std::pow(b.z, 2))));
}

double angle(Vec4 a, Vec4 b)
{
    return std::acos((a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w) / (std::sqrt(std::pow(a.x, 2) + std::pow(a.y, 2) + std::pow(a.z, 2) + std::pow(a.w, 2)) * std::sqrt(std::pow(b.x, 2) + std::pow(b.y, 2) + std::pow(b.z, 2) + std::pow(b.w, 2))));
}

double interp_linear(double a, double b, double t)
{
    return (b - a) * t + a;
}

double interp_cosine(double a, double b, double t)
{
    double f = (1 - std::cos(t * 3.1415927)) / 2;
    return (b - a) * f + a;
}

Vec3 midpoint_linear(Vec3 a, Vec3 b, double va, double vb, double t)
{
    double mu;
    Vec3 p;

    if(std::abs(t - va) < 0.00001)
        return a;
    if(std::abs(t - vb) < 0.00001)
        return b;
    if (std::abs(va - vb) < 0.00001)
        return a;
    
    mu = (t - va) / (vb - va);
    p.x = a.x + mu * (b.x - a.x);
    p.y = a.y + mu * (b.y - a.y);
    p.z = a.z + mu * (b.z - a.z);

    return p;
}

unsigned int gen_random(int seed, int x)
{
    if(x == 0) x = 3771;
    if(x < 0) x -= 6023;
    long a = (seed * 0xF324D9) % 0x8232AE;
    long b = (x * 0xB34E2C1) % 0x591C23B;
    return std::abs(a * b) % 256;
}

unsigned int gen_random(int seed, int x, int y)
{
    if(x == 0) x = 3771;
    if(y == 0) y = 2621;
    if(x < 0) x -= 6023;
    if(y < 0) y -= 4511;
    long a = (seed * 0x2834D9) % 0x8D32AE;
    long b = (x * 0xB343AC1) % 0x501C23B;
    long c = (y * 0x67E3007) % 0x910B923;
    return std::abs(a * b * c) % 256;
}

unsigned int gen_random(int seed, int x, int y, int z)
{
    if(x == 0) x = 3771;
    if(y == 0) y = 2621;
    if(z == 0) z = 6911;
    if(x < 0) x -= 6023;
    if(y < 0) y -= 4511;
    if(z < 0) z -= 8001;
    long a = (seed * 0x283FD9) % 0x8232AE;
    long b = (x * 0xB313AC1) % 0x593C23B;
    long c = (y * 0x67E3007) % 0x9123923;
    long d = (z * 0x8FA1811) % 0xE21A91B;
    return std::abs(a * b * c * d) % 256;
}

int gen_random_large(int seed, int x)
{
    unsigned int p = seed * x + seed * x + seed + x;
    for(int i = 0; i < 4; i++)
        p *= permutations[p % 39] + (x % 1000) * 811;
    return (p + x + seed + (seed * x)) % 1000000000;
}

int gen_random_large(int seed, int x, int y)
{
    unsigned int p = seed * x * y + seed * x * y + seed + x + y;
    for(int i = 0; i < 4; i++)
        p *= permutations[p % 39] + (x % 1000) + (y % 1000) * 811;
    return (p + x + y + seed + (seed * x * y)) % 1000000000;
}

int gen_random_large(int seed, int x, int y, int z)
{
    unsigned int p = seed * x * y * z + seed * x * y * z + seed + x + y + z;
    for(int i = 0; i < 4; i++)
        p *= permutations[p % 39] + (x % 1000) + (y % 1000) + (z % 1000) * 811;
    return (p + x + y + seed + (seed * x * y * z)) % 1000000000;
}

double perlin_value(int seed, double x, double y)
{
    int ix = std::floor(x);
    int iy = std::floor(y);

    double dx = x - (double) ix;
    double dy = y - (double) iy;

    int aa = gen_random(seed, ix, iy);
    int ba = gen_random(seed, ix + 1, iy);
    int ab = gen_random(seed, ix, iy + 1);
    int bb = gen_random(seed, ix + 1, iy + 1);

    return interp_cosine(interp_cosine(aa, ba, dx), interp_cosine(ab, bb, dx), dy) / 128 - 1;
}

double perlin_value(int seed, double x, double y, double z)
{
    int ix = std::floor(x);
    int iy = std::floor(y);
    int iz = std::floor(z);

    double dx = x - (double) ix;
    double dy = y - (double) iy;
    double dz = z - (double) iz;

    int aaa = gen_random(seed, ix, iy, iz);
    int baa = gen_random(seed, ix + 1, iy, iz);
    int aba = gen_random(seed, ix, iy + 1, iz);
    int bba = gen_random(seed, ix + 1, iy + 1, iz);
    int aab = gen_random(seed, ix, iy, iz + 1);
    int bab = gen_random(seed, ix + 1, iy, iz + 1);
    int abb = gen_random(seed, ix, iy + 1, iz + 1);
    int bbb = gen_random(seed, ix + 1, iy + 1, iz + 1);

    return interp_cosine(interp_cosine(interp_cosine(aaa, baa, dx), interp_cosine(aba, bba, dx), dy), interp_cosine(interp_cosine(aab, bab, dx), interp_cosine(abb, bbb, dx), dy), dz) / 128 - 1;
}

double fractal_noise(int seed, double x, double y, unsigned int octaves, float persistance)
{
    float result = perlin_value(seed, x, y);
    for(unsigned int i = 1; i < octaves; i++)
    {
        float d = std::pow(2, i);
        result += (perlin_value(seed + i, x * d, y * d)) / d * persistance;
    }
    return result;
}

double fractal_noise(int seed, double x, double y, double z, unsigned int octaves, float persistance)
{
    float result = perlin_value(seed, x, y, z);
    for(unsigned int i = 1; i < octaves; i++)
    {
        float d = std::pow(2, i);
        result += (perlin_value(seed + i, x * d, y * d, z * d)) / d * persistance;
    }
    return result;
}

Vec3 cross(Vec3 a, Vec3 b)
{
    Vec3 product;
    product.x = a.y * b.z - a.z * b.y;
    product.y = a.z * b.x - a.x * b.z;
    product.z = a.x * b.y - a.y * b.x;
    return product;
}

float midpoint(float a, float b, float t)
{
    return (t - a) / (b - a);
}

float fade(float t)
{
    return (1 / (1 + std::pow(2.71828, -5 * t)) - 0.5f) * 2;
}

bool point_within_rectangle(Vec3 rec_a, Vec3 rec_b, Vec3 point)
{
    return (point.x >= rec_a.x && point.y >= rec_a.y && point.z >= rec_a.z && point.x <= rec_b.x && point.y <= rec_b.y && point.z <= rec_b.z);
}

Mat4x4 mat_transform(Vec3 translation, Vec3 rotation, Vec3 scale)
{
    Mat4x4 rot_x;
    Mat4x4 rot_y;
    Mat4x4 rot_z;

    rot_x.m[1][1] = std::cos(rotation.x);
    rot_x.m[2][1] = -std::sin(rotation.x);
    rot_x.m[1][2] = std::sin(rotation.x);
    rot_x.m[2][2] = std::cos(rotation.x);

    rot_y.m[0][0] = std::cos(rotation.y);
    rot_y.m[2][0] = std::sin(rotation.y);
    rot_y.m[0][2] = -std::sin(rotation.y);
    rot_y.m[2][2] = std::cos(rotation.y);

    rot_z.m[0][0] = std::cos(rotation.z);
    rot_z.m[1][0] = -std::sin(rotation.z);
    rot_z.m[0][1] = std::sin(rotation.z);
    rot_z.m[1][1] = std::cos(rotation.z);

    Mat4x4 rot = rot_x * (rot_y * rot_z);

    Mat4x4 scl;
    Mat4x4 pos;

    scl.m[0][0] = scale.x;
    scl.m[1][1] = scale.y;
    scl.m[2][2] = scale.z;

    pos.m[3][0] = translation.x;
    pos.m[3][1] = translation.y;
    pos.m[3][2] = translation.z;

    return pos * (rot * scl);
}

Mat4x4 mat_perspective(double fov, double aspect, double near, double far)
{
    Mat4x4 matrix;
    matrix.m[0][0] = 1 / (aspect * std::tan(fov / 2));
    matrix.m[1][1] = 1 / std::tan(fov / 2);
    matrix.m[2][2] = -(far + near) / (far - near);
    matrix.m[3][2] = -(2 * far * near) / (far - near);
    matrix.m[2][3] = -1;
    matrix.m[3][3] = 0;
    return matrix;
}

Mat4x4 mat_view(Vec3 position, Vec3 target, Vec3 up_axis)
{
    Mat4x4 matrix;
    Vec3 forward = normalize(target - position);
    Vec3 side = normalize(cross(forward, up_axis));
    Vec3 up = normalize(cross(side, forward));

    matrix.m[0][0] = side.x;
    matrix.m[1][0] = side.y;
    matrix.m[2][0] = side.z;
    matrix.m[0][1] = up.x;
    matrix.m[1][1] = up.y;
    matrix.m[2][1] = up.z;
    matrix.m[0][2] = forward.x;
    matrix.m[1][2] = forward.y;
    matrix.m[2][2] = forward.z;
    matrix.m[3][0] = dot(side, -position);
    matrix.m[3][1] = dot(up, -position);
    matrix.m[3][2] = dot(forward, -position);

    return matrix;
}

Mat4x4 mat_ortho(float left, float right, float bottom, float top, float near, float far)
{
    Mat4x4 matrix;

    matrix.m[0][0] = 2 / (right - left);
    matrix.m[1][1] = 2 / (top - bottom);
    matrix.m[2][2] = 1 / (far - near);
    matrix.m[3][2] = -near / (far - near);

    return matrix;
}

unsigned int read_block(char* block, bool big_endian)
{
    unsigned int val = 0;
    unsigned char u_block[4];

    for(unsigned int i = 0; i < 4; i++)
    {
        u_block[i] = block[i];
    }

    if(big_endian)
    {
        val += u_block[3] * std::pow(256, 3);
        val += u_block[2] * std::pow(256, 2);
        val += u_block[1] * 256;
        val += u_block[0];
    }
    else
    {
        val += u_block[0] * std::pow(256, 3);
        val += u_block[1] * std::pow(256, 2);
        val += u_block[2] * 256;
        val += u_block[3];
    }
    return val;
}