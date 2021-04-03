#include "powerup.h"
#include <list>
#include "cmath"
#include "common.h"
using namespace std;

Powerup::Powerup(float x, float y, int value)
{
    this->position = glm::vec3(x, y, 0.0);
    this->rotation = 0;
    this->value = value;
    speed = 1;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices

    int head_triangles = 1000;

    GLfloat vertex_buffer_data[(head_triangles)*9 + 6 * 6];

    for (int i = 0; i < head_triangles; i++)
    {
        vertex_buffer_data[9 * i] = 0.5;
        vertex_buffer_data[9 * i + 1] = 0.5;
        vertex_buffer_data[9 * i + 2] = 0;
        vertex_buffer_data[9 * i + 3] = 0.5 + 0.3 * cos((2 * M_PI * (float)i) / (float)head_triangles);
        vertex_buffer_data[9 * i + 4] = 0.5 + 0.3 * sin((2 * M_PI * (float)i) / (float)head_triangles);
        vertex_buffer_data[9 * i + 5] = 0;
        vertex_buffer_data[9 * i + 6] = 0.5 + 0.3 * cos((2 * M_PI * ((float)i + 1)) / (float)head_triangles);
        vertex_buffer_data[9 * i + 7] = 0.5 + 0.3 * sin((2 * M_PI * ((float)i + 1)) / (float)head_triangles);
        vertex_buffer_data[9 * i + 8] = 0;
    }

    int xr = head_triangles * 9;

    vertex_buffer_data[xr] = 0.35;
    vertex_buffer_data[xr + 1] = 0.35;
    vertex_buffer_data[xr + 2] = 0.0;

    vertex_buffer_data[xr + 3] = 0.44;
    vertex_buffer_data[xr + 4] = 0.65;
    vertex_buffer_data[xr + 5] = 0.0;

    vertex_buffer_data[xr + 6] = 0.44;
    vertex_buffer_data[xr + 7] = 0.65;
    vertex_buffer_data[xr + 8] = 0.0;

    vertex_buffer_data[xr + 9] = 0.5;
    vertex_buffer_data[xr + 10] = 0.4;
    vertex_buffer_data[xr + 11] = 0.0;

    vertex_buffer_data[xr + 12] = 0.5;
    vertex_buffer_data[xr + 13] = 0.4;
    vertex_buffer_data[xr + 14] = 0.0;

    vertex_buffer_data[xr + 15] = 0.56;
    vertex_buffer_data[xr + 16] = 0.65;
    vertex_buffer_data[xr + 17] = 0.0;

    vertex_buffer_data[xr + 18] = 0.56;
    vertex_buffer_data[xr + 19] = 0.65;
    vertex_buffer_data[xr + 20] = 0.0;

    vertex_buffer_data[xr + 21] = 0.65;
    vertex_buffer_data[xr + 22] = 0.35;
    vertex_buffer_data[xr + 23] = 0.0;

    vertex_buffer_data[xr + 24] = 0.35;
    vertex_buffer_data[xr + 25] = 0.47;
    vertex_buffer_data[xr + 26] = 0.0;

    vertex_buffer_data[xr + 27] = 0.65;
    vertex_buffer_data[xr + 28] = 0.47;
    vertex_buffer_data[xr + 29] = 0.0;

    vertex_buffer_data[xr + 30] = 0.35;
    vertex_buffer_data[xr + 31] = 0.53;
    vertex_buffer_data[xr + 32] = 0.0;

    vertex_buffer_data[xr + 33] = 0.65;
    vertex_buffer_data[xr + 34] = 0.53;
    vertex_buffer_data[xr + 35] = 0.0;

    GLfloat colors[(head_triangles)*9];
    GLfloat colors2[(head_triangles)*9 + 6 * 6];

    // for (int i = 0; i < (head_triangles + 8) * 9; i++)
    // {
    //     colors[i] = rand()
    // }

    // this->object = create3DObject(GL_TRIANGLES, (head_triangles) * 3, vertex_buffer_data, color, GL_FILL);

    for (int i = 0; i < head_triangles * 3; i++)
    {
        colors2[3 * i] = COLOR_YELLOW.r / 256.0;
        colors2[3 * i + 1] = COLOR_YELLOW.g / 256.0;
        colors2[3 * i + 2] = COLOR_YELLOW.b / 256.0;

        if ((i % head_triangles) > head_triangles / 2)
        {
            // cout << i << " yellow " << endl;
            colors[3 * i] = COLOR_YELLOW.r / 256.0;
            colors[3 * i + 1] = COLOR_YELLOW.g / 256.0;
            colors[3 * i + 2] = COLOR_YELLOW.b / 256.0;
        }
        else
        {
            // cout << i << " black " << endl;
            colors[3 * i] = COLOR_BLACK.r / 256.0;
            colors[3 * i + 1] = COLOR_BLACK.g / 256.0;
            colors[3 * i + 2] = COLOR_BLACK.b / 256.0;
        }
    }

    for (int i=0; i<12; i++)
    {
        colors2[xr + 3 * i] = COLOR_BLACK.r / 256.0;
        colors2[xr + 3 * i + 1] = COLOR_BLACK.g / 256.0;
        colors2[xr + 3 * i + 2] = COLOR_BLACK.b / 256.0;
    }

    if (value < 0)
    {
        this->object = create3DObject(GL_TRIANGLES, head_triangles * 3, vertex_buffer_data, colors, GL_FILL);
    }
    else
    {
        this->object = create3DObject(GL_LINES, head_triangles * 3 + 6 * 2, vertex_buffer_data, colors2, GL_FILL);
    }
}

void Powerup::draw(glm::mat4 VP)
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

void Powerup::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0.0);
}
