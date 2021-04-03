#include <list>
#include <utility>
#include <main.h>
#include <unistd.h>
using namespace std;

#ifndef WALLS_H
#define WALLS_H

class Wall
{
public:
    bool edge;
    float x1;
    float y1;
    float x2;
    float y2;

    Wall(float a, float b, float c, float d, float n)
    {
        x1 = a;
        y1 = b;
        x2 = c;
        y2 = d;
        if ((b == n - 1 || b == 0) && b == d)
        {
            edge = true;
        }
        else if ((a == 0 || a == n - 1) && a == c)
        {
            edge = true;
        }
        else
        {
            edge = false;
        }
    }
};

class Edge
{
public:
    int x1;
    int y1;
    int x2;
    int y2;

    Edge(int a, int b, int c, int d)
    {
        x1 = a;
        y1 = b;
        x2 = c;
        y2 = d;
    }
};

extern list<Edge> edges;

list<Wall> create_maze(int n);

#endif // WALLS_H
