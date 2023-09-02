#include "font.h"

#include <fstream>
#include <iostream>
#include <string>

Font::Font(const char* filepath) : filepath(filepath)
{
    bool readMode = false;
    height = 0;
    name = "Unknown";
    author = "Unknown";

    unsigned int cur_x = 0;
    unsigned int cur_y = 0;

    std::ifstream reader(filepath);
    if(reader.good())
    {
        std::string line;
        while(std::getline(reader, line))
        {
            if(!readMode)
            {
                if(line.substr(0, 5) == "name=")
                {
                    name = line.substr(5);
                }
                else if(line.substr(0, 7) == "author=")
                {
                    author = line.substr(7);
                }
                else if(line.substr(0, 7) == "height=")
                {
                    height = std::stoi(line.substr(7));
                }
                else if(line.substr(0, 5) == "size=")
                {
                    size = std::stoi(line.substr(5));
                }
                else if(line == "begin:") readMode = true;
            }
            else
            {
                if(line == "end;")
                {
                    readMode = false;
                    continue;
                }
                else if(line == "break")
                {
                    cur_x = 0;
                    cur_y += height;
                    continue;
                }

                Character character;
                for(unsigned int i = 0; i < line.length(); i++)
                {
                    char c = line[i];
                    if(c == ':' && i != 0)
                    {
                        character.c = line[i - 1];
                    }
                    if(c == '=' && i != 0)
                    {
                        switch(line[i - 1])
                        {
                            case 'w':
                            {
                                int length = 0;
                                for(unsigned int j = i + 1; j < line.length(); j++)
                                {
                                    if(line[j] == ',') break;
                                    length++;
                                }
                                int width = std::stoi(line.substr(i + 1, length));
                                character.width = width;
                                character.x = cur_x;
                                character.y = cur_y;
                                cur_x += width;
                            }
                            break;
                            case 'o':
                            {
                                int length = 0;
                                for(unsigned int j = i + 1; j < line.length(); j++)
                                {
                                    if(line[j] == ',') break;
                                    length++;
                                }
                                int offset = std::stoi(line.substr(i + 1, length));
                                character.offset = offset;
                            }
                            break;
                            default:
                                std::cerr << "Unknown symbol: " << line[i - 1] << std::endl;
                                break;
                        }
                    }
                }
                characters.push_back(character);
            }
        }
    }
    else
    {
        std::cerr << "Cannot read font file: " << filepath << std::endl;
    }
}

void Font::get_message_mesh(Polygon* polygon, std::string message)
{
    int cur_x = 0;
    int cur_y = 0;

    unsigned int index_count = 0;

    for(unsigned int i = 0; i < message.length(); i++)
    {
        int index = get_character(message[i]);
        if(message[i] == '\n')
        {
            cur_y -= height;
            cur_x = 0;
            continue;
        }

        float x = characters[index].x;
        float y = characters[index].y;
        int width = characters[index].width;
        int offset = characters[index].offset;

        polygon->add_vertex(create_vertex(Vec3(cur_x, cur_y - offset, 0), Vec2(x / size, (1 - y / size) - (float) height / size)));
        polygon->add_vertex(create_vertex(Vec3(cur_x + width, cur_y - offset, 0), Vec2((x + (float) width) / size, (1 - y / size) - (float) height / size)));
        polygon->add_vertex(create_vertex(Vec3(cur_x + width, cur_y + (signed) height - offset, 0), Vec2((x + (float) width) / size, (1 - y / size) - 0.001f)));
        polygon->add_vertex(create_vertex(Vec3(cur_x, cur_y + (signed) height - offset, 0), Vec2(x / size, (1 - y / size) - 0.001f)));

        //std::cout << x / size << ", " << (1 - y / size) - (float) height / size << ", " << (x + (float) width) / size << ", " << (1 - y / size) << std::endl;

        polygon->add_index(index_count + 0);
        polygon->add_index(index_count + 1);
        polygon->add_index(index_count + 2);
        polygon->add_index(index_count + 0);
        polygon->add_index(index_count + 3);
        polygon->add_index(index_count + 2);

        cur_x += width;
        index_count += 4;
    }

    polygon->build();
    polygon->clear_data();
}

int Font::get_character(char c)
{
    for(unsigned int i = 0; i < characters.size(); i++)
    {
        if(characters[i].c == c) return (signed) i;
    }

    return -1;
}