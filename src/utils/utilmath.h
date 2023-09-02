#ifndef _UTILMATH_H_
#define _UTILMATH_H_

class Vec2
{
public:
    Vec2();
    Vec2(double x, double y);

    const void operator=(Vec2 v)
    {
        x = v.x;
        y = v.y;
    }

    const Vec2 operator+(double v)
    {
        return Vec2(x + v, y + v);
    }

    const Vec2 operator+=(double v)
    {
        x += v;
        y += v;
        return *this;
    }

    const Vec2 operator-()
    {
        return Vec2(-x, -y);
    }

    const Vec2 operator-(double v)
    {
        return Vec2(x - v, y - v);
    }

    const Vec2 operator-=(double v)
    {
        x -= v;
        y -= v;
        return *this;
    }

    const Vec2 operator*(double v)
    {
        return Vec2(x * v, y * v);
    }

    const Vec2 operator*=(double v)
    {
        x *= v;
        y *= v;
        return *this;
    }

    const Vec2 operator/(double v)
    {
        return Vec2(x / v, y / v);
    }

    const Vec2 operator/=(double v)
    {
        x /= v;
        y /= v;
        return *this;
    }

    const Vec2 operator+(Vec2 v)
    {
        return Vec2(x + v.x, y + v.y);
    }

    const Vec2 operator+=(Vec2 v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    const Vec2 operator-(Vec2 v)
    {
        return Vec2(x - v.x, y - v.y);
    }

    const Vec2 operator-=(Vec2 v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    const Vec2 operator*(Vec2 v)
    {
        return Vec2(x * v.x, y * v.y);
    }

    const Vec2 operator*=(Vec2 v)
    {
        x *= v.x;
        y *= v.y;
        return *this;
    }

    const Vec2 operator/(Vec2 v)
    {
        return Vec2(x / v.x, y / v.y);
    }

    const Vec2 operator/=(Vec2 v)
    {
        x /= v.x;
        y /= v.y;
        return *this;
    }

    const bool operator==(Vec2 v)
    {
        return (v.x == x && v.y == y);
    }

    const bool operator!=(Vec2 v)
    {
        return (v.x != x || v.y != y);
    }

    double length();

    double x, y;
};

class Vec3
{
public:
    Vec3();
    Vec3(double x, double y, double z);

    const void operator=(Vec3 v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
    }

    const Vec3 operator+(double v)
    {
        return Vec3(x + v, y + v, z + v);
    }

    const Vec3 operator+=(double v)
    {
        x += v;
        y += v;
        z += v;
        return *this;
    }

    const Vec3 operator-()
    {
        return Vec3(-x, -y, -z);
    }

    const Vec3 operator-(double v)
    {
        return Vec3(x - v, y - v, z - v);
    }

    const Vec3 operator-=(double v)
    {
        x -= v;
        y -= v;
        z -= v;
        return *this;
    }

    const Vec3 operator*(double v)
    {
        return Vec3(x * v, y * v, z * v);
    }

    const Vec3 operator*=(double v)
    {
        x *= v;
        y *= v;
        z *= v;
        return *this;
    }

    const Vec3 operator/(double v)
    {
        return Vec3(x / v, y / v, z / v);
    }

    const Vec3 operator/=(double v)
    {
        x /= v;
        y /= v;
        z /= v;
        return *this;
    }

    const Vec3 operator+(Vec3 v)
    {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }

    const Vec3 operator+=(Vec3 v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    const Vec3 operator-(Vec3 v)
    {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }

    const Vec3 operator-=(Vec3 v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    const Vec3 operator*(Vec3 v)
    {
        return Vec3(x * v.x, y * v.y, z * v.z);
    }

    const Vec3 operator*=(Vec3 v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    const Vec3 operator/(Vec3 v)
    {
        return Vec3(x / v.x, y / v.y, z / v.z);
    }

    const Vec3 operator/=(Vec3 v)
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }

    const bool operator==(Vec3 v)
    {
        return (v.x == x && v.y == y && v.z == z);
    }

    const bool operator!=(Vec3 v)
    {
        return (v.x != x || v.y != y || v.z != z);
    }

    double length();

    double x, y, z;
};

class Vec4
{
public:
    Vec4();
    Vec4(double x, double y, double z, double w);

    const void operator=(Vec4 v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        w = v.w;
    }

    const Vec4 operator+(double v)
    {
        return Vec4(x + v, y + v, z + v, w + v);
    }

    const Vec4 operator+=(double v)
    {
        x += v;
        y += v;
        z += v;
        w += v;
        return *this;
    }

    const Vec4 operator-()
    {
        return Vec4(-x, -y, -z, -w);
    }

    const Vec4 operator-(double v)
    {
        return Vec4(x - v, y - v, z - v, w - v);
    }

    const Vec4 operator-=(double v)
    {
        x -= v;
        y -= v;
        z -= v;
        w -= v;
        return *this;
    }

    const Vec4 operator*(double v)
    {
        return Vec4(x * v, y * v, z * v, w * v);
    }

    const Vec4 operator*=(double v)
    {
        x *= v;
        y *= v;
        z *= v;
        w *= v;
        return *this;
    }

    const Vec4 operator/(double v)
    {
        return Vec4(x / v, y / v, z / v, w / v);
    }

    const Vec4 operator/=(double v)
    {
        x /= v;
        y /= v;
        z /= v;
        w /= v;
        return *this;
    }

    const Vec4 operator+(Vec4 v)
    {
        return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);
    }

    const Vec4 operator+=(Vec4 v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
        return *this;
    }

    const Vec4 operator-(Vec4 v)
    {
        return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);
    }

    const Vec4 operator-=(Vec4 v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
        return *this;
    }

    const Vec4 operator*(Vec4 v)
    {
        return Vec4(x * v.x, y * v.y, z * v.z, w * v.w);
    }

    const Vec4 operator*=(Vec4 v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        w *= v.w;
        return *this;
    }

    const Vec4 operator/(Vec4 v)
    {
        return Vec4(x / v.x, y / v.y, z / v.z, w / v.w);
    }

    const Vec4 operator/=(Vec4 v)
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        w /= v.w;
        return *this;
    }

    const bool operator==(Vec4 v)
    {
        return (v.x == x && v.y == y && v.z == z && v.w == w);
    }

    const bool operator!=(Vec4 v)
    {
        return (v.x != x || v.y != y || v.z != z || v.w != w);
    }

    double length();

    double x, y, z, w;
};

double angle(Vec2 a, Vec2 b);
double angle(Vec3 a, Vec3 b);
double angle(Vec4 a, Vec4 b);

double dot(Vec2 a, Vec2 b);
double dot(Vec3 a, Vec3 b);
double dot(Vec4 a, Vec4 b);

double interp_linear(double a, double b, double t);
double interp_cosine(double a, double b, double t);

Vec3 midpoint_linear(Vec3 a, Vec3 b, double va, double vb, double t);

double perlin_value(int seed, double x, double y);
double perlin_value(int seed, double x, double y, double z);

double fractal_noise(int seed, double x, double y, unsigned int octaves, float persistance);
double fractal_noise(int seed, double x, double y, double z, unsigned int octaves, float persistance);

unsigned int gen_random(int seed, int x);
unsigned int gen_random(int seed, int x, int y);
unsigned int gen_random(int seed, int x, int y, int z);

int gen_random_large(int seed, int x);
int gen_random_large(int seed, int x, int y);
int gen_random_large(int seed, int x, int y, int z);

Vec2 normalize(Vec2 v);
Vec3 normalize(Vec3 v);
Vec4 normalize(Vec4 v);

Vec3 cross(Vec3 a, Vec3 b);

class Mat4x4
{
public:
    Mat4x4();

