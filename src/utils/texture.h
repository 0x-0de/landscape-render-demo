#ifndef _TEXTURE_H_
#define _TEXTURE_H_

class Texture
{
public:
    Texture(const char* filepath, bool should_filter, bool should_genmipmap);

    void bind();
private:
    void load_texture(const char* filepath, bool uses_alpha, unsigned int* w, unsigned int* h, unsigned int* ch);
    unsigned char* data;
    char* raw_data;

    unsigned int texture;
};

#endif