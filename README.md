# landscape-render-demo
A graphical demo featuring a first-person camera roaming around a massive, infinite landscape with a very high draw distance.

This was originally a game project I worked on in November 2021. I scrapped it after discovering the horrors of floating-point rounding error.
It uses OpenGL, with GLEW and GLFW as dependencies.

The landscape is generated with fractal noise, and the meshes are built using marching tetrahedra. Mesh generation is multi-threaded, so when you
load the demo it'll take a while for everything to appear. There's also a fast day/night cycle, and a radiating light effect that should help give
you a sense of scale. It also has to do with what this game was going to be about in the first place. I'll keep it a secret in case I ever want
to revisit this idea.

Controls
----
You can move with WASD and look around with the mouse.
If you want your cursor to reappear, press ESC.
If you want to accelarate in speed, hold left shift.