    float* get_readable_array(float* blank);

    double m[4][4];

    const Mat4x4 operator*(Mat4x4 mat)
    {
        Mat4x4 matrix;
        for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
        {
            Vec4 a(m[0][j], m[1][j], m[2][j], m[3][j]);
            Vec4 b(mat.m[i][0], mat.m[i][1], mat.m[i][2], mat.m[i][3]);

            matrix.m[i][j] = dot(a, b);
        }

        return matrix;
    }

    const Vec4 operator*(Vec4 vec)
    {
        Vec4 vector;
        
        vector.x = m[0][0] * vec.x + m[1][0] * vec.y + m[2][0] * vec.z + m[3][0] * vec.w;
        vector.y = m[0][1] * vec.x + m[1][1] * vec.y + m[2][1] * vec.z + m[3][1] * vec.w;
        vector.z = m[0][2] * vec.x + m[1][2] * vec.y + m[2][2] * vec.z + m[3][2] * vec.w;
        vector.w = m[0][3] * vec.x + m[1][3] * vec.y + m[2][3] * vec.z + m[3][3] * vec.w;

        return vector;
    }
};

class Ray
{
public:
    Ray(Vec3 a, Vec3 b);

    Vec3 get_point(double t);
    Vec3 get_point_closest(Vec3 point);

    Vec3 get_difference();
    Vec3 get_direction() const { return delta; }

    Vec3 get_origin() const { return a; }

    void set_recent_t(double t);
    double get_recent_t() const { return t_; }

private:
    Vec3 a, b, delta;
    double t_;
};

class Plane
{
public:
    Plane();
    Plane(Vec3 a, Vec3 b, Vec3 c);

    Vec3 line_intersection(Ray ray);
    double line_intersection_val(Ray ray);

    Vec3 get_normal();

    bool is_parallel(Plane plane);

private:
    double v_a, v_b, v_c, v_d;
};

class Triangle
{
public:
    Triangle();
    Triangle(Vec3 a, Vec3 b, Vec3 c);

    double triplane_intesection_val(Ray ray);

    bool ray_intersects(Ray& ray);

    Vec3 get_a() const { return a; }
    Vec3 get_b() const { return b; }
    Vec3 get_c() const { return c; }

    Plane get_plane() const { return plane; }
    Vec3 get_normal();

    void set_a(Vec3 val);
    void set_b(Vec3 val);
    void set_c(Vec3 val);

    void transform(Vec3 translation, Vec3 rotation, Vec3 scale);

    double area();

    const Triangle operator+(Vec3 vec)
    {
        return Triangle(a + vec, b + vec, c + vec);
    }

    const Triangle operator-(Vec3 vec)
    {
        return Triangle(a - vec, b - vec, c - vec);
    }

    const Triangle operator*(Vec3 vec)
    {
        return Triangle(a * vec, b * vec, c * vec);
    }

    const Triangle operator/(Vec3 vec)
    {
        return Triangle(a / vec, b / vec, c / vec);
    }

private:
    Vec3 a, b, c;
    Vec3 o_a, o_b, o_c;

    Plane plane;
};

float midpoint(float a, float b, float t);
float fade(float t);

bool point_within_rectangle(Vec3 rec_a, Vec3 rec_b, Vec3 point);

Mat4x4 mat_transform(Vec3 translation, Vec3 rotation, Vec3 scale);
Mat4x4 mat_perspective(double fov, double aspect, double near, double far);
Mat4x4 mat_view(Vec3 position, Vec3 target, Vec3 up);
Mat4x4 mat_ortho(float left, float right, float bottom, float top, float near, float far);

unsigned int read_block(char* block, bool big_endian);

#endif