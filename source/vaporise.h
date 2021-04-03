#include "main.h"
#include "walls.h"

#ifndef VAPORISE_H
#define VAPORISE_H

class Vaporise
{
public:
    Vaporise() {}
    Vaporise(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(int dirn);
    double speed;

private:
    VAO *object;
};

#endif // VAPORISE_H
