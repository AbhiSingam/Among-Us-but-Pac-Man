#include "main.h"
#include "walls.h"

#ifndef MAZE_H
#define MAZE_H

class Maze
{
public:
    Maze() {}
    Maze(float x, float y, list<Wall> walls);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(int dirn);
    double speed;

private:
    VAO *object;
};

#endif // MAZE_H
