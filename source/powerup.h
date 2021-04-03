#include "main.h"

#ifndef POWERUP_H
#define POWERUP_H

class Powerup
{
public:
    Powerup() {}
    Powerup(float x, float y, int value);
    glm::vec3 position;
    int value;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    double speed;

private:
    VAO *object;
};

#endif // POWERUP_H
