#include "maze.h"
#include <list>
#include "cmath"
#include "walls.h"
using namespace std;

Maze::Maze(float x, float y, list<Wall> walls)
{
    this->position = glm::vec3(x, y, 0.0);
    this->rotation = 0;
    speed = 1;
    double theta = 2.0 * 3.14159 / 11.0;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices

    // list<Wall> walls = create_maze(maze_size);

    int wall_count = walls.size();

    GLfloat vertex_buffer_data[wall_count * 6];

    int wall_ind = 0;
    for (auto it = walls.begin(); it != walls.end(); it++)
    {
        vertex_buffer_data[wall_ind * 6] = (*it).x1;
        vertex_buffer_data[wall_ind * 6 + 1] = (*it).y1;
        vertex_buffer_data[wall_ind * 6 + 2] = 0;
        vertex_buffer_data[wall_ind * 6 + 3] = (*it).x2;
        vertex_buffer_data[wall_ind * 6 + 4] = (*it).y2;
        vertex_buffer_data[wall_ind * 6 + 5] = 0;

        wall_ind++;
    }

    // GLfloat colors[180];

    // for (int i = 0; i < 180; i++)
    // {
    //     colors[i] = (float)rand() / RAND_MAX;
    // }

    this->object = create3DObject(GL_LINES, wall_count * 2, vertex_buffer_data, COLOR_RED, GL_FILL);
}

void Maze::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Maze::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0.0);
}

void Maze::tick(int dirn)
{
    this->rotation += dirn * speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}
