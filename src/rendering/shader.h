#ifndef _SHADER_H_
#define _SHADER_H_

#include <vector>

#include "../utils/utilmath.h"

class Shader
{
    public:
        Shader();

        void add_shader(const char* source, unsigned int type);

        void compile();
        void destroy();

        void use();

        void transfer_int(const char* name, int val);
        void transfer_float(const char* name, float val);

        void transfer_vec2(const char* name, Vec2 val);
        void transfer_vec3(const char* name, Vec3 val);
        void transfer_vec4(const char* name, Vec4 val);

        void transfer_mat4(const char* name, Mat4x4 val);
    private:
        unsigned int program;

        std::vector<unsigned int> shaders;
};

#endif