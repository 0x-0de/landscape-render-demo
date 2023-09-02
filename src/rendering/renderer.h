#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "mesh.h"

extern double engine_delta, engine_time;
extern int engine_width, engine_height;

class Renderer
{
    public:
        Renderer();

        void draw_mesh(Mesh* mesh);
        void prepare();
};

#endif