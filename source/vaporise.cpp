#include "vaporise.h"
#include <list>
#include "cmath"
#include "walls.h"
using namespace std;

Vaporise::Vaporise(float x, float y, color_t color)
{
    this->position = glm::vec3(x, y, 0.0);
    this->rotation = 0;
    speed = 1;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices

    // list<Wall> walls = create_maze(maze_size);

    int head_triangles = 1000;

    GLfloat vertex_buffer_data[(head_triangles) * 9];

    for (int i = 0; i < head_triangles; i++)
    {
        vertex_buffer_data[9 * i] = 0.5;
        vertex_buffer_data[9 * i + 1] = 0.5;
        vertex_buffer_data[9 * i + 2] = 0;
        vertex_buffer_data[9 * i + 3] = 0.5 + 0.4 * cos((2.0 * M_PI * (float)i) / (float)head_triangles);
        vertex_buffer_data[9 * i + 4] = 0.5 + 0.4 * sin((2.0 * M_PI * (float)i) / (float)head_triangles);
        vertex_buffer_data[9 * i + 5] = 0;
        vertex_buffer_data[9 * i + 6] = 0.5 + 0.4 * cos((2.0 * M_PI * ((float)i + 1)) / (float)head_triangles);
        vertex_buffer_data[9 * i + 7] = 0.5 + 0.4 * sin((2.0 * M_PI * ((float)i + 1)) / (float)head_triangles);
        vertex_buffer_data[9 * i + 8] = 0;
    }

    // GLfloat colors[180];

    // for (int i = 0; i < 180; i++)
    // {
    //     colors[i] = (float)rand() / RAND_MAX;
    // }

    this->object = create3DObject(GL_TRIANGLES, head_triangles * 3, vertex_buffer_data, color, GL_FILL);
}

void Vaporise::draw(glm::mat4 VP)
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

void Vaporise::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0.0);
}

void Vaporise::tick(int dirn)
{
    this->rotation += dirn * speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}
