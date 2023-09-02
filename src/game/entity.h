#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "../rendering/mesh.h"
#include "../rendering/renderer.h"
#include "../rendering/shader.h"

#include "../utils/utilmath.h"

class Entity
{
    public:
        Entity(Vec3 pos);

        void draw(Renderer& renderer, Shader& shader);

        virtual void generate(unsigned int seed) = 0;
    protected:
        Vec3 pos;
        Mesh* mesh;
};

class EntityTree : public Entity
{
    public:
        using Entity::Entity;

        void generate(unsigned int seed);
    
};

#endif