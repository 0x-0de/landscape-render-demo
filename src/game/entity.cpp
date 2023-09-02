#include "entity.h"

//Base entity class.

Entity::Entity(Vec3 pos) : pos(pos) {}

void Entity::draw(Renderer& renderer, Shader& shader)
{
    shader.transfer_mat4("transform", mat_transform(pos, Vec3(0, 0, 0), Vec3(1, 1, 1)));
    renderer.draw_mesh(mesh);
}