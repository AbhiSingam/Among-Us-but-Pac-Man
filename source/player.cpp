#include "player.h"
#include <list>
#include "cmath"
#include "common.h"
using namespace std;

Player::Player(float x, float y, color_t color)
{
    this->position = glm::vec3(x, y, 0.0);
    this->rotation = 0;
    speed = 1;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices

    int head_triangles = 1000;

    GLfloat vertex_buffer_data[(head_triangles + 8) * 9];

    for (int i = 0; i < head_triangles; i++)
    {
        vertex_buffer_data[9 * i] = 0.5;
        vertex_buffer_data[9 * i + 1] = 0.6;
        vertex_buffer_data[9 * i + 2] = 0;
        vertex_buffer_data[9 * i + 3] = 0.5 + 0.3 * cos((M_PI * (float)i) / (float)head_triangles);
        vertex_buffer_data[9 * i + 4] = 0.6 + 0.3 * sin((M_PI * (float)i) / (float)head_triangles);
        vertex_buffer_data[9 * i + 5] = 0;
        vertex_buffer_data[9 * i + 6] = 0.5 + 0.3 * cos((M_PI * ((float)i + 1)) / (float)head_triangles);
        vertex_buffer_data[9 * i + 7] = 0.6 + 0.3 * sin((M_PI * ((float)i + 1)) / (float)head_triangles);
        vertex_buffer_data[9 * i + 8] = 0;
    }

    // left foot triangle 1
    vertex_buffer_data[9 * head_triangles] = 0.2;
    vertex_buffer_data[9 * head_triangles + 1] = 0.25;
    vertex_buffer_data[9 * head_triangles + 2] = 0;
    vertex_buffer_data[9 * head_triangles + 3] = 0.2;
    vertex_buffer_data[9 * head_triangles + 4] = 0.1;
    vertex_buffer_data[9 * head_triangles + 5] = 0;
    vertex_buffer_data[9 * head_triangles + 6] = 0.45;
    vertex_buffer_data[9 * head_triangles + 7] = 0.25;
    vertex_buffer_data[9 * head_triangles + 8] = 0;

    // left foot triangle 2
    vertex_buffer_data[9 * (head_triangles + 1)] = 0.45;
    vertex_buffer_data[9 * (head_triangles + 1) + 1] = 0.1;
    vertex_buffer_data[9 * (head_triangles + 1) + 2] = 0;
    vertex_buffer_data[9 * (head_triangles + 1) + 3] = 0.2;
    vertex_buffer_data[9 * (head_triangles + 1) + 4] = 0.1;
    vertex_buffer_data[9 * (head_triangles + 1) + 5] = 0;
    vertex_buffer_data[9 * (head_triangles + 1) + 6] = 0.45;
    vertex_buffer_data[9 * (head_triangles + 1) + 7] = 0.25;
    vertex_buffer_data[9 * (head_triangles + 1) + 8] = 0;

    // right foot triangle 1
    vertex_buffer_data[9 * (head_triangles + 2)] = 0.55;
    vertex_buffer_data[9 * (head_triangles + 2) + 1] = 0.25;
    vertex_buffer_data[9 * (head_triangles + 2) + 2] = 0;
    vertex_buffer_data[9 * (head_triangles + 2) + 3] = 0.8;
    vertex_buffer_data[9 * (head_triangles + 2) + 4] = 0.25;
    vertex_buffer_data[9 * (head_triangles + 2) + 5] = 0;
    vertex_buffer_data[9 * (head_triangles + 2) + 6] = 0.55;
    vertex_buffer_data[9 * (head_triangles + 2) + 7] = 0.1;
    vertex_buffer_data[9 * (head_triangles + 2) + 8] = 0;

    // right foot triangle 2
    vertex_buffer_data[9 * (head_triangles + 3)] = 0.8;
    vertex_buffer_data[9 * (head_triangles + 3) + 1] = 0.1;
    vertex_buffer_data[9 * (head_triangles + 3) + 2] = 0;
    vertex_buffer_data[9 * (head_triangles + 3) + 3] = 0.8;
    vertex_buffer_data[9 * (head_triangles + 3) + 4] = 0.25;
    vertex_buffer_data[9 * (head_triangles + 3) + 5] = 0;
    vertex_buffer_data[9 * (head_triangles + 3) + 6] = 0.55;
    vertex_buffer_data[9 * (head_triangles + 3) + 7] = 0.1;
    vertex_buffer_data[9 * (head_triangles + 3) + 8] = 0;

    // body triangle 1
    vertex_buffer_data[9 * (head_triangles + 4)] = 0.2;
    vertex_buffer_data[9 * (head_triangles + 4) + 1] = 0.6;
    vertex_buffer_data[9 * (head_triangles + 4) + 2] = 0;
    vertex_buffer_data[9 * (head_triangles + 4) + 3] = 0.8;
    vertex_buffer_data[9 * (head_triangles + 4) + 4] = 0.6;
    vertex_buffer_data[9 * (head_triangles + 4) + 5] = 0;
    vertex_buffer_data[9 * (head_triangles + 4) + 6] = 0.2;
    vertex_buffer_data[9 * (head_triangles + 4) + 7] = 0.25;
    vertex_buffer_data[9 * (head_triangles + 4) + 8] = 0;

    // body triangle 2
    vertex_buffer_data[9 * (head_triangles + 5)] = 0.8;
    vertex_buffer_data[9 * (head_triangles + 5) + 1] = 0.25;
    vertex_buffer_data[9 * (head_triangles + 5) + 2] = 0;
    vertex_buffer_data[9 * (head_triangles + 5) + 3] = 0.8;
    vertex_buffer_data[9 * (head_triangles + 5) + 4] = 0.6;
    vertex_buffer_data[9 * (head_triangles + 5) + 5] = 0;
    vertex_buffer_data[9 * (head_triangles + 5) + 6] = 0.2;
    vertex_buffer_data[9 * (head_triangles + 5) + 7] = 0.25;
    vertex_buffer_data[9 * (head_triangles + 5) + 8] = 0;

    // visor triangle 1
    vertex_buffer_data[9 * (head_triangles + 6)] = 0.3;
    vertex_buffer_data[9 * (head_triangles + 6) + 1] = 0.65;
    vertex_buffer_data[9 * (head_triangles + 6) + 2] = 0;
    vertex_buffer_data[9 * (head_triangles + 6) + 3] = 0.7;
    vertex_buffer_data[9 * (head_triangles + 6) + 4] = 0.65;
    vertex_buffer_data[9 * (head_triangles + 6) + 5] = 0;
    vertex_buffer_data[9 * (head_triangles + 6) + 6] = 0.3;
    vertex_buffer_data[9 * (head_triangles + 6) + 7] = 0.5;
    vertex_buffer_data[9 * (head_triangles + 6) + 8] = 0;

    // visor triangle 2
    vertex_buffer_data[9 * (head_triangles + 7)] = 0.7;
    vertex_buffer_data[9 * (head_triangles + 7) + 1] = 0.5;
    vertex_buffer_data[9 * (head_triangles + 7) + 2] = 0;
    vertex_buffer_data[9 * (head_triangles + 7) + 3] = 0.7;
    vertex_buffer_data[9 * (head_triangles + 7) + 4] = 0.65;
    vertex_buffer_data[9 * (head_triangles + 7) + 5] = 0;
    vertex_buffer_data[9 * (head_triangles + 7) + 6] = 0.3;
    vertex_buffer_data[9 * (head_triangles + 7) + 7] = 0.5;
    vertex_buffer_data[9 * (head_triangles + 7) + 8] = 0;

    GLfloat colors[(head_triangles + 8) * 9];

    for (int i = 0; i < (head_triangles + 6) * 3; i++)
    {
        colors[3 * i] = (float)color.r/255;
        colors[3 * i + 1] = (float)color.g/255;
        colors[3 * i + 2] = (float)color.b/255;
    }

    for (int i = (head_triangles + 6) * 3; i < (head_triangles + 8) * 3; i++)
    {
        colors[3 * i] = (float)COLOR_GREY.r / 255;
        colors[3 * i + 1] = (float)COLOR_GREY.g/255;
        colors[3 * i + 2] = (float)COLOR_GREY.b / 255;
    }

    // for (int i = 0; i < (head_triangles + 8) * 9; i++)
    // {
    //     colors[i] = rand()
    // }

    this->object = create3DObject(GL_TRIANGLES, (head_triangles + 8) * 3, vertex_buffer_data, colors, GL_FILL);
}

