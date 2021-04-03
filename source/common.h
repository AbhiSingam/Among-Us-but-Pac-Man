#include <player.h>
#include <walls.h>
#include <vector>

using namespace std;

#ifndef COMMON_H
#define COMMON_H

extern Player player;
extern list<Wall> walls;
extern int maze_size;
extern vector<pair<int,int>> adj[20][20];

#endif // MAZE_H
