#ifndef _FONT_H_
#define _FONT_H_

#include <string>
#include <vector>

#include "../rendering/mesh.h"

struct Character
{
    char c;
    unsigned int x, y, width, offset;
};

class Font
{
public:
    Font(const char* filepath);

    void get_message_mesh(Polygon* mesh, std::string message);
private:
    const char* filepath;
    std::vector<Character> characters;

    std::string name, author;
    unsigned int height;
    float size;

    int get_character(char c);
};

#endif