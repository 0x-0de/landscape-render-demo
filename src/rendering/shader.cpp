#include "shader.h"

#include <iostream>

#include "../../lib/GL/glew.h"

Shader::Shader()
{
    program = glCreateProgram();
}

void Shader::add_shader(const char* source, unsigned int type)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    char log[512];
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, log);
        std::cerr << log << std::endl;
    }
    else
    {
        shaders.push_back(shader);
        glAttachShader(program, shader);
    }
}

void Shader::compile()
{
    glLinkProgram(program);

    int success;
    char log[512];
    
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(program, 512, NULL, log);
        std::cerr << log << std::endl;
    }

    glValidateProgram(program);

    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(program, 512, NULL, log);
        std::cerr << log << std::endl;
    }
}

void Shader::destroy()
{
    for(unsigned int i = 0; i < shaders.size(); i++)
    {
        glDetachShader(program, shaders[i]);
        glDeleteShader(shaders[i]);
    }

    glDeleteProgram(program);

    std::vector<unsigned int>().swap(shaders);
}

void Shader::use()
{
    glUseProgram(program);
}

void Shader::transfer_int(const char* name, int val)
{
    glUniform1i(glGetUniformLocation(program, name), val);
}

void Shader::transfer_float(const char* name, float val)
{
    glUniform1f(glGetUniformLocation(program, name), val);
}

void Shader::transfer_vec2(const char* name, Vec2 val)
{
    glUniform2f(glGetUniformLocation(program, name), val.x, val.y);
}

void Shader::transfer_vec3(const char* name, Vec3 val)
{
    glUniform3f(glGetUniformLocation(program, name), val.x, val.y, val.z);
}

void Shader::transfer_vec4(const char* name, Vec4 val)
{
    glUniform4f(glGetUniformLocation(program, name), val.x, val.y, val.z, val.w);
}

void Shader::transfer_mat4(const char* name, Mat4x4 val)
{
    float blank[16];
    glUniformMatrix4fv(glGetUniformLocation(program, name), 1, false, val.get_readable_array(blank));
}