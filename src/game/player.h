#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "world.h"

#include "../../lib/GLFW/glfw3.h"

class Player
{
    public:
        Player();

        Vec3 get_pos() const { return pos; }
        Vec3 get_rot() const { return rot; }

        void set_pos(Vec3 val);
        void set_rot(Vec3 val);

        void update_camera(double x, double y, double sensitivity);
        void update_collision(World* world);
        void update_matrices(Shader& shader);
        void update_movement(GLFWwindow* window, bool is_freemoving);
    private:
        Vec3 pos, rot, vel, pos_prev;
        
        float speed, cur_grav;
        double cam_x, cam_y, prev_x, prev_y;
};

#endif