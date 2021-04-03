#include "star.h"
#include <list>
#include "cmath"
using namespace std;

Star::Star(float x, float y, color_t color)
{
    this->position = glm::vec3(x, y, 0.0);
    this->rotation = 0;
    speed = 1;

    GLfloat vertex_buffer_data[5*9]={
        0.166, 0.0, 0.0,
        0.4, 0.666, 0.0, 
        0.7, 0.4, 0.0,
        0.833, 0.0, 0.0, 
        0.3, 0.4, 0.0, 
        0.6, 0.666, 0.0,
        1.0, 0.666, 0.0,
        0.5, 0.25, 0.0,
        0.4, 0.666, 0.0,
        0.5, 1.0, 0.0,
        0.3, 0.4, 0.0,
        0.7, 0.4, 0.0,
        0.0, 0.666, 0.0,
        0.5, 0.25, 0.0,
        0.6, 0.666, 0.0
    };

    // GLfloat colors[180];

    // for (int i = 0; i < 180; i++)
    // {
    //     colors[i] = (float)rand() / RAND_MAX;
    // }

    this->object = create3DObject(GL_TRIANGLES, 5 * 3, vertex_buffer_data, color, GL_FILL);
}

void Star::draw(glm::mat4 VP)
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

void Star::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0.0);
}

void Star::tick(int dirn)
{
    this->rotation += dirn * speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}
