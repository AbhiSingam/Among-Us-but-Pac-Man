#include "main.h"
#include "walls.h"

#ifndef HUD_H
#define HUD_H

class Hud
{
public:
    Hud() {}
    Hud(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void move(int dirn);
    double speed;

private:
    VAO *object;
};

#endif // HUD_H
