#include "texture.h"

#include <fstream>
#include <iostream>

#include "../utils/utilmath.h"

#include "../../lib/GL/glew.h"

Texture::Texture(const char* filepath, bool should_filter, bool should_genmipmap)
{
    unsigned int width, height, channels;
    load_texture(filepath, true, &width, &height, &channels);

    glGenTextures(1, &texture);
    bind();

    int filter = should_filter ? GL_LINEAR : GL_NEAREST;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data);
    if(should_genmipmap) glGenerateMipmap(GL_TEXTURE_2D);

    delete[] data;
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::load_texture(const char* filepath, bool uses_alpha, unsigned int* w, unsigned int* h, unsigned int* ch)
{
    //BMP File structure:

    //Bytes 0-1: "BM" signature
    //Bytes 2-5: Size of BMP file, in bytes (header + 16 bytes data)
    //Bytes 6-9: Unused
    //Bytes 10-13: Offset of the pixel array
    //Bytes 14-17: Number of bytes in the DIB header (from this point)
    //Bytes 18-21: Image width
    //Bytes 22-25: Image height
    //Bytes 26-27: Number of color planes used
    //Bytes 28-29: Number of bits per pixel
    //Bytes 30-33: Type of compression used (usually 0, being BI_RGB, or 3, being BI_BITFIELDS).
    //Bytes 34-37: Size of the raw bitmap data, in bytes
    //Bytes 38-41: Print width of the bitmap (pixels per meter)
    //Bytes 42-45: Print height of the bitmap (pixels per meter)
    //Bytes 46-49: Number of colors in the pallete
    //Bytes 50-53: Number of 'important' colors (0 means they're all important).
    //Bytes 54-57 (if compression is BI_BITFIELDS): i'll finish this later.

    char width[4];
    char height[4];

    char offset[4];
    char data_size[4];

    std::ifstream reader(filepath, std::ios::binary);

    if(reader.good())
    {
        reader.seekg(10);
        reader.read(offset, 4);

        reader.seekg(18);
        reader.read(width, 4);
        reader.read(height, 4);

        *w = read_block(width, true);
        *h = read_block(height, true);
        *ch = 3 + uses_alpha;

        reader.seekg(34);
        reader.read(data_size, 4);

        unsigned int datasize = read_block(data_size, true);

        reader.seekg(read_block(offset, true));

        raw_data = new char[datasize];
        data = new unsigned char[datasize];
        reader.read(raw_data, datasize);

        for(unsigned int i = 0; i < datasize; i++)
        {
            data[i] = raw_data[i];
        }
    }
    else
    {
        std::cerr << "Cannot open texture file: " << filepath << std::endl;
    }

    delete[] raw_data;
}