void Player::draw(glm::mat4 VP)
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

void Player::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0.0);
}

void Player::move(int dirn)
{
    // this->position.x -= speed;
    // this->position.y -= speed;
    if(dirn == 1)
    {
        // +x
        for(int i = 0; i<adj[(int)this->position.x][(int)this->position.y].size(); i++)
        {
            if (adj[(int)this->position.x][(int)this->position.y][i].first == ((int)this->position.x + 1) && adj[(int)this->position.x][(int)this->position.y][i].second == (int)this->position.y)
            {
                this->position.x += 1;
                break;
            }
        }
    }
    else if(dirn == -1)
    {
        // -x
        for (int i = 0; i < adj[(int)this->position.x][(int)this->position.y].size(); i++)
        {
            if (adj[(int)this->position.x][(int)this->position.y][i].first == ((int)this->position.x - 1) && adj[(int)this->position.x][(int)this->position.y][i].second == (int)this->position.y)
            {
                this->position.x -= 1;
                break;
            }
        }
    }
    else if(dirn == 2)
    {
        // +y
        for (int i = 0; i < adj[(int)this->position.x][(int)this->position.y].size(); i++)
        {
            if (adj[(int)this->position.x][(int)this->position.y][i].first == (int)this->position.x && adj[(int)this->position.x][(int)this->position.y][i].second == ((int)this->position.y + 1))
            {
                this->position.y += 1;
                break;
            }
        }
    }
    else if(dirn == -2)
    {
        // -y
        for (int i = 0; i < adj[(int)this->position.x][(int)this->position.y].size(); i++)
        {
            if (adj[(int)this->position.x][(int)this->position.y][i].first == (int)this->position.x && adj[(int)this->position.x][(int)this->position.y][i].second == ((int)this->position.y - 1))
            {
                this->position.y -= 1;
                break;
            }
        }
    }
}
